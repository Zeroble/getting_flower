#include "stdafx.h"
#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

#define PLAYER_X 14 //플레이어의 기본 위치
#define PLAYER_Y 8

void gotoxy(int x, int y) { //gotoxy함수
	COORD CursorPosition = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), CursorPosition);
}
void intro();//시작화면 출력 함수
void result();//결과 출력함수
void line();//맵 테두리 출력함수
void makeLaser();//레이저 생성함수
void makeFlower();//꽃 생성함수
void frame();//프레임 넘겨주는 함수

typedef enum _STATUS { //게임 진행 상태에대한 열거형
	INTRO, RUNNING, RESULT, RESTART, END
} STATUS;
STATUS status = INTRO;//상태를 인트로로 설정

typedef struct _g_Player { //플레이어 구조체
	int life = 1;
	int x = PLAYER_X, y = PLAYER_Y;
} g_Player;
g_Player player;

typedef struct _g_Laser {//레이저 구조체
	int oldframe = 0;//oldframe -> 생성된떄의 프레임 기록
	int life = 0;
	int x, y;
}g_Laser;
g_Laser laser;

typedef struct _g_Flower {//꽃의 구조체
	int x, y;
	int life = 0;
}g_Flower;
g_Flower flower;

int score = 0;
int laserCounter;
int setLevel = 20; //난이도를 설정해주는 변수
long long allframe = 0;//총 지나간 프레임을 기록하는 변수
clock_t CurTime, OldTime = 0;

int main()
{
	system("mode con lines=18 cols=35");//콘솔창의 크기 변경
	while (1)
	{
		char nKey;
		if (status == END){//게임 끝
			system("cls");
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
			gotoxy(12,5);
			printf("GAME OVER");
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			printf("\n");
			break;
		}
		if (_kbhit())
		{
			nKey = _getch();
			switch (nKey)
			{
			case '4': //왼쪽
				if (player.x != 4) player.x = player.x - 2;
				break;

			case '8': //위
				if (player.y != 2) player.y--;
				break;

			case '5':  //아래
				if (player.y != 14) player.y++;
				break;

			case '6':  //오른쪽
				if (player.x != 30) player.x = player.x + 2;
				break;
			}
		}
		switch (status)
		{
		case INTRO://인트로
			intro();
			status = RUNNING;
			break;

		case RESTART://초기화
			laser.life = 0;

			player.life = 1; //플레이어 초기화
			player.x = PLAYER_X;
			player.y = PLAYER_Y;

			flower.life = 0; //꽃 초기화

			score = 0;//점수 초기화
			status = RUNNING;

			allframe = 0;//지나간 총 프레임수 초기화
			break;

		case RUNNING:
			srand(time(NULL));

			if (player.x == flower.x&&player.y == flower.y) {//꽃 먹는판정
				score++;
				flower.life = 0;
			}
			if (flower.life == 0) makeFlower();//꽃이 없을시 새로운 꽃 생성

			if (laser.life == 1)//레이저가 살아있을때
			{
				if (laser.oldframe + setLevel < allframe) {//경고시간이 끝났는지 확인

					if (laser.oldframe + setLevel + 7 < allframe) laser.life = 0;//끝났을때 7프레임동안 레이저 발사
					else if (laserCounter == 0) {//0일때 x축이 랜덤
						for (int i = 1; i <= 15; i++) {
							gotoxy(laser.x, i);
							printf("□");
						}
						if (player.x == laser.x) status = RESULT;//플레이어와의 충돌 판정
					}
					else {//1일떄 y축이 랜덤
						for (int i = 3; i <= 31; i++, i++) {
							gotoxy(i, laser.y);
							printf("□");
						}
						if (player.y == laser.y) status = RESULT;//플레이어와의 충돌 판정
					}
				}
			}
			else if (allframe > 75) makeLaser(); //laser.life = 0이고 75프레임이상 지나갔을때 레이저 생성

			system("cls");//출력부
			if (laser.life == 1) {//레이저 출력부
				gotoxy(laser.x, laser.y);
				printf("□");
			}
			gotoxy(player.x, player.y);//플레이어 출력부
			printf("■");
			gotoxy(flower.x, flower.y);//꽃 출력부
			printf("★", flower.x, flower.y);
			gotoxy(12, 17);//점수 출력부
			printf("score : %3d", score);
			line();//맵 경계 그려주는 함수
			break;

		case RESULT:
			system("cls");
			result();//결과창 출력 함수
			break;
		}

		frame();//프레임 지나가게해주는 함수 (33밀리 세컨드에 한번)
		allframe++;
	}
	_getch();
	return 0;
}

void intro() {//인트로
	char nKey;
	gotoxy(9, 5);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
	printf("<GETTING FLOWERS>");
	gotoxy(8, 7);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
	printf("MOVE KEY : 8 4 5 6");
	gotoxy(6, 9);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
	printf("EASY 4 NORMAL 5 HARD 6");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	gotoxy(4, 12);
	printf("PRESS 4 or 5 or 6 TO START");
	while (1) {
		nKey = _getch();
		switch (nKey)
		{
		case '4':
			setLevel = 25;
			return;
		case '5':
			setLevel = 18;
			return;
		case '6':
			setLevel = 12;
			return;
		}
	}
}
void result() {//결과
	char nKey;
	gotoxy(11, 5);
	printf("score : %3d", score);
	gotoxy(11, 11);
	printf("retry? (Y/N)");
	while (1) {
		nKey = _getch();
		switch (nKey)
		{
		case 'y':
		case 'Y':
			status = RESTART;
			return;
		case 'n':
		case 'N':
			status = END;
			return;
		}
	}
}
void frame()//프레임 넘김
{
	while (1)
	{
		CurTime = clock();
		if (CurTime - OldTime > 33) {
			OldTime = CurTime;
			break;
		}
	}
}
void line() {//선긋기
	gotoxy(3, 1);
	printf("-----------------------------");
	gotoxy(3, 15);
	printf("-----------------------------");
	for (int i = 1; i <= 15; i++) {
		gotoxy(2, i);
		printf("|");
		gotoxy(32, i);
		printf("|");
	}
}
void makeFlower() {//꽃 생성
	flower.x = (rand() % 14 + 2) * 2;
	flower.y = (rand() % 7 + 1) * 2;
	flower.life = 1;
}
void makeLaser() {//레이저 생성
	if (laser.life == 0 && laserCounter == 0) {
		laserCounter++;
		laser.x = 33;
		laser.y = player.y;
		laser.life = 1;
	}
	else if (laser.life == 0 && laserCounter == 1) {
		laserCounter = 0;
		laser.x = player.x;
		laser.y = 0;
		laser.life = 1;
	}
	laser.oldframe = allframe;
}

