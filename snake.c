#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h> // _kbhit() �� _getch()�� ����ϱ� ���� ��ó����.
#include <time.h> // clock()�� clock_t�� ����ϱ� ���� ��ó����.
#include <memory.h>
#include "Screen.h"

#define MAP_COL 29 // �� ���μ�ġ�� �ִ�ġ
#define MAP_ROW 22 // �� ���μ�ġ�� �ִ�ġ

typedef enum _STATE { INTRO, READY, RUN, CLEARED, FAILED, STOP, RESULT } STATE;
typedef enum _DIRECT { LT, RT, U, D } DIRECT;  // ���Ⱚ�� �������� �̿�. ������������ ����.
											   // _DIRECT �������� DIRECT�� ��ü.

typedef struct _SnakePos // _SnakePos ����ü ����.
{
	int X, Y; // ���� ��ǥ��.
	int OX, OY; // ���� ��ǥ��.
} SnakePos;// _SnakePosition ����ü�� POSITION���� ��ü.

typedef struct _STAGE_INFO // �������� ���� ����ü.
{
	int EnemyC;
	int EnemyCP;
	clock_t LTime;
	int EatC;
	int nMap[MAP_ROW][MAP_COL];

} STAGE_INFO;// _STAGE_INFO ����ü�� STAGE_INFO���� ��ü.

typedef struct _SnakeInfo // _SnakeInfo ����ü ����.
{
	int Life;
	DIRECT SnakeDR;
	clock_t MTime;
	clock_t OTime;
	SnakePos SnakeHD; // SnakePos ����ü�� ��Ҹ� SnakeHD�� ����.
	SnakePos SnakeTL[20];

} SnakeInfo; // _SnakeBody ����ü�� BODY�� ��ü.

typedef struct _EnemyInfo
{
	int Life;
	int X, Y;
	DIRECT EnemyDR;
	clock_t MTime;
	clock_t OTime;
} EnemyInfo;

SnakeInfo Snake;
EnemyInfo *Enemy = NULL;
STAGE_INFO Stage;

int EatC;
clock_t LTime;
STATE State = INTRO;
int StageNum = 0;
int Score = 0;
int TotalS = 0;
int EnemyC = 0;
clock_t StartT;
clock_t UpdateOT;
clock_t RemainT;

char StageIcon[5][4] = { "��", "��", "��", "��", "��" }; // �ʿ� ����� �������� ��. ������� 0,1,2,3,4

int Stage1[MAP_ROW][MAP_COL] =
{
	{ 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1 },
	{ 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1 },
	{ 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1 },
	{ 1, -1, -1,  2, -1, -1, -1, -1, -1, -1, -1,  2, -1, -1, -1,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1 },
	{ 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1, -1, -1, -1, -1, -1,  2, -1, -1, -1, -1, -1, -1,  1 },
	{ 1, -1, -1, -1, -1,  2, -1, -1,  0,  1,  1,  1,  1,  1,  1,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1 },
	{ 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1 },
	{ 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1, -1,  0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1 },
	{ 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  2, -1, -1, -1,  1 },
	{ 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1 },
	{ 1,  1,  1,  1,  1,  1,  1,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1 },
	{ 1, -1, -1, -1, -1, -1, -1,  1, -1, -1, -1, -1, -1,  3, -1, -1, -1, -1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1 },
	{ 1, -1, -1, -1, -1, -1, -1,  1, -1, -1,  2, -1, -1, -1, -1, -1, -1, -1,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1 },
	{ 1, -1, -1,  2, -1, -1, -1,  1, -1, -1, -1, -1,  1,  1,  1, -1, -1, -1,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1 },
	{ 1, -1, -1, -1, -1, -1, -1,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1, -1, -1, -1, -1,  2, -1, -1, -1, -1,  1 },
	{ 1, -1, -1, -1, -1, -1, -1,  1, -1, -1, -1, -1, -1, -1,  0, -1, -1, -1,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1 },
	{ 1, -1, -1, -1, -1, -1, -1,  1, -1,  0, -1, -1, -1, -1, -1, -1, -1, -1,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1 },
	{ 1, -1, -1, -1, -1, -1, -1,  1, -1, -1, -1, -1, -1,  1, -1, -1, -1, -1, -1, -1, -1,  0, -1, -1, -1, -1, -1, -1,  1 },
	{ 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1 },
	{ 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1, -1, -1, -1,  2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1 },
	{ 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1 },
	{ 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1 },
}; // ���� ����.

int Stage2[MAP_ROW][MAP_COL] =
{
	{ 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1 },
	{ 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1, -1,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1 },
	{ 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1 },
	{ 1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  2, -1, -1,  1, -1,  0, -1, -1, -1, -1, -1, -1,  3, -1, -1, -1, -1, -1,  1 },
	{ 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1 },
	{ 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1, -1, -1, -1, -1, -1, -1,  1,  1,  1,  1,  1,  1,  1,  1,  1 },
	{ 1, -1, -1, -1,  1, -1, -1, -1, -1,  1,  1,  1,  1,  1, -1, -1, -1, -1, -1, -1,  1, -1, -1, -1, -1, -1, -1, -1,  1 },
	{ 1, -1, -1, -1,  1,  0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  2, -1, -1,  1, -1, -1, -1, -1, -1, -1, -1,  1 },
	{ 1, -1, -1, -1,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1, -1, -1, -1, -1, -1, -1, -1,  1 },
	{ 1, -1,  2, -1,  1, -1, -1, -1, -1,  2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1, -1, -1, -1,  1, -1, -1, -1,  1 },
	{ 1, -1, -1, -1,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1, -1, -1, -1,  1, -1, -1, -1,  1 },
	{ 1,  1,  1,  1,  1,  1,  1,  1, -1, -1, -1,  0, -1, -1, -1,  0, -1, -1, -1, -1,  1, -1, -1, -1,  1, -1, -1, -1,  1 },
	{ 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1, -1,  2, -1,  1 },
	{ 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1, -1, -1, -1,  1 },
	{ 1, -1, -1,  0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1, -1, -1, -1,  1 },
	{ 1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1,  1,  1,  1,  1,  1, -1, -1, -1, -1, -1, -1, -1, -1,  1,  1,  1,  1,  1 },
	{ 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1, -1, -1,  2, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1 },
	{ 1, -1, -1, -1, -1,  1, -1, -1,  0, -1, -1, -1, -1, -1, -1,  1, -1, -1, -1, -1, -1, -1, -1,  0, -1, -1, -1, -1,  1 },
	{ 1, -1, -1, -1, -1,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  2, -1, -1,  1 },
	{ 1, -1,  2, -1, -1,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1,  0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1 },
	{ 1, -1, -1, -1, -1,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1 },
	{ 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1 },
}; // ���� ����.

void IntroScreen()
{
	ScreenPrint(0, 0, "������������������������������������������������������������������������������������������������������������������������");
	ScreenPrint(0, 1, "�������������������������������᦭");
	ScreenPrint(0, 2, "���ᡡ�����������������������������������������������������᦭");
	ScreenPrint(0, 3, "���ᡡ�����������������������������������������������������᦭");
	ScreenPrint(0, 4, "���ᡡ���������ݡ������ᡡ�����������������������������᦭");
	ScreenPrint(0, 5, "���ᡡ������������ᡡ�������������������������������������᦭");
	ScreenPrint(0, 6, "���ᡡ���������ᡡ�� ������������������������ �������������᦭");
	ScreenPrint(0, 7, "���ᡡ�������ᡡ�� ���ڡ� Snake    �ڦ��������� ���������᦭");
	ScreenPrint(0, 8, "���ᡡ�������������� �������������������� �����������������᦭");
	ScreenPrint(0, 9, "���ᡡ���ݡ������������� /)/)�������� �������������᦭");
	ScreenPrint(0, 10,"���ᡡ����������������(*'')/�������������������������������᦭");
	ScreenPrint(0, 11,"���ᡡ���������������������������������������ݡ������������᦭");
	ScreenPrint(0, 12,"���ᡡ�����������������������������������������������������᦭");
	ScreenPrint(0, 13,"���ᡡ��������ᡡ�������������������������������������᦭");
	ScreenPrint(0, 14,"���ᡡ�������������ᡡ���������� �ܡߡ�   �����������������᦭");
	ScreenPrint(0, 15,"���ᡡ�������ݡ����ᡡ�������������� �� �������������������᦭");
	ScreenPrint(0, 16,"���ᡡ�������������ᡡPress Space Key! �ߡߡߡߡߡ��� �����᦭");
	ScreenPrint(0, 17,"���ᡡ�������������ᡡ�������������������������������������᦭");
	ScreenPrint(0, 18,"���ᡡ�����������������ᡡ�������������ᡡ���������᦭");
	ScreenPrint(0, 19,"���ᡡ���������������������������������ᡡ�����������������᦭");
	ScreenPrint(0, 20,"���ᡡ���������������������������������ᡡ�������ݡ��������᦭");
	ScreenPrint(0, 21,"���ᡡ���������������������������������ᡡ�����������������᦭");
	ScreenPrint(0, 22,"�������������������������������᦭");
	ScreenPrint(0, 23,"������������������������������������������������������������������������������������������������������������������������");
}

void ReadyScreen()
{
	ScreenPrint(0, 0, "������������������������������������������������������������������������������������������������������������������������");
	ScreenPrint(0, 1, "�������������������������������᦭");
	ScreenPrint(0, 2, "���ᡡ�����������������������������������������������������᦭");
	ScreenPrint(0, 3, "���ᡡ�����������������������������������������������������᦭");
	ScreenPrint(0, 4, "���ᡡ�����������������������������������������������������᦭");
	ScreenPrint(0, 5, "���ᡡ�����������������������������������������������������᦭");
	ScreenPrint(0, 6, "���ᡡ�����������������������������������������������������᦭");
	ScreenPrint(0, 7, "���ᡡ�����������������������������������������������������᦭");
	ScreenPrint(0, 8, "���ᡡ�����������������������������������������������������᦭");
	ScreenPrint(0, 9, "���ᡡ�����������������������������������������������������᦭");
	ScreenPrint(0, 10,"���ᡡ�����������������������������������������������������᦭");
	ScreenPrint(0, 11,"���ᡡ�����������������������������������������������������᦭");
	ScreenPrint(0, 12,"���ᡡ�����������������������������������������������������᦭");
	ScreenPrint(0, 13,"���ᡡ�����������������������������������������������������᦭");
	ScreenPrint(0, 14,"���ᡡ������������������Are You Ready?���������������������᦭");
	ScreenPrint(0, 15,"���ᡡ�����������������������������������������������������᦭");
	ScreenPrint(0, 16,"���ᡡ�����������������������������������������������������᦭");
	ScreenPrint(0, 17,"���ᡡ�����������������������������������������������������᦭");
	ScreenPrint(0, 18,"���ᡡ�����������������������������������������������������᦭");
	ScreenPrint(0, 19,"���ᡡ�����������������������������������������������������᦭");
	ScreenPrint(0, 20,"���ᡡ�����������������������������������������������������᦭");
	ScreenPrint(0, 21,"���ᡡ�����������������������������������������������������᦭");
	ScreenPrint(0, 22,"�������������������������������᦭");
	ScreenPrint(0, 23,"������������������������������������������������������������������������������������������������������������������������");
}

void ClearedScreen()
{
	ScreenPrint(0, 0, "������������������������������������������������������������������������������������������������������������������������");
	ScreenPrint(0, 1, "�������������������������������᦭");
	ScreenPrint(0, 2, "���ᡡ�����������������������������������������������������᦭");
	ScreenPrint(0, 3, "���ᡡ�����������������������������������������������������᦭");
	ScreenPrint(0, 4, "���ᡡ�����������������������������������������������������᦭");
	ScreenPrint(0, 5, "���ᡡ�����������������������������������������������������᦭");
	ScreenPrint(0, 6, "���ᡡ�����������������������������������������������������᦭");
	ScreenPrint(0, 7, "���ᡡ�����������������������������������������������������᦭");
	ScreenPrint(0, 8, "���ᡡ����������������Congratulation!!!������ �������������᦭");
	ScreenPrint(0, 9, "���ᡡ�����������������������������������������������������᦭");
	ScreenPrint(0, 10,"���ᡡ�����������������������������������������������������᦭");
	ScreenPrint(0, 11,"���ᡡ�����������������������������������������������������᦭");
	ScreenPrint(0, 12,"���ᡡ�����������������������������������������������������᦭");
	ScreenPrint(0, 13,"���ᡡ�����������������������������������������������������᦭");
	ScreenPrint(0, 14,"���ᡡ�����������������������������������������������������᦭");
	ScreenPrint(0, 15,"���ᡡ�����������������������������������������������������᦭");
	ScreenPrint(0, 16,"���ᡡ�����������������������������������������������������᦭");
	ScreenPrint(0, 17,"���ᡡ�����������������������������������������������������᦭");
	ScreenPrint(0, 18,"���ᡡ�����������������������������������������������������᦭");
	ScreenPrint(0, 19,"���ᡡ�����������������������������������������������������᦭");
	ScreenPrint(0, 20,"���ᡡ�����������������������������������������������������᦭");
	ScreenPrint(0, 21,"���ᡡ�����������������������������������������������������᦭");
	ScreenPrint(0, 22,"�������������������������������᦭");
	ScreenPrint(0, 23,"������������������������������������������������������������������������������������������������������������������������");
}

void FailureScreen()
{
	ScreenPrint(0, 0, "������������������������������������������������������������������������������������������������������������������������");
	ScreenPrint(0, 1, "�������������������������������᦭");
	ScreenPrint(0, 2, "���ᡡ�����������������������������������������������������᦭");
	ScreenPrint(0, 3, "���ᡡ�����������������������������������������������������᦭");
	ScreenPrint(0, 4, "���ᡡ�����������������������������������������������������᦭");
	ScreenPrint(0, 5, "���ᡡ�����������������������������������������������������᦭");
	ScreenPrint(0, 6, "���ᡡ�����������������������������������������������������᦭");
	ScreenPrint(0, 7, "���ᡡ�����������������������������������������������������᦭");
	ScreenPrint(0, 8, "���ᡡ����������������You Failed....   ������ �������������᦭");
	ScreenPrint(0, 9, "���ᡡ�����������������������������������������������������᦭");
	ScreenPrint(0, 10,"���ᡡ�����������������������������������������������������᦭");
	ScreenPrint(0, 11,"���ᡡ�����������������������������������������������������᦭");
	ScreenPrint(0, 12,"���ᡡ�����������������������������������������������������᦭");
	ScreenPrint(0, 13,"���ᡡ�����������������������������������������������������᦭");
	ScreenPrint(0, 14,"���ᡡ�����������������������������������������������������᦭");
	ScreenPrint(0, 15,"���ᡡ�����������������������������������������������������᦭");
	ScreenPrint(0, 16,"���ᡡ�����������������������������������������������������᦭");
	ScreenPrint(0, 17,"���ᡡ�����������������������������������������������������᦭");
	ScreenPrint(0, 18,"���ᡡ�����������������������������������������������������᦭");
	ScreenPrint(0, 19,"���ᡡ�����������������������������������������������������᦭");
	ScreenPrint(0, 20,"���ᡡ�����������������������������������������������������᦭");
	ScreenPrint(0, 21,"���ᡡ�����������������������������������������������������᦭");
	ScreenPrint(0, 22,"�������������������������������᦭");
	ScreenPrint(0, 23,"������������������������������������������������������������������������������������������������������������������������");
}

void ResultScreen()
{
	ScreenPrint(0, 0, "������������������������������������������������������������������������������������������������������������������������");
	ScreenPrint(0, 1, "��������������������������������������������������������������");
	ScreenPrint(0, 2, "��������������������������������������������������������������");
	ScreenPrint(0, 3, "��������������������������������������������������������������");
	ScreenPrint(0, 4, "��������������������������������������������������������������");
	ScreenPrint(0, 5, "��������������������������������������������������������������");
	ScreenPrint(0, 6, "��������������������������������������������������������������");
	ScreenPrint(0, 7, "��������������������������������������������������������������");
	ScreenPrint(0, 8, "��������������������������������������������������������������");
	ScreenPrint(0, 9, "��������������������������������������������������������������");
	ScreenPrint(0, 10,"��������������������������������������������������������������");
	ScreenPrint(0, 11,"��������������������������������������������������������������");
	ScreenPrint(0, 12,"��������������������������������������������������������������");
	ScreenPrint(0, 13,"��������������������������������������������������������������");
	ScreenPrint(0, 14,"��������������������������������������������������������������");
	ScreenPrint(0, 15,"��������������������������������������������������������������");
	ScreenPrint(0, 16,"��������������������������������������������������������������");
	ScreenPrint(0, 17,"��������������������������������������������������������������");
	ScreenPrint(0, 18,"��������������������������������������������������������������");
	ScreenPrint(0, 19,"��������������������������������������������������������������");
	ScreenPrint(0, 20,"��������������������������������������������������������������");
	ScreenPrint(0, 21,"��������������������������������������������������������������");
	ScreenPrint(0, 22,"��������������������������������������������������������������");
	ScreenPrint(0, 23,"������������������������������������������������������������������������������������������������������������������������");
}

void Stageopen()
{
	FILE *fp;

	if (StageNum == 0)
	{
		fp = fopen("C:\\Users\\HakureY\\source\\repos\\SnakeGameReBuild\\map.dat", "wb");

		fwrite(Stage1, sizeof(int), 638, fp);
		fclose(fp);

		Stage.EnemyC = 5;
		Stage.EnemyCP = 5;
		Snake.MTime = 100;
		Snake.OTime = clock();
		Snake.SnakeDR = LT;
		Snake.Life = 4;
		Stage.LTime = 100000;
		Stage.EatC = 9;
		EatC = 0;
		Score = 0;
	}

	if (StageNum == 1)
	{
		fp = fopen("C:\\Users\\HakureY\\source\\repos\\SnakeGameReBuild\\map.dat", "wb");

		fwrite(Stage2, sizeof(int), 638, fp);
		fclose(fp);

		Stage.EnemyC = 8;
		Stage.EnemyCP = 8;
		Snake.MTime = 100;
		Snake.OTime = clock();
		Snake.SnakeDR = LT;
		Snake.Life = 4;
		Stage.LTime = 90000;
		Stage.EatC = 10;
		EatC = 0;
		Score = 0;
	}

	fp = fopen("C:\\Users\\HakureY\\source\\repos\\SnakeGameReBuild\\map.dat", "rb");

	fread(Stage.nMap, sizeof(int), 638, fp);
	fclose(fp);
}

void SnakeMovement()
{
	int i;

	Snake.SnakeHD.OX = Snake.SnakeHD.X;
	Snake.SnakeHD.OY = Snake.SnakeHD.Y;

	for (i = 0;i < Snake.Life - 1;i++)
	{
		Snake.SnakeTL[i].OX = Snake.SnakeTL[i].X;
		Snake.SnakeTL[i].OY = Snake.SnakeTL[i].Y;
	}

	Snake.SnakeTL[0].X = Snake.SnakeHD.OX;
	Snake.SnakeTL[0].Y = Snake.SnakeHD.OY;

	for (i = 1;i < Snake.Life - 1;i++)
	{
		Snake.SnakeTL[i].X = Snake.SnakeTL[i-1].OX;
		Snake.SnakeTL[i].Y = Snake.SnakeTL[i-1].OY;
	}
}

void StageInfo()
{
	int i, j, count;
	
	Stageopen();

	if (Enemy != NULL)
		free(Enemy);

	Enemy = (EnemyInfo*)malloc(sizeof(EnemyInfo) * Stage.EnemyC);
	srand((unsigned int)time(NULL));

	count = 0;

	for (i = 0;i < MAP_ROW;i++)
	{
		for (j = 0;j < MAP_COL;j++)
		{
			if (Stage.nMap[i][j] == 3)
			{
				Snake.SnakeHD.X = j;
				Snake.SnakeHD.Y = i;
				Stage.nMap[i][j] = -1;
			}

			if (Stage.nMap[i][j] == 0)
			{
				Enemy[count].Life = 1;
				Enemy[count].X = j;
				Enemy[count].Y = i;
				Enemy[count].MTime = 100;
				Enemy[count].OTime = clock();
				Enemy[count].EnemyDR = rand() % 4;

				Stage.nMap[i][j] = -1;
				count++;
			}
		}
	}

	Snake.SnakeHD.OX = Snake.SnakeHD.X;
	Snake.SnakeHD.OY = Snake.SnakeHD.Y;

	for (i = 0;i < 3;i++)
	{
		Snake.SnakeTL[i].X = Snake.SnakeHD.X + i + 1;
		Snake.SnakeTL[i].Y = Snake.SnakeHD.Y;
	}
}

int EnemySmash(int X, int Y)
{
	int i;

	for (i = 0;i < Stage.EnemyC;i++)
	{
		if (Enemy[i].Life == 1)
		{
			if (Enemy[i].X == X && Enemy[i].Y == Y)
			{
				Enemy[i].Life = 0;
				return 1;
			}
		}
	}

	return 0;
}

void StateCheck()
{
	int i, j;
	clock_t CurTime = clock();
	DIRECT DR;

	switch (State)
	{
	case READY:
		if (CurTime - UpdateOT > 3000)
		{
			State = RUN;
			StartT = CurTime;
		}
		break;

	case RUN:

		RemainT = (Stage.LTime - (CurTime - StartT)) / 1000;

		if ((CurTime - StartT) > Stage.LTime || EatC == Stage.EatC)
		{
			State = STOP;
			return;
		}

		if (CurTime - Snake.OTime > Snake.MTime)
		{
			Snake.OTime = CurTime;

			switch (Snake.SnakeDR)
			{
			case LT:
				if (Snake.SnakeHD.X - 1 >= 0)
				{
					if (Stage.nMap[Snake.SnakeHD.Y][Snake.SnakeHD.X - 2] == 1)
					{
						srand((unsigned int)time(NULL));

						while (1)
						{
							DR = rand() % 4;

							if (DR != LT)
							{
								break;
							}
						}

						Snake.SnakeDR = DR;
					}

					if (Stage.nMap[Snake.SnakeHD.Y][Snake.SnakeHD.X - 1] == 2)
					{
						Snake.Life++;
						Stage.nMap[Snake.SnakeHD.Y][Snake.SnakeHD.X - 1] = -1;
						EatC++;
						Score += 100;
					}

					if (EnemySmash(Snake.SnakeHD.X - 1, Snake.SnakeHD.Y) == 1)
					{
						Snake.Life--;
						Stage.EnemyC--;

						if (Snake.Life < 1)
						{
							State = STOP;
							return;
						}
					}

					if (Snake.Life > 1)
					{
						SnakeMovement();
					}

					Snake.SnakeHD.X--;
				}
				break;

			case RT:
				if (Snake.SnakeHD.X + 1 < MAP_COL)
				{
					if (Stage.nMap[Snake.SnakeHD.Y][Snake.SnakeHD.X + 2] == 1)
					{
						srand((unsigned int)time(NULL));

						while (1)
						{
							DR = rand() % 4;

							if (DR != RT)
							{
								break;
							}
						}

						Snake.SnakeDR = DR;
					}

					if (Stage.nMap[Snake.SnakeHD.Y][Snake.SnakeHD.X + 1] == 2)
					{
						Snake.Life++;
						Stage.nMap[Snake.SnakeHD.Y][Snake.SnakeHD.X + 1] = -1;
						EatC++;
						Score += 100;
					}

					if (EnemySmash(Snake.SnakeHD.X + 1, Snake.SnakeHD.Y) == 1)
					{
						Snake.Life--;
						Stage.EnemyC--;

						if (Snake.Life < 1)
						{
							State = STOP;
							return;
						}
					}

					if (Snake.Life > 1)
					{
						SnakeMovement();
					}

					Snake.SnakeHD.X++;
				}
				break;

			case U:
				if (Snake.SnakeHD.Y - 1 >= 0)
				{
					if (Stage.nMap[Snake.SnakeHD.Y-2][Snake.SnakeHD.X] == 1)
					{
						srand((unsigned int)time(NULL));

						while (1)
						{
							DR = rand() % 4;

							if (DR != U)
							{
								break;
							}
						}

						Snake.SnakeDR = DR;
					}

					if (Stage.nMap[Snake.SnakeHD.Y - 1][Snake.SnakeHD.X] == 2)
					{
						Snake.Life++;
						Stage.nMap[Snake.SnakeHD.Y - 1][Snake.SnakeHD.X] = -1;
						EatC++;
						Score += 100;
					}

					if (EnemySmash(Snake.SnakeHD.X, Snake.SnakeHD.Y - 1) == 1)
					{
						Snake.Life--;
						Stage.EnemyC--;

						if (Snake.Life < 1)
						{
							State = STOP;
							return;
						}
					}

					if (Snake.Life > 1)
					{
						SnakeMovement();
					}

					Snake.SnakeHD.Y--;
				}
				break;

			case D:
				if (Snake.SnakeHD.Y + 1 < MAP_ROW)
				{
					if (Stage.nMap[Snake.SnakeHD.Y + 2][Snake.SnakeHD.X] == 1)
					{
						srand((unsigned int)time(NULL));

						while (1)
						{
							DR = rand() % 4;

							if (DR != D)
							{
								break;
							}
						}

						Snake.SnakeDR = DR;
					}

					if (Stage.nMap[Snake.SnakeHD.Y + 1][Snake.SnakeHD.X] == 2)
					{
						Snake.Life++;
						Stage.nMap[Snake.SnakeHD.Y + 1][Snake.SnakeHD.X] = -1;
						EatC++;
						Score += 100;
					}

					if (EnemySmash(Snake.SnakeHD.X, Snake.SnakeHD.Y + 1) == 1)
					{
						Snake.Life--;
						Stage.EnemyC--;

						if (Snake.Life < 1)
						{
							State = STOP;
							return;
						}
					}

					if (Snake.Life > 1)
					{
						SnakeMovement();
					}

					Snake.SnakeHD.Y++;
				}
				break;
			}

			for (i = 0;i < Stage.EnemyCP;i++)
			{
				if (Enemy[i].Life == 1)
				{
					if (CurTime - Enemy[i].OTime > Enemy[i].MTime)
					{
						Enemy[i].OTime = CurTime;

						switch (Enemy[i].EnemyDR)
						{
						case LT:
							if (Enemy[i].X - 1 >= 0)
							{
								if (Stage.nMap[Enemy[i].Y][Enemy[i].X - 1] == 1)
								{
									srand((unsigned int)time(NULL));

									while (1)
									{
										DR = rand() % 4;

										if (DR != LT)
										{
											break;
										}
									}

									Enemy[i].EnemyDR = DR;
								}

								else
								{
									Enemy[i].X--;
								}
							}
							break;

						case RT:
							if (Enemy[i].X + 1 < MAP_COL)
							{
								if (Stage.nMap[Enemy[i].Y][Enemy[i].X - 1] == 1)
								{
									srand((unsigned int)time(NULL));

									while (1)
									{
										DR = rand() % 4;

										if (DR != RT)
										{
											break;
										}
									}

									Enemy[i].EnemyDR = DR;
								}

								else
								{
									Enemy[i].X++;
								}
							}
							break;

						case U:
							if (Enemy[i].Y - 1 >= 0)
							{
								if (Stage.nMap[Enemy[i].Y][Enemy[i].X - 1] == 1)
								{
									srand((unsigned int)time(NULL));

									while (1)
									{
										DR = rand() % 4;

										if (DR != U)
										{
											break;
										}
									}

									Enemy[i].EnemyDR = DR;
								}

								else
								{
									Enemy[i].Y--;
								}
							}
							break;

						case D:
							if (Enemy[i].Y + 1 < MAP_ROW)
							{
								if (Stage.nMap[Enemy[i].Y][Enemy[i].X - 1] == 1)
								{
									srand((unsigned int)time(NULL));

									while (1)
									{
										DR = rand() % 4;

										if (DR != D)
										{
											break;
										}
									}

									Enemy[i].EnemyDR = DR;
								}

								else
								{
									Enemy[i].Y++;
								}
							}
							break;
						}
					}

					if (Snake.SnakeHD.X == Enemy[i].X && Snake.SnakeHD.Y == Enemy[i].Y)
					{
						Enemy[i].Life = 0;
						Snake.Life--;
						Stage.EnemyC--;

						if (Snake.Life < 1)
						{
							State = STOP;
							return;
						}
					}

					for (j = 0;j < Snake.Life - 1;j++)
					{
						if (Enemy[i].X == Snake.SnakeTL[j].X && Enemy[i].Y == Snake.SnakeTL[j].Y)
						{
							Enemy[i].Life = 0;
							Snake.Life--;
							Stage.EnemyC--;
						}
					}
				}
			}
			break;

			case STOP:
				if (EatC >= Stage.EatC)
				{
					UpdateOT = CurTime;
					State = CLEARED;
					TotalS += Score;
				}

				else
				{
					State = FAILED;
				}
				break;

			case CLEARED:
				if (CurTime - UpdateOT > 3000)
				{
					UpdateOT = CurTime;
					++StageNum;
					StageInfo();
					State = READY;
				}
				break;
		}
 	}

}

void GamePlay()
{
	int i, j;
	char string[100];

	ScreenInit();
	ScreenClear();

	switch (State)
	{
	case INTRO:
		if (StageNum == 0)
			IntroScreen();
		break;

	case READY:
		ReadyScreen();
		sprintf(string, "%d Stage", StageNum + 1);
		ScreenPrint(27, 9, string);
		break;

	case RUN:
		for (i = 0;i < MAP_ROW;i++)
		{
			for (j = 0;j < MAP_COL;j++)
			{
				if (Stage.nMap[i][j] == -1)
				{
					ScreenPrint((j * 2), i, "��");
				}

				else
				{
					ScreenPrint((j * 2), i, StageIcon[Stage.nMap[i][j]]);
				}
			}
			ScreenPrint(Snake.SnakeHD.X * 2, Snake.SnakeHD.Y, StageIcon[3]);
		}

		for (i = 0;i < Stage.EnemyCP;i++)
		{
			if (Enemy[i].Life == 1)
			{
				ScreenPrint(Enemy[i].X * 2, Enemy[i].Y, StageIcon[0]);
			}
		}

		for (i = 0;i < Snake.Life - 1;i++)
		{
			ScreenPrint(Snake.SnakeTL[i].X * 2, Snake.SnakeTL[i].Y, StageIcon[4]);
		}

		sprintf(string, "%d Stage", StageNum + 1);
		ScreenPrint(61, 5, string);
		sprintf(string, "�����ð�: %d�� %d��", RemainT / 60, RemainT %60);
		ScreenPrint(61, 7, string);
		sprintf(string, "���� ȹ�� ��: %d", EatC);
		ScreenPrint(61, 9, string);
		sprintf(string, "����: %d", Score);
		ScreenPrint(61, 11, string);
		sprintf(string, "�� ��: %d", Stage.EnemyC);
		ScreenPrint(61, 13, string);

		break;

	case CLEARED:
		ClearedScreen();
		sprintf(string, "Stage %d Clear!!", StageNum + 1);
		ScreenPrint(19, 7, string);
		sprintf(string, "Total Score : %d", TotalS);
		ScreenPrint(18, 17, string);
		break;

	case FAILED:
		FailureScreen();
		sprintf(string, "Game Over");
		ScreenPrint(19, 7, string);
		sprintf(string, "ReTry? (Y/N)");
		ScreenPrint(18, 17, string);
		break;

	case RESULT:
		ResultScreen();
		sprintf(string, "Total Score : %d", TotalS);
		ScreenPrint(20, 8, string);
		sprintf(string, "Thank you for playing!!");
		ScreenPrint(20, 16, string);
		break;
	}

	ScreenFlipping();
}

void MemoryFree()
{
	if (Enemy != NULL)
		free(Enemy);
}

int main()
{
	int Key;
	clock_t CurTime;

	ScreenInit();
	StageInfo();

	while (1)
	{
		CurTime = clock();

		if (_kbhit())
		{
			if (State == RESULT)
			{
				break;
			}

			Key = _getch();

			switch(Key)
			{
			case ' ':
				if (State == INTRO && StageNum == 0)
				{
					State = READY;
				}
				break;

			case 75:
				SnakeMovement();
				Snake.SnakeDR = LT;
				break;

			case 77:
				SnakeMovement();
				Snake.SnakeDR = RT;
				break;

			case 72:
				SnakeMovement();
				Snake.SnakeDR = U;
				break;

			case 80:
				SnakeMovement();
				Snake.SnakeDR = D;
				break;

			case 'y':
			case 'Y':
				if (State == FAILED)
				{
					StageInfo();
					State = READY;
					UpdateOT = clock();
				}
				break;
				
			case 'n':
			case 'N':
				if (State == FAILED)
				{
					State = RESULT;
				}
				break;
			}
		}
		StateCheck();
		GamePlay();
	}

	MemoryFree();
	ScreenRelease();
	return 0;
}