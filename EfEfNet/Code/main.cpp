#include <Windows.h>
#include <WinUser.h>

#include "efefNet.h"
#include "efefStreams.h"

#include "Render.h"

#include <iostream>
#include <string>

char alphabet[26] = { 'A','B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'Ñ', 'O', 'P', 'Q', 'R', 'S', 'T', 'V', 'W', 'X', 'Y', 'Z' };

HWND winHandle = GetForegroundWindow();
Render* render = nullptr;
efef::manager* manager = nullptr;
bool first = false;

bool GetKey(int VKey)
{
	if (winHandle == GetForegroundWindow())
	{
		int pressed = GetKeyState(VKey);
		if (pressed != 0 && pressed != 1)
			return true;
	}
	return false;
}

void Start()
{
	if (efef::Init() == EFEF_ERROR)
		return;

	render->Start();

	manager = efef::manager::instance();
}

uint currentChat = 0u;
uint chatSize = 0u;
#define MAX_CHAT 10u
std::wstring chat[MAX_CHAT];

std::wstring chatMessage;

void InsertChatMessage(const std::wstring& msg)
{
	chat[currentChat++] = msg;

	if (currentChat >= MAX_CHAT)
		currentChat = 0u;

	if (chatSize < MAX_CHAT)
		++chatSize;
}

int lastInput = 0;
void TextInput(efef::fast_socket& socket)
{
	for (uint i = 0u; i < 26u; ++i)
	{
		if (GetKey(alphabet[i]))
			if (alphabet[i] != lastInput)
			{
				lastInput = alphabet[i];
				chatMessage += lastInput;
				break;
			}
	}

	if (GetKey(VK_SPACE) && VK_SPACE != lastInput)
	{
		lastInput = VK_SPACE;
		chatMessage += L' ';
	}

	if (!chatMessage.empty() && GetKey(VK_BACK) && VK_BACK != lastInput)
	{
		lastInput = VK_BACK;
		chatMessage.erase(chatMessage.end());
	}

	if (GetKey(VK_RETURN) && !chatMessage.empty())
	{
		InsertChatMessage(std::wstring(L"You: ") + chatMessage);
		chatMessage += L'\0';
		efef::istream istr;
		istr.push_var<uint>(1u);
		istr.push_var<uint>(chatMessage.size());
		istr.push_array<wchar>(const_cast<wchar*>(chatMessage.c_str()), chatMessage.size());
		socket.send(istr.get_buffer(), istr.size());
		chatMessage.clear();
	}
}

enum State
{
	CHOOSE,
	WAIT,
	DISCONNECTED
};
State state = CHOOSE;

iPoint score = { 0,0 };
std::wstring action; 
std::wstring oppAction;
std::wstring result;

byte you = '0';
byte opp = '0';

void Play()
{
	switch (opp)
	{
	case '1':
		oppAction = L"Your opponent played Rock!";
		break;
	case '2':
		oppAction = L"Your opponent played Paper!";
		break;
	case '3':
		oppAction = L"Your opponent played Scissors!";
		break;
	}

	int match = (int)you - (int)opp;

	switch (match)
	{
	case 0:
		result = L"It's a DRAW";
		break;
	case 1:
		result = L"You WIN :D";
		++score.x;
		break;
	case -1:
		result = L"You LOST :(";
		++score.y;
		break;
	case 2:
		result = L"You LOST :(";
		++score.y;
		break;
	case -2:
		result = L"You WIN :D";
		++score.x;
		break;
	}

	you = '0';
	opp = '0';
	state = CHOOSE;

	Sleep(500);
}

void Chat(efef::ostream& ostr)
{
	uint size = ostr.get_var<uint>();
	wchar* cMsg = ostr.dynm_get_array<wchar>(size);
	std::wstring str = std::wstring(L"Opp: ") + cMsg;
	InsertChatMessage(str);

	delete[] cMsg;
}

bool Update(efef::fast_socket& socket)
{
	if (GetKey('Z'))
	{
		efef::istream istr;
		istr.push_var<uint>(2u);
		socket.send(istr.get_buffer(), istr.size());
		socket.force_send();

		return false;
	}

	render->RenderText(1, L"Rock: 1 - Paper: 2 - Scissors: 3", { 0.0f, 0.0f });

	if (socket.poll(efef::select_mode::RECEIVE))
	{
		efef::set<efef::message> messages = socket.receive();
		for (uint i = 0u; i < messages.size(); ++i)
		{
			efef::message& msg = messages[i];
			if (msg.type != efef::message_type::MESSAGE)
				continue;

			efef::ostream ostr(msg.data, msg.size);
			switch (ostr.get_var<uint>())
			{
			case 0u:
				opp = ostr.get_var<byte>();
				break;
			case 1u:
				Chat(ostr);
				break;
			case 2u:
				state = DISCONNECTED;
				break;
			}
			
			msg.destroy();
		}
	}

	switch (state)
	{
	case CHOOSE:
		render->RenderText(1, L"Choose an action...", { 0.0f, 2.0f });

		if (GetKey('1'))
			you = '1';
		else if (GetKey('2'))
			you = '2';
		else if (GetKey('3'))
			you = '3';

		if (you != '0')
		{
			efef::istream istr;
			istr.push_var<uint>(0u);
			istr.push_var<byte>(you);
			socket.send(istr.get_buffer(), istr.size());

			switch (you)
			{
			case '1':
				action = L"You played Rock!";
				break;
			case '2':
				action = L"You played Paper!";
				break;
			case '3':
				action = L"You played Scissors!";
				break;
			}
			oppAction = L"";
			state = WAIT;
		}
		break;
	case WAIT:
		render->RenderText(1, L"Waiting for the opponent...", { 0.0f, 2.0f });
		if (opp != '0')
			Play();
		break;
	case DISCONNECTED:
		action = L"Opponent disconected";
		oppAction = L" ";
		result = L" ";
		for (uint i = 0u; i < chatSize; ++i)
			chat[i] = L" ";
		break;
	}

	wchar_t scoreDisplay[64];
	wsprintf(scoreDisplay, L"You -> %d - %d <- Opp", score.x, score.y);
	render->RenderText(1, scoreDisplay, { 0.0f, 3.0f });

	render->RenderText(1, action, {0.0f, 5.0f});
	render->RenderText(1, oppAction, { 0.0f, 6.0f });
	render->RenderText(1, result, { 0.0f, 8.0f });

	float firstY = 10.0f;
	for (uint i = 0u; i < chatSize; ++i)
	{
		render->RenderText(1, chat[i], {0.0f, firstY + (float)i});
	}

	if (GetKey(VK_ESCAPE))
		return false;

	return true;
}

void CleanUp()
{
	if (efef::CleanUp() == EFEF_ERROR)
		return;

	render->CleanUp();
}

int main()
{
	render = new Render(true);

	Start();

	{
		uint thisPort = 6000u;
		uint remotePort = 6001u;

		while (true)
		{
			if (GetKey(VK_SPACE))
			{
				break;
			}
			else if (GetKey(VK_CONTROL))
			{
				thisPort = 6001u;
				remotePort = 6000u;
				break;
			}
		}
		
		Sleep(1000);

		efef::socket_addr thisAddress(L"127.0.0.1", thisPort);
		efef::socket_addr remoteAddress(L"127.0.0.1", remotePort);
		efef::fast_socket socket = efef::CreateFastSocket(efef::address_family::IPv4);

		socket.bind(thisAddress);
		socket.connect(remoteAddress);

		render->RenderText(1, L"Looking for a match...", { 0.0f, 3.0f });
		render->Update(0.0f);
		{
			byte confirmation = 'A';
			while (confirmation == 'A')
			{
				if (GetKey(VK_CONTROL))
				{
					socket.send(&confirmation, 1u);
					break;
				}

				manager->update();

				if (socket.poll(efef::select_mode::RECEIVE))
				{
					efef::set<efef::message> messages = socket.receive();
					for (uint i = 0u; i < messages.size(); ++i)
					{
						efef::message& msg = messages[i];
						if (msg.type != efef::message_type::MESSAGE)
							continue;

						if (msg.data[0] == confirmation)
							confirmation = 'B';

						msg.destroy();
					}
				}
			}
		}

		while (Update(socket))
		{
			TextInput(socket);
			manager->update();
			render->Update(0.0f);
		}
	}

	CleanUp();

	return 0;
}