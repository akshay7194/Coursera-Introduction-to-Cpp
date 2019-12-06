//============================================================================
// Name        : hw5.cpp
// Author      : Akshay Joshi
// Version     :
// Copyright   : Your copyright notice
// Description : Homework 5, Play HEX using monte carlo's simulations for computers move
//============================================================================

#include <iostream>
#include <vector>
#include <queue>
#include <cassert>
#include <random>
#include <utility>
#include <algorithm>

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

	bool Won(Player player);

	void isSide(int x, int y, vector<bool> &flags, Player player);

	void printBoard();

	bool undo(int x, int y);

	bool isBoardFull();

	vector<pair<int, int> > getUnoccupied();

	double getScore(int x, int y, Player player);

	void playHuman(Player player);

	void playComputer(Player player);

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


bool Hex::Won(Player player){

	int count = 0;
	if (player == Player::BLUE){
		for (int i = 0; i < size; i++){
			if (graph[i][0].player == Player::BLUE)
			{
				if (ifWon(i, 0, Player::BLUE)){count+= 1;}
			}
		}
	}
	else {
		for (int i = 0; i < size; i++){
			if (graph[0][i].player == Player::RED)
			{
				if (ifWon(0, i, Player::RED)){count+= 1;}
			}
		}
	}

	if (count == 0){return 0;}
	else{return 1;}

}

bool Hex::isBoardFull(){

	int count = 0;
	for (int i = 0; i < size; i++){
		for (int j = 0; j < size; j++){
			if (!isValidMove(i,j)){
				count += 1;
			}
		}
	}

	bool y = (count == size*size) ? true : false;
	return y;
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

bool Hex::undo(int x, int y)
{
	assert(x>=0 && x<size && y>=0 && y<size);
	graph[x][y].player = Player::BLANK;
	graph[x][y].visit = 0;

	return true;
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

vector<pair<int, int> > Hex::getUnoccupied()
{
	vector<pair<int, int> > unoccupied;

	for (int i=0; i<size; i++)
	{
		for (int j=0; j<size; j++)
		{
			if (graph[i][j].player == Player::BLANK)
				unoccupied.push_back(make_pair(i, j));
		}
	}

	return unoccupied;
}

double Hex::getScore(int x, int y, Player player){

	auto unoccupied = getUnoccupied();
	vector<int>unc;
	for (int i = 0; i<unoccupied.size(); i++){
		unc.push_back(i);
	}

	int chance = 0;
	int count = 0;
	int iterations = 5000;

	for (int n = 0; n<iterations; n++){

		unsigned seed1 = rand() % 10;
	    auto gen = default_random_engine(seed1);
	    shuffle (unc.begin(), unc.end(), gen);

		for (int i = 0; i<unc.size(); i++){
			chance = !chance;
			x = unoccupied[unc[i]].first;
			y = unoccupied[unc[i]].second;

			if (chance == 0){//Computer
				updateMove(x, y, player);
			}
			else {//Human
				if (player == Player::BLUE){
					updateMove(x, y, Player::RED);
				}
				else{
					updateMove(x, y, Player::BLUE);
				}
			}

			if (isBoardFull()){
				if (ifWon(x, y, player)){
					count += 1 ;
				}
			}

		}

		for (auto itr = unoccupied.begin(); itr != unoccupied.end(); ++itr){
			undo(itr->first, itr->second);
		}
	}

	return static_cast<double>(count) / iterations;

}


void Hex::playHuman(Player player){

	int x, y;
	cout << "Input your move: x y = ";
	cin >> x >> y;

	while(!isValidMove(x, y)){
		cout << "Illegal Move!! Input another: x y = ";
		cin >> x >> y;
	}

	cout << "\nYour Move : (" << x << "," << y << ")" << endl;
	updateMove(x, y, player);
	printBoard();
}


void Hex::playComputer(Player player){

	int x_cord, y_cord;

	auto unoccupied = getUnoccupied();

	vector<int>unc;

	for (int i = 0; i<unoccupied.size(); i++){
		unc.push_back(i);
	}

	unsigned seed = rand() % 10;
    auto gen = default_random_engine(seed);
    shuffle (unc.begin(), unc.end(), gen);

	double probMax = 0.0;
	pair<int, int> move(0, 0);

	for (int i = 0; i < unc.size(); i++){
		x_cord = unoccupied[unc[i]].first;
		y_cord = unoccupied[unc[i]].second;

		updateMove(x_cord, y_cord, player);

		double probMove = getScore(x_cord, y_cord, player);

		if (probMove >= probMax){
			move.first = unoccupied[unc[i]].first;
			move.second = unoccupied[unc[i]].second;
			probMax = probMove;
		}

		undo(x_cord, y_cord);
	}

	cout << "\nComputer's Move: (" << move.first << "," << move.second << ")" << endl;
	updateMove(move.first, move.second, player);
	printBoard();
}



int main() {

	int verticeNum; char player_col = '.';
	cout << "Enter verticeNum : " ;
	cin >> verticeNum;

	cout << "\nEnter your color (b for Blue and r for Red): ";
	cin >> player_col;

	while(player_col != 'b' && player_col != 'r'){
		cout << "\nIllegal color, chose again (b for Blue and r for Red): ";
		cin >> player_col;
	}

	Hex h(verticeNum);

	srand(time(0));

	Player Human = Player::BLANK;

	Player Computer= Player::BLANK;

	if (player_col == 'b'){

		cout << "Computer is Red" << endl;
		Human = Player::BLUE;
		Computer = Player::RED;

		while (true){

			h.playHuman(Human);
			if (h.Won(Human)){
				cout << "\nCongratulations!! You won." << endl;
				break;
			}
			h.playComputer(Computer);
			if (h.Won(Computer)){
				cout << "\nComputer has won!! Better luck next time." << endl;
				break;
			}
		}

	}

	else if (player_col == 'r'){

		cout << "Computer is Blue" << endl;

		Human = Player::RED;
		Computer = Player::BLUE;

		while (true){

			h.playComputer(Computer);
			if (h.Won(Computer)){
				cout << "\nComputer has won!! Better luck next time." << endl;
				break;
			}
			h.playHuman(Human);
			if (h.Won(Human)){
				cout << "\nCongratulations!! You won." << endl;
				break;
			}
		}

	}

	return 0;
}

