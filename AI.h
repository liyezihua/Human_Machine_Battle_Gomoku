#pragma once
#include "Chess.h"
#include<vector>
class AI
{
public:
	//AI初始化
	void init(Chess *chess);
	//AI开始下棋
	void go();
private:
	Chess* chess;
	vector<vector<int>> scoreMap;

private:
	//AI判定(后加)
	//普通
	void calculateScore();
	//高级
	void calculateScore1();

	ChessPos think();

};

