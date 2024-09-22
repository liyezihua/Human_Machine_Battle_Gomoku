#include "AI.h"


void AI::init(Chess* chess)
{
	this->chess = chess;
	int size = chess->getGradeSize();
	for (int i = 0;i < size;i++)
	{
		vector<int> row;
		for (int j = 0;j < size;j++)
		{
			row.push_back(0);
		}
		scoreMap.push_back(row);
	}
}

void AI::go()
{
	ChessPos pos = think();
	
	Sleep(1000);//假装思考
	
	chess->ChessDown(&pos, CHESS_WHITE);
	cout <<"白子"<< pos.row << "," << pos.col << endl;
}

//普通难度
void AI::calculateScore() {
    int blackNum = 0; // 棋手方（黑棋）多少个连续的棋子
    int whiteNum = 0; // AI方（白棋）连续有多少个连续的棋子
    int emptyNum = 0; // 该方向上空白位的个数
    // 评分向量数组清零
    for (int i = 0; i < scoreMap.size(); i++) {
        for (int j = 0; j < scoreMap[i].size(); j++) {
            scoreMap[i][j] = 0;
        }
    }
    int size = chess->getGradeSize();
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            // 对每个点进行计算
            if (chess->getChessData(row, col)) continue;

            // 计算黑棋棋型
            for (int y = -1; y <= 0; y++) {
                for (int x = -1; x <= 1; x++) {
                    if (y == 0 && x == 0) continue;
                    if (y == 0 && x != 1) continue;
                    blackNum = 0;
                    emptyNum = 0;
                    // 正向计算
                    for (int i = 1; i <= 4; i++) {
                        int curRow = row + i * y;
                        int curCol = col + i * x;
                        if (curRow >= 0 && curRow < size &&
                            curCol >= 0 && curCol < size &&
                            chess->getChessData(curRow, curCol) == 1) {
                            blackNum++;
                        }
                        else if (curRow >= 0 && curRow < size &&
                            curCol >= 0 && curCol < size &&
                            chess->getChessData(curRow, curCol) == 0) {
                            emptyNum++;
                            break;
                        }
                        else {
                            break;
                        }
                    }
                    // 反向计算
                    for (int i = 1; i <= 4; i++) {
                        int curRow = row - i * y;
                        int curCol = col - i * x;
                        if (curRow >= 0 && curRow < size &&
                            curCol >= 0 && curCol < size &&
                            chess->getChessData(curRow, curCol) == 1) {
                            blackNum++;
                        }
                        else if (curRow >= 0 && curRow < size &&
                            curCol >= 0 && curCol < size &&
                            chess->getChessData(curRow, curCol) == 0) {
                            emptyNum++;
                            break;
                        }
                        else {
                            break;
                        }
                    }
                    // 计算黑棋棋型得分
                    if (blackNum == 1) {
                        scoreMap[row][col] += 10;
                    }
                    else if (blackNum == 2) {
                        if (emptyNum == 1) {
                            scoreMap[row][col] += 30;
                        }
                        else if (emptyNum == 2) {
                            scoreMap[row][col] += 40;
                        }
                    }
                    else if (blackNum == 3) {
                        if (emptyNum == 1) {
                            scoreMap[row][col] += 60;
                        }
                        else if (emptyNum == 2) {
                            scoreMap[row][col] += 5000;
                        }
                    }
                    else if (blackNum == 4) {
                        scoreMap[row][col] += 20000;
                    }
                    else if (blackNum >= 5) {
                        // 判断跳四和跳三
                        if (emptyNum == 1) {
                            scoreMap[row][col] += 100000;
                        }
                        else if (emptyNum == 2) {
                            scoreMap[row][col] += 100000;
                        }
                    }
                }
            }

            // 斜三阵的开局策略 
            if (row >= 2 && col >= 2 && row < size - 2 && col < size - 2) {
                if (chess->getChessData(row - 2, col - 2) == 0 &&
                    chess->getChessData(row - 1, col - 1) == 0 &&
                    chess->getChessData(row + 1, col + 1) == 0 &&
                    chess->getChessData(row + 2, col + 2) == 0) {
                    scoreMap[row][col] += 500;
                }
            }

            // 半燕翼的开局策略 
            if (row >= 2 && col >= 1 && row < size - 2 && col < size - 1) {
                if (chess->getChessData(row - 2, col - 1) == 0 &&
                    chess->getChessData(row - 1, col - 1) == 0 &&
                    chess->getChessData(row + 1, col + 1) == 0 &&
                    chess->getChessData(row + 2, col + 1) == 0) {
                    scoreMap[row][col] += 500;
                }
            }

            // 计算白棋棋型
            emptyNum = 0;
            for (int y = -1; y <= 0; y++) {
                for (int x = -1; x <= 1; x++) {
                    if (y == 0 && x == 0) continue;
                    if (y == 0 && x != 1) continue;
                    whiteNum = 0;
                    emptyNum = 0;
                    // 正向计算
                    for (int i = 1; i <= 4; i++) {
                        int curRow = row + i * y;
                        int curCol = col + i * x;
                        if (curRow >= 0 && curRow < size &&
                            curCol >= 0 && curCol < size &&
                            chess->getChessData(curRow, curCol) == -1) {
                            whiteNum++;
                        }
                        else if (curRow >= 0 && curRow < size &&
                            curCol >= 0 && curCol < size &&
                            chess->getChessData(curRow, curCol) == 0) {
                            emptyNum++;
                            break;
                        }
                        else {
                            break;
                        }
                    }
                    // 反向计算
                    for (int i = 1; i <= 4; i++) {
                        int curRow = row - i * y;
                        int curCol = col - i * x;
                        if (curRow >= 0 && curRow < size &&
                            curCol >= 0 && curCol < size &&
                            chess->getChessData(curRow, curCol) == -1) {
                            whiteNum++;
                        }
                        else if (curRow >= 0 && curRow < size &&
                            curCol >= 0 && curCol < size &&
                            chess->getChessData(curRow, curCol) == 0) {
                            emptyNum++;
                            break;
                        }
                        else {
                            break;
                        }
                    }
                    // 计算白棋棋型得分
                    if (whiteNum == 0) {
                        scoreMap[row][col] += 5;
                    }
                    else if (whiteNum == 1) {
                        scoreMap[row][col] += 10;
                    }
                    else if (whiteNum == 2) {
                        if (emptyNum == 1) {
                            scoreMap[row][col] += 25;
                        }
                        else if (emptyNum == 2) {
                            scoreMap[row][col] += 50;
                        }
                    }
                    else if (whiteNum == 3) {
                        if (emptyNum == 1) {
                            scoreMap[row][col] += 55;
                        }
                        else if (emptyNum == 2) {
                            scoreMap[row][col] += 10000;
                        }
                    }
                    else if (whiteNum >= 4) {
                        scoreMap[row][col] += 30000;
                    }
                    else if (whiteNum >= 5) {
                        // 判断跳四和跳三
                        if (emptyNum == 1) {
                            scoreMap[row][col] += 50000;
                        }
                        else if (emptyNum == 2) {
                            scoreMap[row][col] += 100000;
                        }
                    }
                }
            }

            // 判断斜三阵
            if (row >= 2 && col >= 2 && row < size - 2 && col < size - 2) {
                if (chess->getChessData(row - 2, col - 2) == 0 &&
                    chess->getChessData(row - 1, col - 1) == 0 &&
                    chess->getChessData(row + 1, col + 1) == 0 &&
                    chess->getChessData(row + 2, col + 2) == 0) {
                    scoreMap[row][col] += 500;
                }
            }

            // 判断半燕翼
            if (row >= 2 && col >= 1 && row < size - 2 && col < size - 1) {
                if (chess->getChessData(row - 2, col - 1) == 0 &&
                    chess->getChessData(row - 1, col - 1) == 0 &&
                    chess->getChessData(row + 1, col + 1) == 0 &&
                    chess->getChessData(row + 2, col + 1) == 0) {
                    scoreMap[row][col] += 500;
                }
            }
        }
    }
}
//中级难度
void AI::calculateScore1() {
    int blackNum = 0; // 棋手方（黑棋）多少个连续的棋子
    int whiteNum = 0; // AI方（白棋）连续有多少个连续的棋子
    int emptyNum = 0; // 该方向上空白位的个数
    // 评分向量数组清零
    for (int i = 0; i < scoreMap.size(); i++) {
        for (int j = 0; j < scoreMap[i].size(); j++) {
            scoreMap[i][j] = 0;
        }
    }
    int size = chess->getGradeSize();
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            // 对每个点进行计算
            if (chess->getChessData(row, col)) continue;

            // 计算黑棋棋型
            for (int y = -1; y <= 0; y++) {
                for (int x = -1; x <= 1; x++) {
                    if (y == 0 && x == 0) continue;
                    if (y == 0 && x != 1) continue;
                    blackNum = 0;
                    emptyNum = 0;
                    // 正向计算
                    for (int i = 1; i <= 4; i++) {
                        int curRow = row + i * y;
                        int curCol = col + i * x;
                        if (curRow >= 0 && curRow < size &&
                            curCol >= 0 && curCol < size &&
                            chess->getChessData(curRow, curCol) == 1) {
                            blackNum++;
                        }
                        else if (curRow >= 0 && curRow < size &&
                            curCol >= 0 && curCol < size &&
                            chess->getChessData(curRow, curCol) == 0) {
                            emptyNum++;
                            break;
                        }
                        else {
                            break;
                        }
                    }
                    // 反向计算
                    for (int i = 1; i <= 4; i++) {
                        int curRow = row - i * y;
                        int curCol = col - i * x;
                        if (curRow >= 0 && curRow < size &&
                            curCol >= 0 && curCol < size &&
                            chess->getChessData(curRow, curCol) == 1) {
                            blackNum++;
                        }
                        else if (curRow >= 0 && curRow < size &&
                            curCol >= 0 && curCol < size &&
                            chess->getChessData(curRow, curCol) == 0) {
                            emptyNum++;
                            break;
                        }
                        else {
                            break;
                        }
                    }
                    // 计算黑棋棋型得分
                    if (blackNum == 1) {
                        scoreMap[row][col] += 10;
                    }
                    else if (blackNum == 2) {
                        if (emptyNum == 1) {
                            scoreMap[row][col] += 30;
                        }
                        else if (emptyNum == 2) {
                            scoreMap[row][col] += 40;
                        }
                    }
                    else if (blackNum == 3) {
                        if (emptyNum == 1) {
                            scoreMap[row][col] += 60;
                        }
                        else if (emptyNum == 2) {
                            scoreMap[row][col] += 5000;
                        }
                    }
                    else if (blackNum == 4) {
                        scoreMap[row][col] += 20000;
                    }
                    else if (blackNum >= 5) {
                        // 判断跳四和跳三
                        if (emptyNum == 1) {
                            scoreMap[row][col] += 100000;
                        }
                        else if (emptyNum == 2) {
                            scoreMap[row][col] += 100000;
                        }
                    }
                }
            }

         

            // 计算白棋棋型
            emptyNum = 0;
            for (int y = -1; y <= 0; y++) {
                for (int x = -1; x <= 1; x++) {
                    if (y == 0 && x == 0) continue;
                    if (y == 0 && x != 1) continue;
                    whiteNum = 0;
                    emptyNum = 0;
                    // 正向计算
                    for (int i = 1; i <= 4; i++) {
                        int curRow = row + i * y;
                        int curCol = col + i * x;
                        if (curRow >= 0 && curRow < size &&
                            curCol >= 0 && curCol < size &&
                            chess->getChessData(curRow, curCol) == -1) {
                            whiteNum++;
                        }
                        else if (curRow >= 0 && curRow < size &&
                            curCol >= 0 && curCol < size &&
                            chess->getChessData(curRow, curCol) == 0) {
                            emptyNum++;
                            break;
                        }
                        else {
                            break;
                        }
                    }
                    // 反向计算
                    for (int i = 1; i <= 4; i++) {
                        int curRow = row - i * y;
                        int curCol = col - i * x;
                        if (curRow >= 0 && curRow < size &&
                            curCol >= 0 && curCol < size &&
                            chess->getChessData(curRow, curCol) == -1) {
                            whiteNum++;
                        }
                        else if (curRow >= 0 && curRow < size &&
                            curCol >= 0 && curCol < size &&
                            chess->getChessData(curRow, curCol) == 0) {
                            emptyNum++;
                            break;
                        }
                        else {
                            break;
                        }
                    }
                    // 计算白棋棋型得分
                    if (whiteNum == 0) {
                        scoreMap[row][col] += 5;
                    }
                    else if (whiteNum == 1) {
                        scoreMap[row][col] += 10;
                    }
                    else if (whiteNum == 2) {
                        if (emptyNum == 1) {
                            scoreMap[row][col] += 25;
                        }
                        else if (emptyNum == 2) {
                            scoreMap[row][col] += 50;
                        }
                    }
                    else if (whiteNum == 3) {
                        if (emptyNum == 1) {
                            scoreMap[row][col] += 55;
                        }
                        else if (emptyNum == 2) {
                            scoreMap[row][col] += 10000;
                        }
                    }
                    else if (whiteNum >= 4) {
                        scoreMap[row][col] += 30000;
                    }
                    else if (whiteNum >= 5) {
                        // 判断跳四和跳三
                        if (emptyNum == 1) {
                            scoreMap[row][col] += 50000;
                        }
                        else if (emptyNum == 2) {
                            scoreMap[row][col] += 100000;
                        }
                    }
                }
            } 
        }
    }
}

//高级(调试)
//void AI::calculateScore() {
//    int blackNum = 0; // 棋手方（黑棋）多少个连续的棋子
//    int whiteNum = 0; // AI方（白棋）连续有多少个连续的棋子
//    int emptyNum = 0; // 该方向上空白位的个数
//     //评分向量数组清零
//    for (int i = 0; i < scoreMap.size(); i++) {
//        for (int j = 0; j < scoreMap[i].size(); j++) {
//            scoreMap[i][j] = 0;
//        }
//    }
//    int size = chess->getGradeSize();
//    for (int row = 0; row < size; row++) {
//        for (int col = 0; col < size; col++) {
//             //对每个点进行计算
//            if (chess->getChessData(row, col)) continue;
//
//             //计算黑棋棋型
//            for (int y = -1; y <= 0; y++) {
//                for (int x = -1; x <= 1; x++) {
//                    if (y == 0 && x == 0) continue;
//                    if (y == 0 && x != 1) continue;
//                    blackNum = 0;
//                    emptyNum = 0;
//                     //正向计算
//                    for (int i = 1; i <= 4; i++) {
//                        int curRow = row + i * y;
//                        int curCol = col + i * x;
//                        if (curRow >= 0 && curRow < size &&
//                            curCol >= 0 && curCol < size &&
//                            chess->getChessData(curRow, curCol) == 1) {
//                            blackNum++;
//                        }
//                        else if (curRow >= 0 && curRow < size &&
//                            curCol >= 0 && curCol < size &&
//                            chess->getChessData(curRow, curCol) == 0) {
//                            emptyNum++;
//                            break;
//                        }
//                        else {
//                            break;
//                        }
//                    }
//                     //反向计算
//                    for (int i = 1; i <= 4; i++) {
//                        int curRow = row - i * y;
//                        int curCol = col - i * x;
//                        if (curRow >= 0 && curRow < size &&
//                            curCol >= 0 && curCol < size &&
//                            chess->getChessData(curRow, curCol) == 1) {
//                            blackNum++;
//                        }
//                        else if (curRow >= 0 && curRow < size &&
//                            curCol >= 0 && curCol < size &&
//                            chess->getChessData(curRow, curCol) == 0) {
//                            emptyNum++;
//                            break;
//                        }
//                        else {
//                            break;
//                        }
//                    }
//                     //计算黑棋棋型得分
//                    if (blackNum == 1) {
//                        scoreMap[row][col] += 10;
//                    }
//                    else if (blackNum == 2) {
//                        if (emptyNum == 1) {
//                            scoreMap[row][col] += 30;
//                        }
//                        else if (emptyNum == 2) {
//                            scoreMap[row][col] += 40;
//                        }
//                    }
//                    else if (blackNum == 3) {
//                        if (emptyNum == 1) {
//                            scoreMap[row][col] += 600;
//                        }
//                        else if (emptyNum == 2) {
//                            scoreMap[row][col] += 5000;
//                        }
//                    }
//                    else if (blackNum == 4) {
//                        scoreMap[row][col] += 20000;
//                    }
//                }
//            }
//
//             //斜三阵的开局策略 
//            if (row >= 2 && col >= 2 && row < size - 2 && col < size - 2) {
//                if (chess->getChessData(row - 2, col - 2) == 0 &&
//                    chess->getChessData(row - 1, col - 1) == 0 &&
//                    chess->getChessData(row + 1, col + 1) == 0 &&
//                    chess->getChessData(row + 2, col + 2) == 0) {
//                    scoreMap[row][col] += 500;
//                }
//            }
//
//             //半燕翼的开局策略 
//            if (row >= 2 && col >= 1 && row < size - 2 && col < size - 1) {
//                if (chess->getChessData(row - 2, col - 1) == 0 &&
//                    chess->getChessData(row - 1, col - 1) == 0 &&
//                    chess->getChessData(row + 1, col + 1) == 0 &&
//                    chess->getChessData(row + 2, col + 1) == 0) {
//                    scoreMap[row][col] += 500;
//                }
//            }
//
//             //判断四角阵
//            if ((row == 0 && col == 0) ||
//                (row == 0 && col == size - 1) ||
//                (row == size - 1 && col == 0) ||
//                (row == size - 1 && col == size - 1)) {
//                scoreMap[row][col] += 1000;
//            }
//
//            // 判断双四角阵
//            if ((row == 0 && col == 0 && chess->getChessData(size - 1, size - 1) == 0) ||
//                (row == 0 && col == size - 1 && chess->getChessData(size - 1, 0) == 0) ||
//                (row == size - 1 && col == 0 && chess->getChessData(0, size - 1) == 0) ||
//                (row == size - 1 && col == size - 1 && chess->getChessData(0, 0) == 0)) {
//                scoreMap[row][col] += 1500;
//            }
//
//            // 判断梅花阵
//            if (row >= 1 && col >= 1 && row < size - 1 && col < size - 1) {
//                if (chess->getChessData(row - 1, col) == 0 &&
//                    chess->getChessData(row, col - 1) == 0 &&
//                    chess->getChessData(row, col + 1) == 0 &&
//                    chess->getChessData(row + 1, col) == 0) {
//                    scoreMap[row][col] += 600;
//                }
//            }
//
//            // 判断剑阵
//            if (row >= 1 && col >= 1 && row < size - 1 && col < size - 1) {
//                if (chess->getChessData(row - 1, col - 1) == 0 &&
//                    chess->getChessData(row - 1, col + 1) == 0 &&
//                    chess->getChessData(row + 1, col - 1) == 0 &&
//                    chess->getChessData(row + 1, col + 1) == 0) {
//                    scoreMap[row][col] += 600;
//                }
//            }
//
//            // 判断燕阵
//            if (row >= 1 && col >= 2 && row < size - 1 && col < size - 2) {
//                if (chess->getChessData(row - 1, col - 1) == 0 &&
//                    chess->getChessData(row - 1, col + 1) == 0 &&
//                    chess->getChessData(row, col - 2) == 0 &&
//                    chess->getChessData(row, col + 2) == 0 &&
//                    chess->getChessData(row + 1, col - 1) == 0 &&
//                    chess->getChessData(row + 1, col + 1) == 0) {
//                    scoreMap[row][col] += 600;
//                }
//            }
//
//            // 判断八卦阵
//            if (row >= 1 && col >= 1 && row < size - 1 && col < size - 1) {
//                if (chess->getChessData(row - 1, col) == 0 &&
//                    chess->getChessData(row, col - 1) == 0 &&
//                    chess->getChessData(row, col + 1) == 0 &&
//                    chess->getChessData(row + 1, col) == 0 &&
//                    chess->getChessData(row - 1, col - 1) == 0 &&
//                    chess->getChessData(row - 1, col + 1) == 0 &&
//                    chess->getChessData(row + 1, col - 1) == 0 &&
//                    chess->getChessData(row + 1, col + 1) == 0) {
//                    scoreMap[row][col] += 600;
//                }
//            }
//
//            // 判断三三阵
//            if (row >= 1 && col >= 1 && row < size - 1 && col < size - 1) {
//                if (chess->getChessData(row - 1, col - 1) == 0 &&
//                    chess->getChessData(row - 1, col) == 0 &&
//                    chess->getChessData(row - 1, col + 1) == 0 &&
//                    chess->getChessData(row, col - 1) == 0 &&
//                    chess->getChessData(row, col) == 0 &&
//                    chess->getChessData(row, col + 1) == 0 &&
//                    chess->getChessData(row + 1, col - 1) == 0 &&
//                    chess->getChessData(row + 1, col) == 0 &&
//                    chess->getChessData(row + 1, col + 1) == 0) {
//                    scoreMap[row][col] += 800;
//                }
//            }
//
//             //计算白棋棋型
//            emptyNum = 0;
//            for (int y = -1; y <= 0; y++) {
//                for (int x = -1; x <= 1; x++) {
//                    if (y == 0 && x == 0) continue;
//                    if (y == 0 && x != 1) continue;
//                    whiteNum = 0;
//                    emptyNum = 0;
//                     //正向计算
//                    for (int i = 1; i <= 4; i++) {
//                        int curRow = row + i * y;
//                        int curCol = col + i * x;
//                        if (curRow >= 0 && curRow < size &&
//                            curCol >= 0 && curCol < size &&
//                            chess->getChessData(curRow, curCol) == -1) {
//                            whiteNum++;
//                        }
//                        else if (curRow >= 0 && curRow < size &&
//                            curCol >= 0 && curCol < size &&
//                            chess->getChessData(curRow, curCol) == 0) {
//                            emptyNum++;
//                            break;
//                        }
//                        else {
//                            break;
//                        }
//                    }
//                     //反向计算
//                    for (int i = 1; i <= 4; i++) {
//                        int curRow = row - i * y;
//                        int curCol = col - i * x;
//                        if (curRow >= 0 && curRow < size &&
//                            curCol >= 0 && curCol < size &&
//                            chess->getChessData(curRow, curCol) == -1) {
//                            whiteNum++;
//                        }
//                        else if (curRow >= 0 && curRow < size &&
//                            curCol >= 0 && curCol < size &&
//                            chess->getChessData(curRow, curCol) == 0) {
//                            emptyNum++;
//                            break;
//                        }
//                        else {
//                            break;
//                        }
//                    }
//                     //计算白棋棋型得分
//                    if (whiteNum == 0) {
//                        scoreMap[row][col] += 5;
//                    }
//                    else if (whiteNum == 1) {
//                        scoreMap[row][col] += 10;
//                    }
//                    else if (whiteNum == 2) {
//                        if (emptyNum == 1) {
//                            scoreMap[row][col] += 25;
//                        }
//                        else if (emptyNum == 2) {
//                            scoreMap[row][col] += 50;
//                        }
//                    }
//                    else if (whiteNum == 3) {
//                        if (emptyNum == 1) {
//                            scoreMap[row][col] += 550;
//                        }
//                        else if (emptyNum == 2) {
//                            scoreMap[row][col] += 10000;
//                        }
//                    }
//                    else if (whiteNum >= 4) {
//                        scoreMap[row][col] += 30000;
//                    }
//                }
//            }
//
//             //判断斜三阵
//            if (row >= 2 && col >= 2 && row < size - 2 && col < size - 2) {
//                if (chess->getChessData(row - 2, col - 2) == 0 &&
//                    chess->getChessData(row - 1, col - 1) == 0 &&
//                    chess->getChessData(row + 1, col + 1) == 0 &&
//                    chess->getChessData(row + 2, col + 2) == 0) {
//                    scoreMap[row][col] += 500;
//                }
//            }
//
//             //判断半燕翼
//            if (row >= 2 && col >= 1 && row < size - 2 && col < size - 1) {
//                if (chess->getChessData(row - 2, col - 1) == 0 &&
//                    chess->getChessData(row - 1, col - 1) == 0 &&
//                    chess->getChessData(row + 1, col + 1) == 0 &&
//                    chess->getChessData(row + 2, col + 1) == 0) {
//                    scoreMap[row][col] += 500;
//                }
//            }
//
//             //判断四角阵
//            if ((row == 0 && col == 0) ||
//                (row == 0 && col == size - 1) ||
//                (row == size - 1 && col == 0) ||
//                (row == size - 1 && col == size - 1)) {
//                scoreMap[row][col] += 1000;
//            }
//
//            // 判断双四角阵
//            if ((row == 0 && col == 0 && chess->getChessData(size - 1, size - 1) == 0) ||
//                (row == 0 && col == size - 1 && chess->getChessData(size - 1, 0) == 0) ||
//                (row == size - 1 && col == 0 && chess->getChessData(0, size - 1) == 0) ||
//                (row == size - 1 && col == size - 1 && chess->getChessData(0, 0) == 0)) {
//                scoreMap[row][col] += 1500;
//            }
//
//            // 判断梅花阵
//            if (row >= 1 && col >= 1 && row < size - 1 && col < size - 1) {
//                if (chess->getChessData(row - 1, col) == 0 &&
//                    chess->getChessData(row, col - 1) == 0 &&
//                    chess->getChessData(row, col + 1) == 0 &&
//                    chess->getChessData(row + 1, col) == 0) {
//                    scoreMap[row][col] += 600;
//                }
//            }
//
//            // 判断剑阵
//            if (row >= 1 && col >= 1 && row < size - 1 && col < size - 1) {
//                if (chess->getChessData(row - 1, col - 1) == 0 &&
//                    chess->getChessData(row - 1, col + 1) == 0 &&
//                    chess->getChessData(row + 1, col - 1) == 0 &&
//                    chess->getChessData(row + 1, col + 1) == 0) {
//                    scoreMap[row][col] += 600;
//                }
//            }
//
//            // 判断燕阵
//            if (row >= 1 && col >= 2 && row < size - 1 && col < size - 2) {
//                if (chess->getChessData(row - 1, col - 1) == 0 &&
//                    chess->getChessData(row - 1, col + 1) == 0 &&
//                    chess->getChessData(row, col - 2) == 0 &&
//                    chess->getChessData(row, col + 2) == 0 &&
//                    chess->getChessData(row + 1, col - 1) == 0 &&
//                    chess->getChessData(row + 1, col + 1) == 0) {
//                    scoreMap[row][col] += 600;
//                }
//            }
//
//            // 判断八卦阵
//            if (row >= 1 && col >= 1 && row < size - 1 && col < size - 1) {
//                if (chess->getChessData(row - 1, col) == 0 &&
//                    chess->getChessData(row, col - 1) == 0 &&
//                    chess->getChessData(row, col + 1) == 0 &&
//                    chess->getChessData(row + 1, col) == 0 &&
//                    chess->getChessData(row - 1, col - 1) == 0 &&
//                    chess->getChessData(row - 1, col + 1) == 0 &&
//                    chess->getChessData(row + 1, col - 1) == 0 &&
//                    chess->getChessData(row + 1, col + 1) == 0) {
//                    scoreMap[row][col] += 600;
//                }
//            }
//
//            // 判断三三阵
//            if (row >= 1 && col >= 1 && row < size - 1 && col < size - 1) {
//                if (chess->getChessData(row - 1, col - 1) == 0 &&
//                    chess->getChessData(row - 1, col) == 0 &&
//                    chess->getChessData(row - 1, col + 1) == 0 &&
//                    chess->getChessData(row, col - 1) == 0 &&
//                    chess->getChessData(row, col) == 0 &&
//                    chess->getChessData(row, col + 1) == 0 &&
//                    chess->getChessData(row + 1, col - 1) == 0 &&
//                    chess->getChessData(row + 1, col) == 0 &&
//                    chess->getChessData(row + 1, col + 1) == 0) {
//                    scoreMap[row][col] += 800;
//                }
//            }
//
//        }
//    }
//}

// 计算分数并选择最高分的位置
ChessPos AI::think()
{
    calculateScore1();
	int maxScore = 0;
	vector<ChessPos> maxPoints;

	int size = chess->getGradeSize();

	for (int row = 0; row < size; row++)
	{
		for (int col = 0; col < size; col++)
		{
			if (chess->getChessData(row, col) == 0)
			{
				int cellScore = scoreMap[row][col];

				if (cellScore > maxScore)
				{
					maxScore = cellScore;
					maxPoints.clear();
					maxPoints.emplace_back(row, col);
				}
				else if (cellScore == maxScore)
				{
					maxPoints.emplace_back(row, col);
				}
			}
		}
	}

	// 使用随机数生成器选择一个最高分的位置
	int index = rand() % maxPoints.size();
	return maxPoints[index];
}
