#include "Render.h"

#include <windows.h>

#define BACKGROUND ' '
#define BORDER_SEPARATION 0

Render::Render(bool active) : resolution(RESX, RESY)
{
}

Render::~Render()
{
}

bool Render::Start()
{
    screen = new WCHAR * [resolution.y];
    for (int y = 0; y < resolution.y; ++y)
    {
        screen[y] = new WCHAR[resolution.x + 1];
        screen[y][resolution.x] = '\0';
    }

    console = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(console);

    ClearScreen();

	return true;
}

void Render::Update(float deltaTime)
{
    ClearScreen();
    SortRequestsByLayer();
    RequestsToScreen();
    PrintScreenToConsole();
}

bool Render::CleanUp()
{
    for (int y = 0; y < resolution.y; ++y)
        delete[] screen[y];
    delete[] screen;

	return true;
}

void Render::RenderRectangle(unsigned int layer, WCHAR filler, fPoint position, iPoint size)
{
    RenderRequest request(RequestType::RECTANGLE);

    request.layer = layer;
    request.filler = filler;
    request.position = { (int)position.x,(int)position.y };
    request.size = { size.x, size.y };

    renderQueue.push_back(request);
}

void Render::RenderText(unsigned int layer, std::wstring text, fPoint position)
{
    RenderRequest request(RequestType::TEXT);

    request.layer = layer;
    request.text = text;
    request.position = { (int)position.x,(int)position.y };

    renderQueue.push_back(request);
}

void Render::Log(std::wstring text)
{
    DWORD bytesWritten = 0;

    wchar_t nothing[64] = L"                                                               ";
    WriteConsoleOutputCharacter(console, nothing, 64, { (SHORT)(0), (SHORT)(RESY + line) }, &bytesWritten);

    bytesWritten = 0;
    WriteConsoleOutputCharacter(console, text.c_str(), text.size(), { (SHORT)(0), (SHORT)(RESY + line) }, &bytesWritten);
    ++line;
    if (line >= 30)
        line = 0;
}

void Render::SortRequestsByLayer()
{
    if (renderQueue.size() == 0)
        return;
    while (true)
    {
        UINT changes = 0;
        for (UINT i = 0; i < renderQueue.size() - 1; ++i)
            if (renderQueue[i].layer > renderQueue[i + 1].layer)
            {
                RenderRequest buffer = renderQueue[i];
                renderQueue[i] = renderQueue[i + 1];
                renderQueue[i + 1] = buffer;
                ++changes;
            }
        if (changes == 0)
            break;
    }

}

void Render::RequestsToScreen()
{
    for (unsigned int i = 0u; i < renderQueue.size(); ++i)
        switch (renderQueue[i].type)
        {
        case RequestType::RECTANGLE:
            DrawRectangleRequest(renderQueue[i].filler, renderQueue[i].position, renderQueue[i].size);
            break;
        case RequestType::TEXT:
            DrawTextRequest(renderQueue[i].text, renderQueue[i].position);
            break;
        }
    renderQueue.clear();
}

void Render::PrintScreenToConsole()
{
    DWORD bytesWritten = 0;

    for (int y = 0; y < resolution.y; ++y)
        WriteConsoleOutputCharacter(console, screen[y], resolution.x + 1, 
            { 0, (SHORT)y }, &bytesWritten);
}

void Render::ClearScreen()
{
    for (int y = 0; y < resolution.y; ++y)
        for (int x = 0; x < resolution.x; ++x)
            screen[y][x] = BACKGROUND;     
}

void Render::DrawRectangleRequest(WCHAR filler, iPoint position, iPoint size)
{
    iPoint end = { position.x + size.x, position.y + size.y };
    if (end.x < position.x || end.y < position.y)
        return;

    for (int y = position.y; y < position.y + size.y; ++y)
    {
        if (y >= resolution.y)
            break;
        if (y < 0)
            continue;
        for (int x = position.x; x < position.x + size.x; ++x)
        {
            if (x >= resolution.x)
                break;
            if (x < 0)
                continue;
            screen[y][x] = filler;
        }
    }
}

void Render::DrawTextRequest(std::wstring text, iPoint position)
{
    int x = position.x;
    int y = position.y;
    for (unsigned int i = 0u; i < text.size(); ++i)
    {
        if (text[i] == '\n')
        {
            x = position.x;
            ++y;
            if (y >= resolution.y)
                break;
            continue;
        }

        if (x >= resolution.x || x < 0 || y < 0)
        {
            ++x;
            continue;
        }
        if (y >= resolution.y)
            break;

        screen[y][x] = text[i];
        ++x;
    }
}
