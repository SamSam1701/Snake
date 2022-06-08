#pragma once

#include<conio.h>
#include<Windows.h>
#include<iostream>
#include<wincon.h>
#include<string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>
using namespace std;

char MSSV[] = "1912053719120627191206631912063619120664";
char ROCK[] = "NHOM19";

#define MAX_SIZE_SNAKE 50
#define MAX_SIZE_FOOD 4
#define MAX_SPEED 3
//Biến toàn cục

POINT snake[50]; //Con rắn
POINT gate[5];
POINT rock[20]; // vị trí đầu của vật cản

POINT food[4]; // Thức ăn
POINT step[4] = { {-1,0},{1,0},{0,-1},{0,1} };// bước đi tiếp theo
int CHAR_LOCK;//Biến xác định hướng không thể di chuyển (Ở một thời điểm có một hướng rắn không thể di chuyển)
int MOVING;//Biến xác định hướng di chuyển của snake (Ở một thời điểm có ba hướng rắn có thể di chuyển)
int SPEED;// Có thể hiểu như level, level càng cao thì tốc độ càng nhanh
int HEIGH_CONSOLE, WIDTH_CONSOLE;// Độ rộng và độ cao của màn hình console
int FOOD_INDEX; // Chỉ số food hiện hành đang có trên màn hình
int SIZE_SNAKE; // Kích thước của snake, lúc đầu có 6 và tối đa lên tới 10
int STATE; // Trạng thái sống hay chết của rắn

int score, level, food_ate;


//Xử lý hành động
int Move(POINT step);
void Eat1();
void Eat2();
void effectDead();

//tạo thức ăn, tạo rắn
void GenerateFood();


//Xử lý qua màn
void GenerateGate();
//void drawGate();
void throughGate();
void clearGate();
void clearRock();
void clearSnake();
bool checkRock(int x, int y);
void createRock();

//Các hàm vẽ
void drawScore();
void drawFood();
void drawGate();
void DrawBoard(int x, int y, int width, int height);
void drawSnake();
void startGame();
void drawRock();

// một số hàm khác
void ShowCur(bool CursorVisibility);
void SetColor(WORD color);
void FixConsoleWindow();
void GotoXY(int x, int y);
void ResetData();
void NewGame();
void ExitGame();
void ThreadFunc();
void PauseGame();
bool IsValid(int x, int y);
bool IsValid2(POINT gate[]);
void startGame();


//Hàm lưu và load trò chơi
void ProcessDead();
void saveGame();
void saveScore();
int Move(POINT step);
int check();







