#include <iostream>
#include "Chess.h"
#include<mmsystem.h>
#include<math.h>
#pragma comment(lib,"winmm.lib")
#include<conio.h>
#include <fstream>
#include <chrono>
#include <cstdlib>
#include <windows.h>
#include"AI.h"

using namespace std::chrono;
//实现棋子背景透明
void putimagePNG(int x, int y, IMAGE* picture) //x为载入图片的X坐标，y为Y坐标
{
	// 变量初始化
	DWORD* dst = GetImageBuffer();    // GetImageBuffer()函数，用于获取绘图设备的显存指针，EASYX自带
	DWORD* draw = GetImageBuffer();
	DWORD* src = GetImageBuffer(picture); //获取picture的显存指针
	int picture_width = picture->getwidth(); //获取picture的宽度，EASYX自带
	int picture_height = picture->getheight(); //获取picture的高度，EASYX自带
	int graphWidth = getwidth();       //获取绘图区的宽度，EASYX自带
	int graphHeight = getheight();     //获取绘图区的高度，EASYX自带
	int dstX = 0;    //在显存里像素的角标

	// 实现透明贴图 公式： Cp=αp*FP+(1-αp)*BP ， 贝叶斯定理来进行点颜色的概率计算
	for (int iy = 0; iy < picture_height; iy++)
	{
		for (int ix = 0; ix < picture_width; ix++)
		{
			int srcX = ix + iy * picture_width; //在显存里像素的角标
			int sa = ((src[srcX] & 0xff000000) >> 24); //0xAArrggbb;AA是透明度
			int sr = ((src[srcX] & 0xff0000) >> 16); //获取RGB里的R
			int sg = ((src[srcX] & 0xff00) >> 8);   //G
			int sb = src[srcX] & 0xff;              //B
			if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
			{
				dstX = (ix + x) + (iy + y) * graphWidth; //在显存里像素的角标
				int dr = ((dst[dstX] & 0xff0000) >> 16);
				int dg = ((dst[dstX] & 0xff00) >> 8);
				int db = dst[dstX] & 0xff;
				draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)  //公式： Cp=αp*FP+(1-αp)*BP  ； αp=sa/255 , FP=sr , BP=dr
					| ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)         //αp=sa/255 , FP=sg , BP=dg
					| (sb * sa / 255 + db * (255 - sa) / 255);              //αp=sa/255 , FP=sb , BP=db
			}
		}
	}
}

Chess::Chess(int grideSize, int marginX, int marginY, float chessSize)
{
	this->grideSize = grideSize;
	this->margin_x = marginX;
	this->margin_y = marginY;
	this->chessSize = chessSize;
	playerFlag = CHESS_BLACK;

	for (int i = 0;i < grideSize;i++)
	{
		vector<int> row;
		for (int j = 0;j < grideSize;j++)
		{
			row.push_back(0);
		}
		chessMap.push_back(row);
	}
}

void Chess::Start_The_Game()
{
	initgraph(908, 936, EW_SHOWCONSOLE);
	loadimage(0, "res/棋盘11.png");
	_getch();//暂停，按下任意键退出
}

void Chess::init()
{
	//创建游戏窗口
	initgraph(908, 936, EW_SHOWCONSOLE);
	//显示我们的棋盘图片
	loadimage(0, "res/棋盘.jpg");
	//播放开始音乐
	mciSendString("play res/start.wav",0,0,0);
	//加载黑棋和白棋的图片
	if (!chessLoaded) {
		loadChessImages();
		chessLoaded = true; // 标志变量设置为已加载  
	}
	//棋盘清零
	for (int i = 0; i < grideSize; i++)
	{
		for (int j = 0; j < grideSize; j++)
		{
			chessMap[i][j] = 0;
		}
	}
}

void Chess::loadChessImages()
{
		char a;
		cout << "请选择棋局皮肤(A:经典  B:原神  C:狐妖小红娘  D:一人之下)" << endl;
		cin >> a;

		switch (a)
		{
		case 'A':
			loadimage(&chessBlackImg, "res/black.png", chessSize, chessSize, true);
			loadimage(&chessWhiteImg, "res/white.png", chessSize, chessSize, true);
			break;
		case 'B'://原神
			ischess = 1;
			loadimage(0, "res/棋盘1.png");
			loadimage(&chessBlackImg, "res/black1.png", chessSize, chessSize, true);
			loadimage(&chessWhiteImg, "res/white1.png", chessSize, chessSize, true);
			break;
		case 'C'://狐妖
			ischess = 2;
			loadimage(0, "res/棋盘3.png");
			loadimage(&chessBlackImg, "res/black3.png", chessSize, chessSize, true);
			loadimage(&chessWhiteImg, "res/white3.png", chessSize, chessSize, true);
			break;
		case 'D'://一人之下
			ischess = 3;
			loadimage(0, "res/棋盘4.png");
			loadimage(&chessBlackImg, "res/black4.png", chessSize, chessSize, true);
			loadimage(&chessWhiteImg, "res/white4.png", chessSize, chessSize, true);
			break;
		default:
			cout << "无效的选择，请重新选择" << endl;
			break;
		}
	}


bool Chess::clickBoard(int x, int y, ChessPos* pos) {
	int col = (x - margin_x) / chessSize; // 计算点击位置所在的列数  
	int row = (y - margin_y) / chessSize; // 计算点击位置所在的行数  
	int leftTopPosX = margin_x + chessSize * col; // 格子左上角的 x 坐标  
	int leftTopPosY = margin_y + chessSize * row; // 格子左上角的 y 坐标  
	int offset = chessSize * 0.4; // 鼠标点击的模糊距离上限  
	bool set = false;
	do {
		int len = calculateDistance(x, y, leftTopPosX, leftTopPosY); // 计算点击位置与格子左上角的距离  
		if (len < offset) {
			pos->row = row; // 将行数保存到 pos 中  
			pos->col = col; // 将列数保存到 pos 中  
			if (chessMap[pos->row][pos->col] == 0) { // 如果该位置为空，可以设置  
				set = true; // 设置 set 为 true  
			}
			break; // 跳出循环  
		}
		len = calculateDistance(x, y, leftTopPosX + chessSize, leftTopPosY); // 计算点击位置与格子右上角的距离  
		if (len < offset) {
			pos->row = row; // 将行数保存到 pos 中  
			pos->col = col + 1; // 将列数保存到 pos 中  
			if (chessMap[pos->row][pos->col] == 0) { // 如果该位置为空，可以设置  
				set = true; // 设置 set 为 true  
			}
			break; // 跳出循环  
		}
		len = calculateDistance(x, y, leftTopPosX, leftTopPosY + chessSize); // 计算点击位置与格子左下角的距离  
		if (len < offset) {
			pos->row = row + 1; // 将行数保存到 pos 中  
			pos->col = col; // 将列数保存到 pos 中  
			if (chessMap[pos->row][pos->col] == 0) { // 如果该位置为空，可以设置  
				set = true; // 设置 set 为 true  
			}
			break; // 跳出循环  
		}
		len = calculateDistance(x, y, leftTopPosX - chessSize, leftTopPosY + chessSize); // 计算点击位置与格子右下角的距离  
		if (len < offset) {
			pos->row = row + 1; // 将行数保存到 pos 中  
			pos->col = col + 1; // 将列数保存到 pos 中  
			if (chessMap[pos->row][pos->col] == 0) { // 如果该位置为空，可以设置  
				set = true; // 设置 set 为 true  
			}
			break; // 跳出循环  
		}
	} while (0);
	return set; // 返回 set 的值  
}

int Chess::calculateDistance(int x1, int y1, int x2, int y2) {
	int dx = x1 - x2;
	int dy = y1 - y2;
	return sqrt(dx * dx + dy * dy);
}
//
void Chess::ChessDown(ChessPos* pos, chess_Kind_t kind)
{
	mciSendString("play res/down7.WAV", 0, 0, 0);

	int x = margin_x + pos->col * chessSize - 0.5 * chessSize;
	int y = margin_y + pos->row * chessSize - 0.5 * chessSize;

	if (kind == CHESS_WHITE) {
		putimagePNG(x, y, &chessWhiteImg);
	}
	else {
		putimagePNG(x, y, &chessBlackImg);
	}
	updateGameMap(pos);
}

int Chess::getGradeSize()
{
	return grideSize;
}

int Chess::getChessData(ChessPos* pos)
{
	return chessMap[pos->row][pos->col];
}

int Chess::getChessData(int row, int col)
{
	return chessMap[row][col];
}

//恢复
int s1 = 0;
int s2 = 0;
bool Chess::checkOver() {
	  
        if (chickWin()) {
            Sleep(1500);
            if (playerFlag == false) {
				// 黑棋赢了
				if (s2 == 2)// 赢三局
				{
					mciSendString("play res/不错.mp3", 0, 0, 0);
					loadimage(0, "res/c13.jpg");
					++s1;
				}
				else
				{
					mciSendString("play res/不错.mp3", 0, 0, 0);
					loadimage(0, "res/胜利.jpg");
					++s1;
				}
            } else {
				
                // 白棋赢了
				if (s2 == 2 && s1 == 0) // 连赢三局
				{
					mciSendString("play res/失败.mp3", 0, 0, 0);
					loadimage(0, "res/c12.jpg");
					++s2;
					if (surrender())
					{
						return true;
					}
			
				}
				else
				{
					mciSendString("play res/失败.mp3", 0, 0, 0);
					loadimage(0, "res/失败.jpg");
					++s2;
				}
            }

            cout << "黑棋当前的得分是" << s1 << endl;
            cout << "白棋当前的得分是" << s2 << endl;

            // 将分数写入文件
            saveScores();

            _getch(); // 暂停
            return true;
        }
        return false;
    }
  
bool Chess::surrender() {
	  bool isLost = false;
	  char input;
	  while (true) {
		  cout << "您已连输三局，是否认输? (Y/N)" << endl;
		  cin >> input;
		  if (input == 'Y') {
			  isLost = true;
			  mciSendString("play res/失败.mp3", 0, 0, 0);
			  loadimage(0, "res/c15.jpg");
			  Sleep(2000); // 等待  
			  // 认输退出程序  
			  exit(1);
		  }
		  else if (input == 'N') {
			  loadimage(0, "res/c14.jpg");
			  Sleep(1000);
			  break;
		  }
	  }
	  return isLost;
  }

void Chess::saveScores()
  {
	  // 将分数写入文件
	  ofstream file("D:/scores.txt");
	  if (file.is_open()) {
		  file << s1 << " " << s2;
		  file.close();
	  }
	  else {
		  cout << "无法打开文件以保存分数" << endl;
	  }
  }

void Chess::updateGameMap(ChessPos* pos)
{
	lastPos = *pos;
	chessMap[pos->row][pos->col] = playerFlag ? CHESS_BLACK : CHESS_WHITE;
	playerFlag = !playerFlag;//黑白方交换行棋
}

bool Chess::chickWin()
{
	//最近落子点的位置
	int row = lastPos.row;
	int col = lastPos.col;

	//落子点的水平方向
	for (int i = 0;i < 5;i++)
	{
		//对于第i次
		//五子连珠最左侧位置
		if (col - i >= 0 && col - i + 4 < grideSize &&
			chessMap[row][col - i] == chessMap[row][col - i + 1] && 
			chessMap[row][col - i] == chessMap[row][col - i + 2] &&
			chessMap[row][col - i] == chessMap[row][col - i + 3] &&
			chessMap[row][col - i] == chessMap[row][col - i + 4])
		{
			return true;
		}
	}
	//垂直方向
	for (int i = 0;i < 5;i++)
	{
		if (row - i >= 0 && row - i + 4 < grideSize &&
			chessMap[row - i][col] == chessMap[row - i + 1][col] &&
			chessMap[row - i][col] == chessMap[row - i + 2][col] &&
			chessMap[row - i][col] == chessMap[row - i + 3][col] &&
			chessMap[row - i][col] == chessMap[row - i + 4][col] )
		{
			return true;
		}
	}

	//右斜方向
	for (int i = 0;i < 5;i++)
	{
		if (row + i < grideSize && row + i - 4 >= 0 && col - i >= 0 && col - i + 4 < grideSize &&
			chessMap[row + i][col - i] == chessMap[row + i - 1][col - i + 1] &&
			chessMap[row + i][col - i] == chessMap[row + i - 2][col - i + 2] &&
			chessMap[row + i][col - i] == chessMap[row + i - 3][col - i + 3] &&
			chessMap[row + i][col - i] == chessMap[row + i - 4][col - i + 4])
		{
			return true;
		}
	}

	//左斜方向
	for (int i = 0;i < 5;i++)
	{
		if (row - i >= 0 && row - i + 4 < grideSize && col - i >= 0 && col - i + 4 < grideSize &&
			chessMap[row - i][col - i] == chessMap[row - i + 1][col - i + 1] &&
			chessMap[row - i][col - i] == chessMap[row - i + 2][col - i + 2] &&
			chessMap[row - i][col - i] == chessMap[row - i + 3][col - i + 3] &&
			chessMap[row - i][col - i] == chessMap[row - i + 4][col - i + 4])
		{
			return true;
		}
	}
	return false;
}