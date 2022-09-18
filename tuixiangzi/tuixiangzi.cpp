#include <stdio.h> 
#include <string>
#include <conio.h>
#include <windows.h>
#include <fstream>
using namespace std;
#define MAX 99
bool check_of_air(int x0[], int y0[], int x, int y) {//尾气处理函数，试试看嗷
	int i;
	int length = 20;
	for (i = 0; i < length; i++) {					
		if (x0[i] == x) {
			if (y0[i] == y) {
				return(1);
			}
		}
	}
	return(0);
}
void unfinished();
void ingame(int x, string location);
void gamesmeau();//游戏选择菜单
void sokuban(); //推箱子
void inplay(int a);//一级关卡选择（是否进入关卡）
void win(int x);//胜利判定， 现期决定一步一判  
bool wcompare(int mount,int xd[], int yd[],int map01[][50]) ;//x，y是需要比较的区域大小，xd【】yd【】存入目的地坐标x，y值
void snakeEX();
void gotoxy(int x, int y) {           //光标移动:修改输出字符的在屏幕上的位置  (参考:)  此函数及代码串来自于网络以及DBM_PIGEON的支持           
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);            //1.找到控制台的这个窗口
	COORD coord;                                                //2.光标的结构体
	coord.X = x; coord.Y = y;                                   //3.设置光标的坐标 
	SetConsoleCursorPosition(handle, coord);                    //4.设置控制台光标位置                              
}//基础函数 用于输出 定义画布
void level(int x) {
	inplay(x);
}
void Levelselectmeau(int x) {
	system("CLS");
	int meaudirection = 1, meaukey;
	int xpt = 24, ypt = 7, count = 1, num = 1 + 8*(x-1);
	gotoxy(1, 1); printf("返回请按esc");
	gotoxy(34, 21); printf("按'/'键打开输入面板输入具体关卡");
	while (count <= 8) {												//制作菜单 
		gotoxy(24, ypt);
		printf("Level %d", num);
		count++;
		num++;
		gotoxy(64, ypt);
		printf("Level %d", num);
		ypt = ypt + 3;
		count++;
		num++;													//我是傻逼，真的 （待优化） 
	}
	gotoxy(10, 19);
	printf("上一页（q）");
	gotoxy(74, 19);
	printf("下一页（e）");
	while (1) {														 //制作选择按钮 
		switch (meaudirection) {
		case 1:
			gotoxy(22, 7);printf("→");
			gotoxy(22, 10);printf("  ");
			gotoxy(62, 7);printf("  ");//其实只有可能是从旁边几个点位过来，所以说其实可以小优化一下
			meaukey = _getch();
			if (meaukey == 13) {
				level(1 + 8 * (x - 1));
			}
			break;
		case 2:
			gotoxy(22, 7);printf("  ");
			gotoxy(62, 7);printf("→");
			gotoxy(62, 10);printf("  ");
			meaukey = _getch();
			if (meaukey == 13) {
				level(2 + 8 * (x - 1));
			}
			break;
		case 3:
			gotoxy(22, 7);printf("  ");
			gotoxy(22, 10);printf("→");
			gotoxy(22, 13);printf("  ");
			gotoxy(62, 10);printf("  ");
			meaukey = _getch();
			if (meaukey == 13) {
				level(3 + 8 * (x - 1));
			}
			break;
		case 4:
			gotoxy(22, 10);printf("  ");
			gotoxy(62, 7);printf("  ");
			gotoxy(62, 10);printf("→");
			gotoxy(62, 13);printf("  ");
			meaukey = _getch();
			if (meaukey == 13) {
				level(4 + 8 * (x - 1));
			}
			break;
		case 5:
			gotoxy(22, 10);printf("  ");
			gotoxy(22, 13);printf("→");
			gotoxy(22, 16);printf("  ");
			gotoxy(62, 13);printf("  ");
			meaukey = _getch();
			if (meaukey == 13) {
				level(5 + 8 * (x - 1));
			}
			break;
		case 6:
			gotoxy(22, 13);printf("  ");
			gotoxy(62, 10);printf("  ");
			gotoxy(62, 13);printf("→");
			gotoxy(62, 16);printf("  ");
			meaukey = _getch();
			if (meaukey == 13) {
				level(6 + 8 * (x - 1));
			}
			break;
		case 7:
			gotoxy(22, 13);printf("  ");
			gotoxy(22, 16);printf("→");
			gotoxy(62, 16);printf("  ");
			meaukey = _getch();
			if (meaukey == 13) {
				level(7 + 8 * (x - 1));
			}
			break;
		case 8:
			gotoxy(22, 16);printf("  ");
			gotoxy(62, 13);printf("  ");
			gotoxy(62, 16);printf("→");
			meaukey = _getch();
			if (meaukey == 13) {
				level(8 + 8 * (x - 1));
			}
			break;

		}
		switch (meaukey) {
		case 's':
		case 'S': 
			if (meaudirection != 7 && meaudirection != 8) {
				meaudirection += 2;
			}
				break;
		case 'w':
		case 'W': 
			if (meaudirection != 1 && meaudirection != 2) {
				meaudirection -= 2;
			}
			break;
		case 'a':
		case 'A':
			if (meaudirection % 2 == 1) { break; }
			else if (meaudirection % 2 == 0) { meaudirection--; break; }
			break;
		case 'd':
		case 'D':
			if (meaudirection % 2 == 1) { meaudirection++; break; }
			else if (meaudirection % 2 == 0) { break; }
			break;
		case 'e':
		case 'E':
			x++;
			if (x == MAX) {
				x--;
			}
			Levelselectmeau(x);
		case 'q':
		case 'Q':
			x--;
			if (x == 0) {
				x++;
			}
			Levelselectmeau(x);
		case 27:
			sokuban();
		case'/':
			gotoxy(80, 10); printf("关卡:");
			gotoxy(86, 10);
			int levelget=1;
			int levelchange;//加一个存一下，避免第二次没法手动选择关卡
			levelchange = levelget;
			level(levelchange);
		}
	}
}

void inplay(int a) { // 在data文件中 0=墙 1=可行 2=箱子 3=空白（墙外地区） 4=目的地（在打印输出后会被重置为2） 5=人位置（在打印输出后会被重置为2）
	system("CLS");
	switch (a) {
		while (1) {
			case 1: {
				string location = ".\\data\\level01\\data.dat";
				ingame(1, location);
			}
			case 2: {
				string location = ".\\data\\level02\\data.txt";
				ingame(2, location);
			}
			case 3: {
				string location = ".\\data\\level03\\data.txt";
				ingame(3, location);
			}
			case 4: {
				string location = ".\\data\\level04\\data.txt";
				ingame(4, location);
			}
			case 5: {
				string location = ".\\data\\level05\\data.txt";
				ingame(5, location);
			}
			case 6: {
				string location = ".\\data\\level06\\data.txt";
				ingame(6, location);
			}
			case 7: {
				string location = ".\\data\\level07\\data.txt";
				ingame(7, location);
			}
			case 8: {
				string location = ".\\data\\level08\\data.txt";
				ingame(8, location);
			}
			case 9: {
				string location = ".\\data\\level09\\data.txt";
				ingame(9, location);
			}
			case 10: {
				string location = ".\\data\\level10\\data.txt";
				ingame(10, location);
			}
			case 11: {
				string location = ".\\data\\level11\\data.txt";
				ingame(11, location);
			}
			case 12: {
				string location = ".\\data\\level12\\data.txt";
				ingame(12, location);
			}
			case 13: {
				string location = ".\\data\\level13\\data.txt";
				ingame(13, location);
			}
			case 14: {
				string location = ".\\data\\level14\\data.txt";
				ingame(14, location);
			}
			case 15:{
				string location = ".\\data\\level15\\data.txt";
				ingame(15, location);
			}
			case 16: {
				string location = ".\\data\\level16\\data.txt";
				ingame(16, location);
			}
			default: {
				unfinished();
			}
		}
	}
}
void sokuban() {         //执行02                    //主菜单 
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);    //隐藏光标	
	CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(hOut, &cci);
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(hOut, &cci);
	system("CLS");
	int meaudirection = 1, meaukey;                    //菜单选择和接收按键 meaudirection算法来自DBM_PIGEON大佬
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 02);
	gotoxy(34, 5);
	printf("           _          _                 ");
	gotoxy(34, 6);
	printf(" ___  ___ | | ___   _| |__   __ _ _ __  ");
	gotoxy(34, 7);
	printf("/ __|/ _ \\| |/ / | | | '_ \\ / _` | '_ \\ ");
	gotoxy(34, 8);
	printf("\\__ \\ (_) |  < | |_| | |_) | (_| | | | |");
	gotoxy(34, 9);
	printf("|___/\\___/|_|\\_\\\\__,_|_.__/ \\__,_|_| |_|");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 07);
	gotoxy(60, 11);
	printf("---by shinianyue");
	gotoxy(50, 14);
	printf("start");
	gotoxy(50, 17);
	printf("exit");
	gotoxy(32, 30);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 04);
	printf("如无特殊说明，使用'W A S D'控制'enter'键选择");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 07);
	while (1) {
		switch (meaudirection) {
		case 1:
			gotoxy(48, 14);
			printf("→");
			gotoxy(48, 17);
			printf("  ");
			meaukey = _getch();
			if (meaukey == 13)
				Levelselectmeau(1);
			break;
		case 2:
			gotoxy(48, 17);
			printf("→");
			gotoxy(48, 14);
			printf("  ");
			meaukey = _getch();
			if (meaukey == 13)
				gamesmeau();
			break;
		}
		switch (meaukey) {
		case 's':
		case 'S':
			if (meaudirection == 1)
			{
				meaudirection = 2; break;
			}
			if (meaudirection == 2)
			{
				meaudirection = 2; break;
			}
			break;
		case 'w':
		case 'W':
			if (meaudirection == 1)
			{
				meaudirection = 1; break;
			}
			if (meaudirection == 2)
			{
				meaudirection = 1; break;//此meaudirction算法来源于DBM，感谢大佬的源码分享--shinianyue
			}
			break;
		case 27:
			gamesmeau();
			break;
		}
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------

void gamesmeau() {
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(hOut, &cci);
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(hOut, &cci);
	system("CLS");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 02);
	gotoxy(20, 2);
	printf("   _________    _____   ____     _____   ____ _____   __ __  ");
	gotoxy(20, 3);
	printf("  / ___\\__  \\  /     \\_/ __ \\   /     \\_/  __ \\__  \\ |  |  \\ ");	
	gotoxy(20, 4);
	printf(" / /_/  > __ \\|  | |  \\  ___/  |  | |  \\  ___/ / __ \\|  |  / ");
	gotoxy(20, 5);
	printf(" \\___  (____  /__|_|  /\\___  > |__|_|  /\\___  >____  /____/  ");	
	gotoxy(20, 6);
	printf("/_____/     \\/      \\/     \\/        \\/     \\/     \\/        ");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 04);
	gotoxy(30, 10); printf("provided by shinianyue  &  D_B_M pigeon");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 07);
	gotoxy(24, 12); printf("Press 'A' & 'D' to choose");
	gotoxy(24, 13); printf("Press 'enter' to select");
	gotoxy(30,8); printf("Sokoban");
	gotoxy(60,8); printf("Snakes");
	gotoxy(27,8); printf("→");
	char keys;
	int direction = 1;
	while (1) {
		keys = _getch();
		switch (keys) {
			case 'a':
			case 'A': {
				if (direction == 1) {
					break;
				}
				direction--;
				gotoxy(27,8); printf("→");
				gotoxy(57, 8); printf("  ");
				break;
			}
			case 'd':
			case 'D': {
				if (direction == 2) {
					break;
				}
				direction++;
				gotoxy(57, 8); printf("→");
				gotoxy(27, 8); printf("  ");
				break;
			}
			case 13: {
				if (direction == 1) {
					sokuban();
				}
				if (direction == 2) {
					snakeEX();
				}
			}
			case 27: {
				exit(0);
			}
			default: {
				break;
			}

			}
		}
	}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------
void win(int x) {
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(hOut, &cci);
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(hOut, &cci);
	system("CLS");//祖传清屏
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
	gotoxy(15, 10); printf("You successed");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 02);
	gotoxy(15, 11); printf("Press 'Q' to get back to levelselect meau.");
	gotoxy(15, 12); printf("Press 'E' to continue (to the next level).");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 07);
	int key;
	while (1) {
		key = _getch();
		switch (key) {
		case 'q':
		case 'Q': {
			Levelselectmeau(1);
		}
		case 'e':
		case 'E': {
			level(x + 1);
		}
		default:key = _getch();
		}
	}
}
bool wcompare(int mount,int xd[], int yd[],int map01[][50]) {
	int m1 = 0 ;
	for (m1 = 0; m1 <= mount-1; m1++) {
		if (map01[yd[m1]][xd[m1]] == 2) {
			continue;
		}
		else {
			return 0;
		}
		
	}
	return 1;
}

int main() {//执行01
	gamesmeau();
	return(0);
}
void snakeEX(){
	unfinished();
}
void ingame(int x,string location) {//机密机密 这是核心代码 (😀）
	{
		gotoxy(30, 20); printf("返回请按“esc”");
		gotoxy(30, 21); printf("重开请按 R 键");
		ifstream infile;
		infile.open(location, ios::in);
		if (!infile) {
			unfinished();
		}
		char data;
		int map[50][50] = { 0 };
		int mapsizex = 0, mapsizey = 0;
		int x1 = 0, y1 = 0, keyrc, x2 = 0, y2 = 0, x3 = 0, y3 = 0;//x1和y1是打印位置，x2，y2是数组内部储存位置,x3 y3是转存用数据，用于将人的位置坐标信息带出循环
		while ((data = infile.get()) != EOF)
		{
			if (data == 32)//woc 伏笔了，关卡数据我写的是倒序的！！！！
			{
				y2 = 0;
				x2++;
				mapsizex++;
			}
			else {
				map[y2][x2] = data - 48;
				y2++;
				mapsizey++;
			}
		} //先读取 录入数组
		infile.close();
		mapsizex++;
		mapsizey = mapsizey / mapsizex;
		x2 = 0; y2 = 0;
		int dest_x[20] = { 0 }, dest_y[20] = { 0 }, di = 0;//记录目的地x，y和一个计数量
		int i = 1;
		while (1)
		{
			if (y2 == mapsizey) {
				if (x2 == mapsizex - 1) {
					break;
				}												// 其实十年月没有在写程序，他在写大雾作业
				x2++;
				y2 = 0;
				continue;
			}
			y1 = y2 + 2;
			x1 = 2 * x2 + 1;
			if (map[y2][x2] == 0) {
				gotoxy(x1, y1); printf("□");
			}
			if (map[y2][x2] == 2) {
				gotoxy(x1, y1); printf("■");
			}
			if (map[y2][x2] == 4) {
				gotoxy(x1, y1); printf("田");
				map[y2][x2] = 1;
				dest_x[di] = x2;
				dest_y[di] = y2;
				di++;
			}
			if (map[y2][x2] == 5) {
				gotoxy(x1, y1); printf("人");
				x3 = x2;
				y3 = y2;
			}
			if (map[y2][x2] == 6) {//初始时 箱子已在一个目的地的情况
				gotoxy(x1, y1); printf("■");
				map[y2][x2] = 2;
				dest_x[di] = x2;
				dest_y[di] = y2;
				di++;
			}
			y2++;
		}//输出图像
		 //以下为基本移动判定
		x2 = x3; y2 = y3;
		x1 = 2 * x2 + 1; y1 = y2 + 2;
		map[y2][x2] = 1;
		keyrc = _getch();
		while (1) {
			switch (keyrc) {
			case 'w':
			case 'W': {
				y2--;
				y1--;
				if (y2 < 1 || x2 < 1) {//老是爆黄色，加上算了
					exit(0);
				}
				if (map[y2][x2] == 0) {//前面是不是墙？
					y2++;
					y1++;
				}
				if (map[y2][x2] == 2 && map[y2 - 1][x2] == 0) {//箱子加墙
					y2++;
					y1++;
				}
				if (map[y2][x2] == 2 && map[y2 - 1][x2] == 2) {//箱子加箱子
					y2++;
					y1++;
				}
				if (map[y2][x2] == 2 && map[y2 - 1][x2] == 1) {//箱子加空白
					map[y2 - 1][x2] = 2;
					gotoxy(x1, y1 - 1); printf("■");
				}
				map[y2][x2] = 1;
				if (map[y2 + 1][x2] == 1) {//我愿称其为尾气处理
					gotoxy(x1, y1 + 1); printf("  ");
				}
				if (check_of_air(dest_x, dest_y, x2, y2 + 1))
				{
					gotoxy(x1, y1 + 1); printf("田");
				}
				gotoxy(x1, y1); printf("人");
				gotoxy(40, 20);
				if (wcompare(di, dest_x, dest_y, map)) {
					win(x);
				}
				keyrc = _getch();
				break;
			}
			case 's':
			case 'S': {
				y2++;
				y1++;
				if (map[y2][x2] == 0) {//前面是不是墙？
					y2--;
					y1--;
				}
				else if (map[y2][x2] == 2 && map[y2 + 1][x2] == 0) {//箱子加墙
					y2--;
					y1--;
				}
				else if (map[y2][x2] == 2 && map[y2 + 1][x2] == 2) {//箱子加箱子
					y2--;
					y1--;
				}
				else if (map[y2][x2] == 2 && map[y2 + 1][x2] == 1) {//箱子加空白
					map[y2 + 1][x2] = 2;
					gotoxy(x1, y1 + 1); printf("■");
				}
				map[y2][x2] = 1;
				if (map[y2 - 1][x2] == 1) {//我愿称其为尾气处理
					gotoxy(x1, y1 - 1); printf("  ");
				}
				if (check_of_air(dest_x, dest_y, x2, y2 - 1))
				{
					gotoxy(x1, y1 - 1); printf("田");
				}
				gotoxy(x1, y1); printf("人");
				gotoxy(40, 20);
				if (wcompare(di, dest_x, dest_y, map)) {
					win(x);
				}
				keyrc = _getch();
				break;
			}
			case 'd':
			case 'D': {
				x1 = x1 + 2;
				x2++;
				if (map[y2][x2] == 0) {
					x1 = x1 - 2;
					x2--;
				}
				if (map[y2][x2] == 2 && map[y2][x2 + 1] == 0) {
					x1 = x1 - 2;
					x2--;
				}
				if (map[y2][x2] == 2 && map[y2][x2 + 1] == 2) {
					x1 = x1 - 2;
					x2--;
				}
				if (map[y2][x2] == 2 && map[y2][x2 + 1] == 1) {
					gotoxy(x1 + 2, y1); printf("■");
					map[y2][x2 + 1] = 2;
				}
				map[y2][x2] = 1;
				if (map[y2][x2 - 1] == 1) {
					gotoxy(x1 - 2, y1); printf("  ");
				}
				if (check_of_air(dest_x, dest_y, x2 - 1, y2))
				{
					gotoxy(x1 - 2, y1); printf("田");
				}
				gotoxy(x1, y1); printf("人");
				gotoxy(40, 20);
				if (wcompare(di, dest_x, dest_y, map)) {
					win(x);
				}
				keyrc = _getch();
				break;
			}
			case 'a':
			case 'A': {
				x1 = x1 - 2;
				x2--;
				if (map[y2][x2] == 0) {
					x1 = x1 + 2;
					x2++;
				}
				if (map[y2][x2] == 2 && map[y2][x2 - 1] == 0) {
					x1 = x1 + 2;
					x2++;
				}
				if (map[y2][x2] == 2 && map[y2][x2 - 1] == 2) {
					x1 = x1 + 2;
					x2++;
				}
				if (map[y2][x2] == 2 && map[y2][x2 - 1] == 1) {
					gotoxy(x1 - 2, y1); printf("■");
					map[y2][x2 - 1] = 2;
				}
				map[y2][x2] = 1;
				if (map[y2][x2 + 1] == 1) {
					gotoxy(x1 + 2, y1); printf("  ");
				}
				if (check_of_air(dest_x, dest_y, x2 + 1, y2))
				{
					gotoxy(x1 + 2, y1); printf("田");
				}
				gotoxy(x1, y1); printf("人");
				gotoxy(40, 20);//统一加入这一行以防万一
				if (wcompare(di, dest_x, dest_y, map)) {
					win(x);
				}
				keyrc = _getch();
				break;
			}
			case 27:
				Levelselectmeau(1);
			case 'r':
			case 'R': {
				level(x);
			}
			default: {
				keyrc = _getch();
				break;
			}
			}
		}
	}
}
void unfinished() {
	system("cls");
	gotoxy(1, 1);
	printf("unfinished\n");
	gotoxy(1, 2);
	printf("未完成或者地图信息不存在（bushi");
	_getch();
	main();
}