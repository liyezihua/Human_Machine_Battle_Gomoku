#include<iostream>
using namespace std;
#include"ChessGame.h"
#include"Chess.h"

int main()
{
	Man man;
	AI ai;
	Chess chess(13, 43, 43, 67.3);

	ChessGame game(&man, &ai, &chess);

	game.play();
	return 0;
}