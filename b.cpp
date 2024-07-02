#include <windows.h>
#include <stdio.h>
#include <process.h>
#pragma comment(lib,"user32.lib")
int main(int argc, char* argv[])
{
	if (argc != 3)
		return 1;
	char filename[1024] = " ", * com[255] = { "cmd", "/C", "start", "", NULL };
	FILE* f, * a;
	int i, filesize;
	PBYTE pSrcFile;
	HANDLE hSrcFile, hMapSrcFile;

	for (i = 0; i < strlen(argv[2]) - 4; i++)
		filename[i] = argv[2][i];
	strcat(filename, ".txt");
	a = fopen(argv[1], "r");
	if (!a)
	{
		MessageBox(NULL, TEXT("Нельзя просмотреть данный файл."), TEXT("Ошибка"), MB_OK | MB_ICONERROR);
		return 1;
	}

	if (fseek(a, 0L, SEEK_END))
		return 1;
	filesize = ftell(a);
	if (filesize == -1L)
		return 1;
	fclose(a);

	hSrcFile = CreateFile(argv[1], GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	hMapSrcFile = CreateFileMapping(hSrcFile, NULL, PAGE_READONLY, 0, 0, NULL);
	pSrcFile = (PBYTE)MapViewOfFile(hMapSrcFile, FILE_MAP_READ, 0, 0, 0);
	if ((hSrcFile == 0) || (hSrcFile == INVALID_HANDLE_VALUE) || (hMapSrcFile == NULL) || (pSrcFile == NULL))
		return 1;
	f = fopen(filename, "w");
	if (!f)
		return 1;
	for (i = 0; i < filesize; i++)
	{
		fprintf(f, "%2X ", pSrcFile[i]);
		if ((i + 1) % 16 == 0)
			fprintf(f, "\n");
	}
	fclose(f);
	com[3] = filename;
	_spawnvp(_P_NOWAIT, com[0], com);
	return 0;
}