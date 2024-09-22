#pragma once
#include"AI.h"
#include"Chess.h"
#include"Man.h"
class ChessGame
{
public:

	int dfdfdf;

	ChessGame(Man* man, AI* ai, Chess* chess);
	//开始对局
	void play();

	//添加收据成员
private:
	Man* man;
	AI* ai;
	Chess* chess;
};


 