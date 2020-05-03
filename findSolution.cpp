#include<iostream>
#include<cmath>
#include<ctime>
#include<vector>
#include<algorithm>
#include<fstream>
#include<sstream>
#include<exception>
#include "float.h"

#define REWARDS_ROWS 8
#define REWARDS_COLS 8
#define TOTAL_EPISODES 1000
#define MAX_STEP 10
int STATES = REWARDS_ROWS * REWARDS_COLS;
#define ACTIONS 4

using namespace std;

const int actions[4] = {0, 1, 2, 3};

void genMaze(int col, int row, double diffLevel, double** rewards);
void printMaze(double **rewards);
void printRewards(double **rewards);
void writeMaze(double **rewards);
void writeRewards(double **rewards);
void drawBestSolution(double **qTable, int currentState, vector<pair<int, int> > &results);
void readqTable(double **qTable);


int main() {

	double **rewards;	
	rewards = new double *[REWARDS_ROWS];
	for (int i = 0; i < REWARDS_ROWS; i++)
		rewards[i] = new double [REWARDS_COLS];
	
	 /*
	genMaze(REWARDS_COLS, REWARDS_ROWS, 0.05, rewards);
	printRewards(rewards);
	printMaze(rewards);
	writeRewards(rewards);
	writeMaze(rewards);
	 */
	
	 
	vector<pair<int, int> > results;
	
	double **qTable;
	qTable = new double *[STATES];
	for (int i = 0; i < STATES; i++) {
		qTable[i] = new double[ACTIONS];
	}
	
	readqTable(qTable);
	
	//for (int i = 1000; i < STATES; i++){
		//cout << i << endl;
		drawBestSolution(qTable, 0, results);
	//}
	ofstream ofile("solution.txt");
	for (auto res : results) {
		ofile << res.first << " " << res.second << endl;
	}

	ofile.close();
	
}


// different Level ranges in 0.1-0.3
void genMaze(int col, int row, double diffLevel, double** rewards) {
	srand((unsigned)time(NULL));
	// doulsble rewards[col][row];
	int obstacleNum = int(col * row * diffLevel);
	if (obstacleNum < 1)
		obstacleNum = 1;
	// int bigRewardNum = 1;
	// int smallRewardNum = col * row / 3;	
	// int noneRewardNum = col * row - obstacleNum - smallRewardNum;		
	
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			double a = rand() % (999 + 1) / (float)(999 + 1);
			if (a > 0.5) 
				rewards[i][j] = 0.0;
			else
				rewards[i][j] = -1.0;				
		}
	}
	
	for (int i = 0; i < obstacleNum; i++) {
		vector<pair<int, int> >::iterator findResult;
		vector<pair<int, int> > is_set;
		pair<int, int> randomPair;
		do {
			randomPair.first = rand() % row;
			randomPair.second = rand() % col;
			findResult = find(is_set.begin(), is_set.end(), randomPair);
		} while (findResult != is_set.end());
		rewards[randomPair.first][randomPair.second] = -10.0;
		is_set.push_back(randomPair);
	}
	
	rewards[0][0] = -1.0;
	rewards[row-1][col-1] = 10.0;
}

void writeMaze(double **rewards) {
	ofstream ofile("maze.txt");
	for (int i = 0; i < REWARDS_ROWS; i++) {
		for (int j = 0; j < REWARDS_COLS; j++) 
		if (rewards[i][j] == 0)				
				ofile << "o";					
			else if (rewards[i][j] == -10)
				ofile << "x";
			else if (rewards[i][j] == 10)
				ofile << "V";
			else
				ofile << "-";								
		ofile << endl;
	}
}


void printMaze(double **rewards) {	
	for (int i = 0; i < REWARDS_ROWS; i++) {
		for (int j = 0; j < REWARDS_COLS; j++) 
			if (rewards[i][j] == 0)
				cout << "o ";
			else if (rewards[i][j] == -10)
				cout << "x ";
			else if (rewards[i][j] == 10) 
				cout << "V ";
			else
				cout << "- ";		
		cout << endl;
	}
}

void writeRewards(double **rewards) {
	ofstream ofile("rewards.txt");
	for (int i = 0; i < REWARDS_ROWS; i++) {
		for (int j = 0; j < REWARDS_COLS; j++) 
			ofile << rewards[i][j] << " ";			
		ofile << endl;
	}	
}

void printRewards(double **rewards) {	
	for (int i = 0; i < REWARDS_ROWS; i++) {
		for (int j = 0; j < REWARDS_COLS; j++)
			printf("%3.1f ", rewards[i][j]);
		cout << endl;
	}	
}


int getAction2(double **qTable, int currentState) {
	double maxValue = -DBL_MAX;
	int action;
	vector<int> possible_action;
	for (int i = 0; i < ACTIONS; i++) {
		if (qTable[currentState][i] > maxValue) {
			maxValue = qTable[currentState][i];
			possible_action.clear();
			possible_action.push_back(i);
		} else if (qTable[currentState][i] == maxValue) {
			possible_action.push_back(i);
		}

		action = possible_action[rand()%possible_action.size()];
		
	}
	return action;
}

void drawBestSolution(double **qTable, int currentState, vector<pair<int, int> > &results) {
	int count = 0;
	do{

		int action = getAction2(qTable, currentState);
		int nextState;
		if (action == 0)
			nextState = currentState - 1;
		if (action == 1)
			nextState = currentState + 1;
		if (action == 2)
			nextState = currentState - REWARDS_COLS;
		if (action == 3)
			nextState = currentState + REWARDS_COLS;
		pair<int, int> pos(currentState/REWARDS_COLS, currentState%REWARDS_COLS);
		results.push_back(pos);
		printf("current posX is %d, posY is %d\n", pos.first, pos.second);
		//printf("current state is %d \n", currentState);
	
		currentState = nextState;

		count ++;
		if (count >= 200){
			cout << "no solution" << endl;
			break;
		}
	} while(currentState != STATES - 1);

	pair<int, int> pos(currentState/REWARDS_COLS, currentState%REWARDS_COLS);
	results.push_back(pos);
	printf("current posX is %d, posY is %d\n", pos.first, pos.second);
}

void readqTable(double **qTable) {
	ifstream input;
	input.open("result.txt");
	
	string row;
	for (int i = 0; i < STATES; i++){
		getline(input, row);
		stringstream ss(row);	  
		string a, index;
		ss >> index;
		for (int j = 0; j < ACTIONS; j++){
		   ss >> a;
		   double tmp = 0.0;
		   try { tmp = stod(a);
		   	throw 1;
		   } catch(int e){
		   }
		   //cout << index << endl;
		   qTable[stoi(index)][j] = tmp;
		}
	}	
}
	



