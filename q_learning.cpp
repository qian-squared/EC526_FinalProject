#include<iostream>
#include<cmath>
#include<ctime>
#include<vector>
#include "float.h"
#include <mpi.h>
#include <chrono>
#include <fstream>
#include <string>
#include <sstream>


using namespace std;


#define ACTIONS 4
#define TOTAL_EPISODES 1000
#define MAX_STEP 20
#define MAX_REWARD 100
#define MIN_REWARD -10

#define REWARDS_ROWS 8
#define REWARDS_COLS 8
int STATES = REWARDS_COLS * REWARDS_ROWS;

//const double rewards[REWARDS_ROWS][REWARDS_COLS] = {{-1, 0, -1}, {0, -10, 10},{0, -1, 0},{0, -1, -1}};
double** rewards;
const double gamma_q = 0.95;
const double lr = 0.8;
double epsilon = 1;
const int actions[4] = {0, 1, 2, 3};  // left, right, up, down
// const string actionWords[4] = {"left", "right", "up", "down"};

void qUpdate(int currentPos, int nextPos, double **qTable, int action, double reward);
void epsilonUpdate(double &epsilon);
int getAction(int currentPos, bool isExplore, double **qTable);
bool checkPosition(int nextPos);
void epsilonUpdate(double &epsilon);
int makeMove(int nextPos, int action);
void updateBUffer(double **qTable);

int world_size; // number of processes
int my_rank; // my process number
int local_size;
//int success_state = 5;
//int fail_state = 4;
double** top_buffer;
double** bottom_buffer; 
double **qTable;


int main(int argc, char** argv) {


	// Initialize MPI
	MPI_Init(&argc, &argv);

	// Get the number of processes
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	// Get the rank
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

   	/*
   	world_size = 1;
   	my_rank = 0;
   	*/
	/*
   	rewards = new double* [REWARDS_ROWS];
   	for (int i = 0; i<REWARDS_ROWS; i++){
   		rewards[i] = new double[REWARDS_COLS];
		for (int j = 0; j<REWARDS_COLS; j++){
			rewards[i][j] = -1.0;
		}
	}
	*/
	

	rewards = new double* [REWARDS_ROWS];

	ifstream input;
	input.open("rewards.txt");
	string row;
	for (int i =0; i < REWARDS_ROWS; i++){
		getline(input, row);
		stringstream ss(row);
		rewards[i] = new double[REWARDS_COLS];
		string a;
		for (int j = 0; j < REWARDS_COLS; j++){
			ss >> a;
			rewards[i][j] = stod(a);
		}
	}
	cout << "Done reading from file!" << endl;
	//input.close();
	

 	local_size = STATES/world_size;

   	if (my_rank == (world_size-1)) { local_size += (STATES % world_size) ; }

	// initialize Q-Table
		
	qTable = new double *[local_size];
	for (int i = 0; i < local_size; i++) {
		qTable[i] = new double[ACTIONS];
		for (int j = 0; j < ACTIONS; j++) 
			qTable[i][j] = -20;
	}

	// initilize buffer
	top_buffer = new double* [REWARDS_COLS];
	bottom_buffer = new double* [REWARDS_COLS];
	for (int i=0; i < REWARDS_COLS; i++) {
		top_buffer[i] = new double [ACTIONS];
		bottom_buffer[i] = new double [ACTIONS];
		for (int j = 0; j<ACTIONS; j++) {
			top_buffer[i][j] = 0.0;
			bottom_buffer[i][j] = 0.0;
		}
	}



	/*
	// check Q-Table
	printf("Q-Table:\n");
	for (int i = 0; i < local_size; i++) {
		for (int j = 0; j < ACTIONS; j++) 
			cout << qTable[i][j] << " ";
		cout << endl;
	}
	*/

	printf("-----------------------------\n");
	srand((signed)time(NULL));


 	std::chrono::time_point<std::chrono::steady_clock> begin_time = 
      std::chrono::steady_clock::now();

	for (int episode = 0; episode < TOTAL_EPISODES; episode++) {

		// cout << "rank "<< my_rank << " episode "<< episode << endl; 

		
		if (episode % 100 == 0){
			//MPI_Barrier(MPI_COMM_WORLD);
			updateBUffer(qTable);
			//cout << "done update buffer" << endl;
			//MPI_Barrier(MPI_COMM_WORLD);
		}

		// initial random position of rat
		int currentPos = rand() % (local_size);
		int currentState = my_rank * (STATES / world_size) + currentPos; 
		double r = rewards[currentState/REWARDS_COLS][currentState%REWARDS_COLS];
		if (r == MAX_REWARD || r == MIN_REWARD)
			continue;
		//printf("current position: %d \n", currentState);
		//pair<int, int> currentPos(0, 1);

		bool done = false;
		// printf("start position: %d, %d\n", currentPos.first, currentPos.second);
		for (int step = 0; step < MAX_STEP; step++) {

			double a = rand() % (999 + 1) / (double)(999 + 1);
			// double a = 0.9;
			// printf("using a: %.3f\n", a);
			int action;
			int nextState;	
			double reward;		
			// printf("current position: %d, %d\n", currentPos.first, currentPos.second);
			// do exploitation
			bool mode = a > epsilon ? false : true; // do exploitation or do exploration	

			action = getAction(currentState, mode, qTable);
			//action = 0;
			//printf("Episode %d, step %d, action is %d\n", episode, step, action);
			nextState = makeMove(currentState, action);
			//nextState = currentState+1;
			//printf("next position: %d \n", nextState);
			done = checkPosition(nextState);
			//cout << "done is: " << done << endl;
	
			// update Q-Table
			// printf("reward of this action: %.3f\n", rewards[nextPos.first][nextPos.second]);
			reward = rewards[nextState/REWARDS_COLS][nextState%REWARDS_COLS];
			qUpdate(currentState, nextState, qTable, action, reward);
			//cout << my_rank << " done q update" << endl;

			currentState = nextState;

			if (done)
				break;

		}		
		// printf("------------------------------\n");
		// update epsilon
		epsilonUpdate(epsilon);
		//cout << my_rank << " rank on episode" << episode << endl;

	}

	  // TIMING LINE 2: Get the ending timestamp.
   std::chrono::time_point<std::chrono::steady_clock> end_time =
      std::chrono::steady_clock::now();

   // TIMING LINE 3: Compute the difference.
   std::chrono::duration<double> difference_in_time = end_time - begin_time;

   // TIMING LINE 4: Get the difference in seconds.
   double difference_in_seconds = difference_in_time.count();

   // Print the time.
   
   if (my_rank == 0) {
      printf("For maze with state %d , it took %.10f seconds.\n", STATES, difference_in_seconds);
   }

   // print result

	ofstream output;
	if (my_rank == 0)
		output.open("result0.txt");
	else if (my_rank == 1)
		output.open("result1.txt");
	else if (my_rank == 2)
		output.open("result2.txt");
	else if (my_rank == 3)
		output.open("result3.txt");

	// print result
	//if (my_rank == 0)
		//printf("    left   right    up      down\n");

	printf("my_rank is %d \n", my_rank);
	for (int i = 0; i < local_size; i++) {
		int num = my_rank * (STATES / world_size) + i;
		printf("%d ", num);
		output << num << " ";
		for (int j = 0; j < ACTIONS; j++) {
			printf("%7.3f ", qTable[i][j]);
			output << qTable[i][j] << " ";
		}
		cout << endl;
		output << endl;
	}

	
	// Clean up
	MPI_Barrier(MPI_COMM_WORLD);
   	MPI_Finalize();

	
	// clear up
	for (int i = 0; i < local_size; i++)
		delete[] qTable[i];
	delete[] qTable;

	input.close();
	output.close();

	
	return 0;
}

// make movement
int makeMove(int currentState, int action) {
	int nextState = currentState;
	if (action == 0) // left
		nextState -= 1;
	if (action == 1) // right 
		nextState += 1;
	if (action == 2) // up
		nextState -= REWARDS_COLS;
	if (action == 3) // down 
		nextState += REWARDS_COLS;
	
	return nextState;	
}

// get valid action
int getAction(int currentState, bool isExplore, double **qTable) {

	int action;
	vector<int> avaialbe_action;

	if (currentState / REWARDS_COLS != 0) // not top row
		avaialbe_action.push_back(2); // go up
	if (currentState / REWARDS_COLS != REWARDS_ROWS-1) // not bottom row
		avaialbe_action.push_back(3); // go down
	if (currentState % REWARDS_COLS !=0 ) // not left column
		avaialbe_action.push_back(0); // go left
	if (currentState % REWARDS_COLS != REWARDS_COLS-1 ) // not right column
		avaialbe_action.push_back(1); // go right


	// Exploration Mode
	if (isExplore) {
		action = avaialbe_action[rand() % avaialbe_action.size()];
	}
	// Exploitation Mode
	else {		
		int index;
		index = currentState % local_size; 
		//if (my_rank == (world_size-1)) { index = currentState - (world_size-1)*(STATES/world_size);}

		double tmp = -DBL_MAX; 
		for (auto pm :actions) {			
			if (qTable[index][pm] > tmp) {
				tmp = qTable[index][pm];
				action = pm;
			}
		}
	}	
	return action;		
}

// update Q-Table
void qUpdate(int currentState, int nextState, double **qTable, int action, double reward) {
	int currentIndex = currentState % local_size;
	// printf("currentPos is: %d, %d, index is %d\n", currentPos.first, currentPos.second, currentIndex);
	
	int nextIndex = nextState % local_size;
	// printf("nextPos is: %d, %d, index is %d\n", nextPos.first, nextPos.second, nextIndex);

	// 2 buffer for top and down needed here

	double* nextState_qt = new double[ACTIONS];

	int lower_bound = my_rank * (STATES / world_size);
	//cout << "lower bound is " << lower_bound << endl;
	if (nextState < lower_bound){
		nextState_qt = top_buffer[nextIndex % REWARDS_COLS]; // read from top bufer
	} else if (nextState >= lower_bound + local_size){
		nextState_qt = bottom_buffer[nextIndex % REWARDS_COLS]; // read from bottom bufer
	} else 
		nextState_qt = qTable[nextIndex];

	double maxValue = DBL_MIN;
	
	for (int i = 0; i < ACTIONS; i++) {
		if (nextState_qt[i] > maxValue)
			maxValue = nextState_qt[i];		
	}

	//cout << "current index is " << currentIndex << endl;
	//cout << "current action is " << action << endl;
	//cout << "q table is " << qTable[currentIndex][action] << endl;

	double tmp = qTable[currentIndex][action] + lr*(reward + gamma_q*maxValue - qTable[currentIndex][action]);
	/*
	printf("qTable[%d][%d] = %.2f + %.2f * (%.2f + %.2f * %.2f - %.2f) \n", currentIndex, action,
			qTable[currentIndex][action], lr, reward, gamma_q, maxValue, qTable[currentIndex][action]);
	*/
	
	//cout << "tmp is " << tmp << endl;

	qTable[currentIndex][action] = tmp;

	//delete[] nextState_qt;
}

// check if current position is valid
bool checkPosition(int nextState) {
	bool done = false;
	//cout << "checking position" << endl;

	// check if gets to the end 
	double reward = rewards[nextState/REWARDS_COLS][nextState % REWARDS_COLS];
	//cout << "reward is " << reward << endl;
	if (reward == MAX_REWARD || reward == MIN_REWARD) {
		/*
		if (reward == MAX_REWARD)
			cout << "Got it!" << endl; 
		else
			cout << "Fail!" << endl;
		*/
		done = true;
	}

	// check if outofbound
	int lower_bound = my_rank * local_size;
	//cout << "lower bound is " << lower_bound << endl;
	if (nextState < lower_bound || nextState >= lower_bound + local_size)
		done = true;

	return done;
}

// linear fit
void epsilonUpdate(double &epsilon) {
	epsilon -= 1.0 / TOTAL_EPISODES;
}


void updateBUffer(double **qTable){

 	MPI_Request request[4];
  	MPI_Status status[4];
  	int requests;
  	requests = 0;
  	int size_buffer = ACTIONS*REWARDS_COLS;
  	//int size_buffer = ACTIONS;

  	double* top_sent = new double[size_buffer];
  	double* bottom_sent = new double[size_buffer];
  	double* top_received = new double[size_buffer];
  	double* bottom_received = new double[size_buffer];
  	int i , j;
  	int count = 0;
  	
  
  	for (i = 0, j = local_size - REWARDS_COLS; i < REWARDS_COLS; i++, j++){
  		for (int k = 0; k < ACTIONS; k++){
  			top_sent[count] = qTable[i][k];
  			bottom_sent[count] = qTable[j][k];
  			count++;
  		}
	} 


  	// Fill the top buffer. Send to the bottom, listen from the top.
  //printf("I am rank %d of %d and I am receicing from the top.\n", my_rank, world_size);
   
   MPI_Isend(&bottom_sent[0], size_buffer , MPI_DOUBLE, (my_rank+1)%world_size, 1, MPI_COMM_WORLD, request + requests++);
   MPI_Irecv(&top_received[0], size_buffer, MPI_DOUBLE, (my_rank+world_size-1)%world_size, 1, MPI_COMM_WORLD, request + requests++);

   //printf("I am rank %d of %d and I received from the top.\n", my_rank, world_size);

   // Fill the bottom buffer. Send to the top, listen from the bottom.
   //printf("I am rank %d of %d and I am receicing from the bottom.\n", my_rank, world_size);

   MPI_Isend(&top_sent[0], size_buffer , MPI_DOUBLE, (my_rank+world_size-1)%world_size, 0, MPI_COMM_WORLD, request + requests++);
   MPI_Irecv(&bottom_received[0], size_buffer, MPI_DOUBLE, (my_rank+1)%world_size, 0, MPI_COMM_WORLD, request + requests++);
   
   //printf("I am rank %d of %d and I received from the bottom.\n", my_rank, world_size);
	
	//printf("waiting all \n");

	MPI_Waitall ( requests, request, status );


	//printf("updating \n");
	count = 0;
 	for (i = 0; i < REWARDS_COLS; i++){
  		for (int k = 0; k < ACTIONS; k++){
  			top_buffer[i][k] = top_received[count];
  			bottom_buffer[i][k] = bottom_received[count];
  			count++;
  		}
	} 

	//printf("done update \n");

 
	delete[] top_sent;
	delete[] top_received;
	delete[] bottom_sent;
	delete[] bottom_received;

	
	//MPI_Barrier(MPI_COMM_WORLD);
	
	//printf("done wait \n");

	//MPI_Barrier(MPI_COMM_WORLD);
	//printf("rank done update \n");

}