#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <string>
#pragma comment(lib,"user32.lib")
using namespace std;
#define ID_RES 1
LRESULT CALLBACK MyWndProc(HWND, UINT, WPARAM, LPARAM);
HDC hdc;
RECT rt;
string str;
bool open = false;
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd;
	MSG msg;
	WNDCLASS wc;
	LPCSTR lpszAppName = "Сканер клавиатуры";
	wc.lpszClassName = lpszAppName;
	wc.hInstance = hInstance;
	wc.lpfnWndProc = (WNDPROC)MyWndProc;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = 0;
	wc.lpszMenuName = 0;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	if (!RegisterClass(&wc))
		return 0;
	hWnd = CreateWindow(lpszAppName, lpszAppName, WS_OVERLAPPEDWINDOW, 520, 350, 440, 100, NULL, NULL, hInstance, NULL);
	if (hWnd == NULL)
		return 0;
	ShowWindow(hWnd, SW_SHOWNORMAL);
	UpdateWindow(hWnd);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK MyWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND ed;
	PAINTSTRUCT ps;
	LPCSTR buf;
	switch (msg)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	case WM_CREATE:
	{
		ed = CreateWindow("static", "", WS_CHILD | WS_VISIBLE, 5, 25, 415, 30, hWnd, (HMENU)ID_RES, NULL, NULL);
		return 0;
	}
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rt);
		DrawText(hdc, "Сканер клавиатуры", strlen("Сканер клавиатуры"), &rt, DT_CENTER);
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_KEYDOWN:
	{
		FILE* f;
		switch (wParam)
		{
		case VK_DELETE:
			str = "Delete";
			break;
		case VK_BACK:
			str = "Back";
			break;
		case VK_CONTROL:
			str = "Ctrl";
			break;
		case VK_HOME:
			str = "Home";
			break;
		case VK_NUMLOCK:
			str = "NumLock";
			break;
		case VK_END:
			str = "End";
			break;
		case VK_ESCAPE:
			str = "Esc";
			break;
		case VK_SHIFT:
			str = "Shift";
			break;
		case VK_SPACE:
			str = "Space";
			break;
		case VK_TAB:
			str = "Tab";
			break;
		case VK_CAPITAL:
			str = "CapsLock";
			break;
		case VK_RETURN:
			str = "Enter";
			break;
		case VK_PRIOR:
			str = "PgUp";
			break;
		case VK_NEXT:
			str = "PgDn";
			break;
		case VK_INSERT:
			str = "Ins";
			break;
		case VK_LWIN:
			str = "Win";
			break;


		case VK_DOWN:
			str = "Down";
			break;
		case VK_LEFT:
			str = "Left";
			break;
		case VK_RIGHT:
			str = "Right";
			break;
		case VK_UP:
			str = "Up";
			break;

		case VK_F1:
			str = "F1";
			break;
		case VK_F2:
			str = "F2";
			break;
		case VK_F3:
			str = "F3";
			break;
		case VK_F4:
			str = "F4";
			break;
		case VK_F5:
			str = "F5";
			break;
		case VK_F6:
			str = "F6";
			break;
		case VK_F7:
			str = "F7";
			break;
		case VK_F8:
			str = "F8";
			break;
		case VK_F9:
			str = "F9";
			break;
		case VK_F10:
			str = "F10";
			break;
		case VK_F11:
			str = "F11";
			break;
		case VK_F12:
			str = "F12";
			break;

		case VK_NUMPAD0:
			str = "0";
			break;
		case VK_NUMPAD1:
			str = "1";
			break;
		case VK_NUMPAD2:
			str = "2";
			break;
		case VK_NUMPAD3:
			str = "3";
			break;
		case VK_NUMPAD4:
			str = "4";
			break;
		case VK_NUMPAD5:
			str = "5";
			break;
		case VK_NUMPAD6:
			str = "6";
			break;
		case VK_NUMPAD7:
			str = "7";
			break;
		case VK_NUMPAD8:
			str = "8";
			break;
		case VK_NUMPAD9:
			str = "9";
			break;
		case VK_MULTIPLY:
			str = "*";
			break;
		case VK_ADD:
			str = "+";
			break;
		case VK_SUBTRACT:
			str = "-";
			break;
		case VK_DIVIDE:
			str = "/";
			break;
		case VK_CLEAR:
			str = "Clear";
			break;
		case VK_DECIMAL:
			str = ",";
			break;

		case VK_OEM_PLUS:
			str = "=+";
			break;
		case VK_OEM_COMMA:
			str = ",<";
			break;
		case VK_OEM_MINUS:
			str = "-_";
			break;
		case VK_OEM_1:
			str = ";:";
			break;
		case VK_OEM_2:
			str = "/?";
			break;
		case VK_OEM_3:
			str = "`~";
			break;
		case VK_OEM_4:
			str = "[{";
			break;
		case VK_OEM_5:
			str = "\\|";
			break;
		case VK_OEM_6:
			str = "]}";
			break;
		case VK_OEM_7:
			str = "'''";
			break;
		case VK_OEM_PERIOD:
			str = ".>";
			break;
			
		case VK_MENU:
			str = "Alt";
			break;
		case VK_SNAPSHOT:
			str = "PrtSc";
			break;

		default:
			str = wParam;
		}
		if (open)
			f = fopen("file_c.txt", "ab");
		else
		{
			f = fopen("file_c.txt", "wb");
			open = true;
		}
		fprintf(f, "%s\n", str.c_str());
		fclose(f);
		buf = (LPCSTR)(str.c_str());
		SetWindowText(ed, buf);
		str.clear();
		break;
	}
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	typedef struct {
		HWND hwnd;
		UINT msg;
		WPARAM wParam;
		LPARAM lParam;
		DWORD time;
		POINT pt;
	} MSG, * PMSG;
	typedef struct {
		ULONG_PTR dwData;
		DWORD cbData;
		PVOID lpData;
	} COPYDATASTRUCT, * PCOPYDATASTRUCT;
	return 0;
}