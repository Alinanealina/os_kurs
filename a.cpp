#include <windows.h>
#include <process.h>
#include <tchar.h>
#include <tlhelp32.h>
#include <psapi.h>
#pragma comment(lib,"user32.lib")
#pragma comment(lib,"Gdi32.lib")
#define ID_LB1 1
#define ID_LB2 2
#define ID_LB3 3
#define ID_PROC 4
#define ID_BTN1 10
#define ID_BTN2 20
#define ID_BTN3 30
#define ID_BTN4 40

static HWND hList1, hList2, hList3, ed;
int N = 0;
bool p = false;
char name[1024], path[MAX_PATH];
void fill_proccount();
void fill_procLB();
void fill_infomodLB(DWORD pid);
void end_proc(DWORD pid);
LRESULT CALLBACK MyWndProc(HWND, UINT, WPARAM, LPARAM);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd;
	MSG msg;
	WNDCLASS wc;
	LPCSTR lpszAppName = "Диспетчер задач";
	wc.lpszClassName = lpszAppName;
	wc.hInstance = hInstance;
	wc.lpfnWndProc = (WNDPROC)MyWndProc;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = 0;
	wc.lpszMenuName = 0;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	if (!RegisterClass(&wc))
		return 0;
	hWnd = CreateWindow(lpszAppName, lpszAppName, WS_OVERLAPPEDWINDOW, 100, 100, 750, 475, NULL, NULL, hInstance, NULL);
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
	HDC hdc;
	PAINTSTRUCT ps;
	LPCSTR buf;
	char* com[255] = { "cmd", "/C", "start", "", NULL, NULL, NULL };
	switch (msg)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	case WM_CREATE:
	{
		hList1 = CreateWindow("listbox", NULL, WS_CHILD | WS_VISIBLE | LBS_STANDARD | LBS_WANTKEYBOARDINPUT, 10, 35, 350, 310, hWnd, (HMENU)ID_LB1, NULL, NULL);
		hList2 = CreateWindow("listbox", NULL, WS_CHILD | WS_VISIBLE | LBS_WANTKEYBOARDINPUT, 370, 35, 350, 125, hWnd, (HMENU)ID_LB2, NULL, NULL);
		hList3 = CreateWindow("listbox", NULL, WS_CHILD | WS_VISIBLE | LBS_STANDARD | LBS_WANTKEYBOARDINPUT, 370, 195, 350, 155, hWnd, (HMENU)ID_LB3, NULL, NULL);
		ed = CreateWindow("static", "", WS_CHILD | WS_VISIBLE, 80, 390, 50, 20, hWnd, (HMENU)ID_PROC, NULL, NULL);
		CreateWindow("button", "Завершить процесс", WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE, 210, 355, 250, 30, hWnd, (HMENU)ID_BTN1, NULL, NULL);
		CreateWindow("button", "Просмотреть PE/ELF-файл", WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE, 470, 355, 250, 30, hWnd, (HMENU)ID_BTN2, NULL, NULL);
		CreateWindow("button", "Обновить список процессов", WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE, 210, 395, 250, 30, hWnd, (HMENU)ID_BTN3, NULL, NULL);
		CreateWindow("button", "Открыть сканер клавиатуры", WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE, 470, 395, 250, 30, hWnd, (HMENU)ID_BTN4, NULL, NULL);
		fill_procLB();
		fill_proccount();
		return 0;
	}
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		TextOut(hdc, 115, 10, "Список процессов", strlen("Список процессов"));
		TextOut(hdc, 470, 10, "Параметры процесса", strlen("Параметры процесса"));
		TextOut(hdc, 490, 165, "Список модулей", strlen("Список модулей"));
		TextOut(hdc, 30, 370, "Количество процессов:", strlen("Количество процессов:"));
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_COMMAND:
	{
		if ((HIWORD(wParam) == 0))
		{
			switch (LOWORD(wParam))
			{
			case ID_BTN1:
			{
				int i = SendMessage(hList1, LB_GETCURSEL, 0, 0);
				DWORD pid = (DWORD)SendMessage(hList1, LB_GETITEMDATA, (WPARAM)i, 0);
				if (MessageBox(hWnd, TEXT("Вы уверены, что хотите завершить данный процесс?"), TEXT("Внимание!"), MB_YESNO | MB_ICONWARNING) == IDYES)
				{
					end_proc(pid);
					Sleep(10);
				}
				else
					break;
			}
			case ID_BTN3:
			{
				N = 0;
				fill_procLB();
				fill_proccount();
				break;
			}
			case ID_BTN2:
			{
				if (p)
				{
					com[3] = "b";
					com[4] = path;
					com[5] = name;
					_spawnvp(_P_NOWAIT, com[0], com);
				}
				else
					MessageBox(hWnd, TEXT("Нельзя просмотреть данный файл."), TEXT("Ошибка"), MB_OK | MB_ICONERROR);
				break;
			}
			case ID_BTN4:
			{
				com[3] = "c";
				com[4] = NULL;
				com[5] = NULL;
				_spawnvp(_P_NOWAIT, com[0], com);
				break;
			}
			}
		}
		else if ((LOWORD(wParam) == ID_LB1) && (HIWORD(wParam) == LBN_SELCHANGE))
		{
			int i = SendMessage(hList1, LB_GETCURSEL, 0, 0);
			DWORD pid = (DWORD)SendMessage(hList1, LB_GETITEMDATA, (WPARAM)i, 0);
			fill_infomodLB(pid);
		}
		break;
	}
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

void fill_proccount()
{
	LPCSTR buf;
	char str[1024];
	itoa(N, str, 10);
	buf = (LPCSTR)(str);
	SetWindowText(ed, buf);
}

void fill_procLB()
{
	int i;
	PROCESSENTRY32 pre;
	HANDLE hsn = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	SendMessage(hList1, LB_RESETCONTENT, 0, 0);
	SendMessage(hList2, LB_RESETCONTENT, 0, 0);
	SendMessage(hList3, LB_RESETCONTENT, 0, 0);
	if (hsn == INVALID_HANDLE_VALUE)
		return;
	pre.dwSize = sizeof(PROCESSENTRY32);
	if (Process32First(hsn, &pre))
	{
		do
		{
			N++;
			i = SendMessage(hList1, LB_ADDSTRING, 0, (DWORD)((LPSTR)pre.szExeFile));
			SendMessage(hList1, LB_SETITEMDATA, i, (DWORD)pre.th32ProcessID);
		} while (Process32Next(hsn, &pre));
	}
	CloseHandle(hsn);
}

void fill_infomodLB(DWORD pid)
{
	int i;
	PROCESSENTRY32 pre;
	HANDLE hsn = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0), pHndl = OpenProcess(PROCESS_QUERY_INFORMATION, 0, pid);
	DWORD min, max;
	TCHAR sz[100];
	MODULEENTRY32 me;
	SendMessage(hList2, LB_RESETCONTENT, 0, 0);
	SendMessage(hList3, LB_RESETCONTENT, 0, 0);
	if (hsn == INVALID_HANDLE_VALUE)
		return;
	pre.dwSize = sizeof(PROCESSENTRY32);
	if (Process32First(hsn, &pre))
	{
		for (i = SendMessage(hList1, LB_FINDSTRING, 0, (LPARAM)((LPCTSTR)sz)); Process32Next(hsn, &pre); i++)
		{
			if (pre.th32ProcessID == pid)
				break;
		}
	}

	strcpy(name, pre.szExeFile);
	if (GetModuleFileNameEx(pHndl, NULL, path, MAX_PATH))
		p = true;
	else
		p = false;

	wsprintf(sz, TEXT("Имя: %s"), pre.szExeFile);
	SendMessage(hList2, LB_ADDSTRING, 0, (LPARAM)sz);
	strcpy(name, pre.szExeFile);
	wsprintf(sz, TEXT("ID процесса: %lu"), pre.th32ProcessID);
	SendMessage(hList2, LB_ADDSTRING, 0, (LPARAM)sz);
	wsprintf(sz, TEXT("Кол-во потоков: %d"), pre.cntThreads);
	SendMessage(hList2, LB_ADDSTRING, 0, (LPARAM)sz);
	wsprintf(sz, TEXT("Базовый приоритет: %d"), pre.pcPriClassBase);
	SendMessage(hList2, LB_ADDSTRING, 0, (LPARAM)sz);
	if (GetProcessWorkingSetSize(pHndl, &min, &max))
	{
		wsprintf(sz, TEXT("Мин размер физической памяти: %lu"), min);
		SendMessage(hList2, LB_ADDSTRING, 0, (LPARAM)sz);
		wsprintf(sz, TEXT("Макс размер физической памяти: %lu"), max);
		SendMessage(hList2, LB_ADDSTRING, 0, (LPARAM)sz);
	}
	CloseHandle(pHndl);

	hsn = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
	if (hsn == INVALID_HANDLE_VALUE)
		return;

	me.dwSize = sizeof(MODULEENTRY32);
	if (Module32First(hsn, &me))
	{
		SendMessage(hList3, LB_ADDSTRING, 0, (LPARAM)me.szExePath);
		while (Module32Next(hsn, &me))
			SendMessage(hList3, LB_ADDSTRING, 0, (LPARAM)me.szExePath);
		CloseHandle(hsn);
	}
}


void end_proc(DWORD pid)
{
	static HWND hwndDlg;
	HANDLE h = OpenProcess(PROCESS_TERMINATE, 0, pid);
	DWORD dw;
	if (h == INVALID_HANDLE_VALUE)
		return;
	TerminateProcess(h, 0);
	dw = WaitForSingleObject(h, 5000);
	if (dw == WAIT_TIMEOUT)
		MessageBox(hwndDlg, TEXT("Ошибка завершения процесса."), TEXT("Ошибка"), MB_OK | MB_ICONERROR);
	CloseHandle(h);
}