#include "stdafx.h"
#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

#define PLAYER_X 14 //�÷��̾��� �⺻ ��ġ
#define PLAYER_Y 8

void gotoxy(int x, int y) { //gotoxy�Լ�
	COORD CursorPosition = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), CursorPosition);
}
void intro();//����ȭ�� ��� �Լ�
void result();//��� ����Լ�
void line();//�� �׵θ� ����Լ�
void makeLaser();//������ �����Լ�
void makeFlower();//�� �����Լ�
void frame();//������ �Ѱ��ִ� �Լ�

typedef enum _STATUS { //���� ���� ���¿����� ������
	INTRO, RUNNING, RESULT, RESTART, END
} STATUS;
STATUS status = INTRO;//���¸� ��Ʈ�η� ����

typedef struct _g_Player { //�÷��̾� ����ü
	int life = 1;
	int x = PLAYER_X, y = PLAYER_Y;
} g_Player;
g_Player player;

typedef struct _g_Laser {//������ ����ü
	int oldframe = 0;//oldframe -> �����ȋ��� ������ ���
	int life = 0;
	int x, y;
}g_Laser;
g_Laser laser;

typedef struct _g_Flower {//���� ����ü
	int x, y;
	int life = 0;
}g_Flower;
g_Flower flower;

int score = 0;
int laserCounter;
int setLevel = 20; //���̵��� �������ִ� ����
long long allframe = 0;//�� ������ �������� ����ϴ� ����
clock_t CurTime, OldTime = 0;

int main()
{
	system("mode con lines=18 cols=35");//�ܼ�â�� ũ�� ����
	while (1)
	{
		char nKey;
		if (status == END){//���� ��
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
			case '4': //����
				if (player.x != 4) player.x = player.x - 2;
				break;

			case '8': //��
				if (player.y != 2) player.y--;
				break;

			case '5':  //�Ʒ�
				if (player.y != 14) player.y++;
				break;

			case '6':  //������
				if (player.x != 30) player.x = player.x + 2;
				break;
			}
		}
		switch (status)
		{
		case INTRO://��Ʈ��
			intro();
			status = RUNNING;
			break;

		case RESTART://�ʱ�ȭ
			laser.life = 0;

			player.life = 1; //�÷��̾� �ʱ�ȭ
			player.x = PLAYER_X;
			player.y = PLAYER_Y;

			flower.life = 0; //�� �ʱ�ȭ

			score = 0;//���� �ʱ�ȭ
			status = RUNNING;

			allframe = 0;//������ �� �����Ӽ� �ʱ�ȭ
			break;

		case RUNNING:
			srand(time(NULL));

			if (player.x == flower.x&&player.y == flower.y) {//�� �Դ�����
				score++;
				flower.life = 0;
			}
			if (flower.life == 0) makeFlower();//���� ������ ���ο� �� ����

			if (laser.life == 1)//�������� ���������
			{
				if (laser.oldframe + setLevel < allframe) {//���ð��� �������� Ȯ��

					if (laser.oldframe + setLevel + 7 < allframe) laser.life = 0;//�������� 7�����ӵ��� ������ �߻�
					else if (laserCounter == 0) {//0�϶� x���� ����
						for (int i = 1; i <= 15; i++) {
							gotoxy(laser.x, i);
							printf("��");
						}
						if (player.x == laser.x) status = RESULT;//�÷��̾���� �浹 ����
					}
					else {//1�ϋ� y���� ����
						for (int i = 3; i <= 31; i++, i++) {
							gotoxy(i, laser.y);
							printf("��");
						}
						if (player.y == laser.y) status = RESULT;//�÷��̾���� �浹 ����
					}
				}
			}
			else if (allframe > 75) makeLaser(); //laser.life = 0�̰� 75�������̻� ���������� ������ ����

			system("cls");//��º�
			if (laser.life == 1) {//������ ��º�
				gotoxy(laser.x, laser.y);
				printf("��");
			}
			gotoxy(player.x, player.y);//�÷��̾� ��º�
			printf("��");
			gotoxy(flower.x, flower.y);//�� ��º�
			printf("��", flower.x, flower.y);
			gotoxy(12, 17);//���� ��º�
			printf("score : %3d", score);
			line();//�� ��� �׷��ִ� �Լ�
			break;

		case RESULT:
			system("cls");
			result();//���â ��� �Լ�
			break;
		}

		frame();//������ �����������ִ� �Լ� (33�и� �����忡 �ѹ�)
		allframe++;
	}
	_getch();
	return 0;
}

void intro() {//��Ʈ��
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
void result() {//���
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
void frame()//������ �ѱ�
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
void line() {//���߱�
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
void makeFlower() {//�� ����
	flower.x = (rand() % 14 + 2) * 2;
	flower.y = (rand() % 7 + 1) * 2;
	flower.life = 1;
}
void makeLaser() {//������ ����
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

