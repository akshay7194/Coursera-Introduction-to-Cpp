//============================================================================
// Name        : hw4.cpp
// Author      : Akshay Joshi
// Version     :
// Copyright   : Your copyright notice
// Description : Homework 4, Setup HEX board and figure out who won
//============================================================================

#include <iostream>
#include <vector>
#include <queue>
#include <cassert>
using namespace std;

enum class Player {RED, BLUE, BLANK};

struct mapping{
	Player player;
	bool visit;

	mapping(Player player = Player::BLANK, bool visit = 0):
			player(player), visit(visit)
		{
		}
};


class Hex{
public:

	Hex(int size = 3);

	void updateMove(int x, int y, Player player);

	bool isValidMove(int x, int y);

	bool ifWon(int x, int y, Player player);

	void isSide(int x, int y, vector<bool> &flags, Player player);

	void printBoard();

private:
	int size;
	vector<vector<mapping> > graph;
	const static int dirs[6][2];
};


Hex::Hex(int verticeNum){

	if (verticeNum <= 0)
	{
		return;
	}

	size = verticeNum;
	graph = vector<vector<mapping> >(size, vector<mapping>(size));

}

void Hex::isSide(int x, int y, vector<bool> &flags, Player player){
	assert(x>=0 && x<size && y>=0 && y<size);

	if (player == Player::RED){
		if (x == 0)
			flags[0] = 1;
		if (x == size - 1)
			flags[1] = 1;
	}

	else if (player == Player::BLUE){
		if (y == 0)
			flags[0] = 1;
		if (y == size - 1)
			flags[1] = 1;
	}
}


const int Hex::dirs[6][2] =
{
	{-1, 0}, {1, 0},  // --
	{0, -1}, {0, 1},  //  |
	{-1, 1}, {1, -1}  //  /
};


bool Hex::isValidMove(int x, int y){
	assert(x>=0 && x<size && y>=0 && y<size);

	if (graph[x][y].player != Player::BLANK && graph[x][y].visit != 0){
		return 0;
	}

	return 1;
}

void Hex::updateMove(int x, int y, Player eitherofTwo){
	assert(x>=0 && x<size && y>=0 && y<size);

	graph[x][y].player = eitherofTwo;
	graph[x][y].visit = 1;
}

void Hex::printBoard(){

	string slanted = "     \\" ;
	string header =  "   ";
	string footer = "   ";

	for (int i = 1; i<size; i++){
		slanted += " / \\";
		header += "R   " ;
		footer += "R   " ;
	}

	cout << header  + "R   " << endl;

	int count;
	for (int a = 0; a < size; a++){
		count  = a;
		while (count >= 1){
			cout << " " ;
			count = count-1;
		}

		cout << "B   ";
		footer = " " + footer;
		for (int b = 0; b < size; b++){
			if (b != size-1){
				if (graph[a][b].player == Player::BLUE)
					cout << " " << "B"  << " -";
				else if (graph[a][b].player == Player::RED)
					cout << " " << "R"  << " -";
				else
					cout << " " << "."  << " -";
			}
			else{
				if (graph[a][b].player == Player::BLUE)
					cout << " " << "B" << "   B";
				else if (graph[a][b].player == Player::RED)
					cout << " " << "R" << "   B";
				else
					cout << " " << "." << "   B";
			}
		}
		cout << endl;

		if (a != size-1){
			cout << slanted << endl;
			slanted = " " + slanted ;
		}
	}

	cout << "\n" << "  " + footer + "R   "<< endl;
}


bool Hex::ifWon(int x, int y, Player player){

	vector<bool> flags(2, false);

	vector<vector<bool> > visited(size, vector<bool>(size));

	queue<pair<int, int> > traces;

	traces.push(make_pair(x, y));

	visited[x][y] = true;

	while (!traces.empty())
	{
		auto top = traces.front();
		isSide(top.first, top.second, flags, player);

		traces.pop();

		for (int n=0; n<6; n++)
		{
			int curX = top.first + dirs[n][0];
			int curY = top.second + dirs[n][1];
			if (curX>=0 && curX<size && curY>=0 && curY<size && graph[curX][curY].player == player
					&& visited[curX][curY] == false)
			{
				visited[curX][curY] = true;
				traces.push(make_pair(curX, curY));
			}
		}
	}


	return flags[0] && flags[1];
}

int main() {

	int verticeNum;
	cout << "Enter verticeNum : " ;
	cin >> verticeNum;

	Hex h(verticeNum);

	bool chance = 0;
	int steps = 0;

	srand(time(0));

	int x_cord, y_cord;

	while (true){

		steps += 1;
		chance = !chance;   //making sure blue always plays first

		x_cord = rand() % verticeNum;
		y_cord = rand() % verticeNum;

		while(!h.isValidMove(x_cord, y_cord)){
			x_cord = rand() % verticeNum;
			y_cord = rand() % verticeNum;
		}

		if (chance == 0){//RED
			cout << "The move by RED player is (" << x_cord << "," << y_cord << ")" << endl;

			h.updateMove(x_cord, y_cord, Player::RED);

			if (h.ifWon(x_cord, y_cord, Player::RED)){
				h.printBoard();
				cout << "\nRed player has won in " << steps << " steps.";
				break;
			}
		}

		else {//BLUE
			cout << "The move by BLUE player is (" << x_cord << "," << y_cord << ")" << endl;

			h.updateMove(x_cord, y_cord, Player::BLUE);

			if (h.ifWon(x_cord, y_cord, Player::BLUE)){
				h.printBoard();
				cout << "\nBlue player has won in " << steps << " steps.";
				break;
			}
		}

		h.printBoard();

	}

	return 0;
}
