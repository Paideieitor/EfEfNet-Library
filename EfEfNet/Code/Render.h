#pragma once

#ifndef _RENDER_
#define _RENDER_

#include "Point.h"

#include <string>
#include <vector>

#define RESX 180
#define RESY 40

typedef wchar_t WCHAR;
typedef void *HANDLE;

class Render
{
public:

    Render(bool active);
    ~Render();

    bool Start();
    void Update(float deltaTime);
    bool CleanUp();

    void RenderRectangle(unsigned int layer, WCHAR filler, fPoint position, iPoint size);
    void RenderText(unsigned int layer, std::wstring text, fPoint position);

    void Log(std::wstring text);
    int line = 0;

private:

    enum class RequestType
    {
        NONE = 0,
        RECTANGLE,
        TEXT
    };

    struct RenderRequest
    {
        RenderRequest() : type(RequestType::NONE), layer(0u), filler(0), text(std::wstring()), position(iPoint()), size(iPoint()) {}
        RenderRequest(RequestType type) : type(type), layer(0u), filler(0), text(std::wstring()), position(iPoint()), size(iPoint()) {}

        RequestType type;

        unsigned int layer;

        WCHAR filler;
        std::wstring text;

        iPoint position;
        iPoint size;
    };

    void SortRequestsByLayer();
    void RequestsToScreen();
    void PrintScreenToConsole();
    void ClearScreen();

    void DrawRectangleRequest(WCHAR filler, iPoint position, iPoint size);
    void DrawTextRequest(std::wstring text, iPoint position);

    iPoint resolution;
    WCHAR** screen = nullptr;
    HANDLE console;

    std::vector<RenderRequest> renderQueue;
};

extern Render* render;

#endif //_RENDER_