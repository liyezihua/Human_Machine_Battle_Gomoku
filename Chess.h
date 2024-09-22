#pragma once
#include<iostream>
using namespace std;
#include<vector>
#include<graphics.h>//图形库
//#include<easyx.h>
#include<stack>
struct ChessPos
{
	int row;
	int col;

	ChessPos(int r = 0, int c = 0):row(r), col(c) {}
};

typedef enum {
	CHESS_WHITE = -1,//白棋
    CHESS_BLACK = 1//黑棋
}chess_Kind_t;

class Chess
{
public:
	//棋盘构造函数
	Chess(int grideSize, int marginX, int marginY, float cheesSize);
	//开局画面(后加)
	void Start_The_Game();
	//棋盘的初始化(后改)
	void init();
	//加载棋局皮肤(后加)
	void loadChessImages();

	//判断在指定坐标（x，y）位置是否是有效点击,如果有效把有效点击的行列保存在Pos中(后改)
	bool clickBoard(int x, int y,ChessPos *pos);

	//计算两点之间的距离(后加)
	int calculateDistance(int x1, int y1, int x2, int y2);

	//在棋盘指定的位置落子
	void ChessDown(ChessPos *pos,chess_Kind_t Kind);
	//获取棋盘的大小
	int getGradeSize();
	//获取指定位置是白棋还是黑棋或者空白
	int getChessData(ChessPos* pos);
	int getChessData(int row,int col);
	//检查棋局是否结束(后改)
	bool checkOver();
	//认输功能(后加)
	bool surrender();
	// 将分数写入文件让分数一直更新(后加)
	void saveScores();

private:
	IMAGE chessBlackImg;//黑棋
	IMAGE chessWhiteImg;//白棋

	int grideSize;//棋盘大小
	int margin_x;//棋盘的左侧边距
	int margin_y;//棋盘的顶部边距
	float chessSize = 0.0;//棋子大小
	//存储当前游戏棋局的棋子分布数据 0 空白，-1白子，1黑子
	vector<vector<int>> chessMap;

	stack<ChessPos> history; // 历史记录堆栈(后加)


	//现在该谁落子
	bool playerFlag;//true黑子 false 白子

	void updateGameMap(ChessPos* pos);
	
	//检查是否胜利，胜负已分true否者false
	bool chickWin();

	ChessPos lastPos;//最近的落子点位置

	bool chessLoaded; // 标志变量，表示是否已经加载过棋子图片(后加)
	int ischess;

};
