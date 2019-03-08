#include <windows.h>

static int g_nScreenIndex;
static HANDLE g_hScreen[2];

void ScreenInit()
{
	CONSOLE_CURSOR_INFO cci;

	// ����Ʈ ���Ϳ� �� ���͸� �����Ѵ�.
	g_hScreen[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0,
		NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	g_hScreen[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0,
		NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

	// Ŀ���� �����.
	cci.dwSize = 1;
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(g_hScreen[0], &cci);
	SetConsoleCursorInfo(g_hScreen[1], &cci);
}

void ScreenFlipping() // Ȱ��ȭ ���ۿ� ��Ȱ��ȭ ������ ���¸� �ٲ۴�.
{
	SetConsoleActiveScreenBuffer(g_hScreen[g_nScreenIndex]);
	g_nScreenIndex = !g_nScreenIndex;
}

void ScreenClear() // ȭ���� ���۸� �����.
{
	COORD Coor = { 0, 0 };
	DWORD dw;
	FillConsoleOutputCharacter(g_hScreen[g_nScreenIndex], ' ', 80 * 25, Coor, &dw);
}

void ScreenRelease() // ������ ȭ���� ���۸� ���� �����Ѵ�.
{
	CloseHandle(g_hScreen[0]);
	CloseHandle(g_hScreen[1]);
}

void ScreenPrint(int x, int y, char *string) // ������ x,t��ǥ�� ���ڿ��� ������ش�.
{
	DWORD dw;
	COORD CursorPosition = { x, y };
	SetConsoleCursorPosition(g_hScreen[g_nScreenIndex], CursorPosition);
	WriteFile(g_hScreen[g_nScreenIndex], string, strlen(string), &dw, NULL);
}