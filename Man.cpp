#include "Man.h"
#include<stdio.h>
#include<easyx.h>
#include"Chess.h"
#include"ChessGame.h"
#include<iostream>
using namespace std;


void Man::init(Chess* chess)
{
	this->chess = chess;
}

void Man::go()
{
	//ExMessage msg;
	//MOUSEMSG msg;
	ChessPos pos;
		while (true)
		{
			// 获取鼠标点击消息
			ExMessage msg = getmessage();
			//msg = getmessage();
			//通过chess对象，来调用判断落子是否有效 以及落子功能
			if (msg.message == WM_LBUTTONDOWN && chess->clickBoard(msg.x,msg.y,&pos) ) {
				break;
			}
		}
		cout <<"黑子"<< pos.row << "," << pos.col << endl;
	//落子
	chess->ChessDown(&pos, CHESS_BLACK);

}

