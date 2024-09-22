#include "ChessGame.h"


ChessGame::ChessGame(Man* man, AI* ai, Chess* chess)
{
	this->man = man;
	this->chess = chess;
	this->ai = ai;

	
	man->init(chess);
	ai->init(chess);
}

void ChessGame::play()
{
	//棋盘初始化
	chess->Start_The_Game();
	chess->init();
	while (1)
	{
		//先有棋手走棋
		man->go();
		if (chess->checkOver())
		{
			chess->init();
			continue;
		}
		//ai走棋
		ai->go();
		if (chess->checkOver())
		{
			chess->init();
			continue;
		}
	}
}
