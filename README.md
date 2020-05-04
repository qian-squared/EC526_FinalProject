# Parallel Implementation of Q-Learning  

## Code
### Code structure
- ``q-learning.cpp`` contains major implementations of q-learning algorithm. ``generateMaze.cpp`` generates random sizes of mazes. ``findSolution.cpp`` finds out if any feasible solution is produced by ``q-learning.cpp``.
\
\
![image1](/images/flowchart.png)

### How to run code
- On SCC, excute command ``load module openmpi``.
- Run Makefile and makeMaze to get all excutables.
- First excutes ``makeMaze``, then run ``submit_qlearning_mpi.sh`` to submit ``q-learning`` using 4 processors on SCC. Run ``get_result.sh`` to combine the results and ``findSolution`` to get the optimized path.
- Because the libary we are using in python is not pre-installed on SCC, so ``solution.txt`` and ``maze.txt`` are needed to be download to local, and put under the same directory as ``draw.py``.
- Run ``draw.py`` to get a visualized result.

## Introduction  
### What is Q-Learning  
&nbsp;&nbsp;Q-Learning is a value-based Reinforcement Learning algorithm that deals with the problem of learning to control autonomous agents. The learning process works based on interactions by trial and error with a dynamic environment which provides reward signals for each action the agent executes.  

<img src="/images/procedure.jpg" width="300"></img>  

### A Simple Example on Q-Learning  
  
<img src="/images/simpleMaze.png" width="300"></img>  
  
Here is a 2x3 maze, with a little rat at the starting point, and its goal is to get to the biggest pile of cheese without being poisoned.   
**ways to solve this problem**    
- Brute Force: Time-consuming, not feasible for large size of maze.   
- Randomly Selection: Randomly select a direction to go at each point. Easily stucked into a infinate loop.  
- Q-learning: Train a model to make a best choice at each step.  
  
<img src="/images/Q-Table.png" width="400"></img>  
  
Here is the Q-Table for this maze. We first initialize it with all zeros. At the starting point, which is state zero, if the rat choose to move right, now what the algorithm does is update the q-value of this state and action. But how?   

### Update Function  
  
<img src="/images/equation.png" width="600"></img>
  
Here is the Bellman equation we use to update Q-Table.  
- s: Sate.  
- a: Action.  
- r: Reward.  
- alpha: Learning rate parameter.  
- gamma: Decay rate (future reward discount) parameter.  
  
## Parallelization  
We used OpenMPI to parallel the Q-table. Here is an example showing a 8 x 3 maze. There are 24 states in total. If we are using 4 processors, each maze will be divided into 4 sections and each section will have 6 states in total. The corresponding Q-table is also shown. Taking the red section as an example, besides the Q_table of the states (6 to 11) in this section, two buffers are used: the top buffer and the bottom buffer. The top buffer stores the Q-table information of the row above this section (e.g. state 3 to 5) and the bottom buffer stores that below the section (e.g. state 12 to 14).  

<img src="/images/example_maze.PNG" width="200"></img> <img src="/images/example_qtable.PNG" width="300"></img>  

A we discussed above, Q-leaning uses episodes to train the Q-table. So for each episode, there are four cases to terminate it: 1. The maximum step defined is reached. 2. The end point is reached and it succeeds. 3. The obstacles are encountered and it fails. 4. The boundaries are reached. This stopping condition is new for the parallel implementation because the information of the states outside one processor needs to be obtained from other processors. However, what is the point of parallelization if communication between processors is needed when boundaries are reached? Our solution is to create two buffers and update the buffers once in a while (e.g. every 100 episodes). This means after information is exchanged among processors, this information is used for the next N episodes and will be updated then. This strategy turns out to be working pretty well. The advantages and disadvantages will be discussed later in the conclusion section.

## Results  
The results we obtained are satisfying. As we can see in the figure below, using 1000 episodes, our algorithm successfully solved the 4x4 and the 8x8 maze. The solution path we obtained did avoid all the obstacles and found the shortest path with max award from an arbitrary starting point to the end point. 


<img src="/images/result1.PNG" width="300" title="maze"></img>   <img src="/images/result2.PNG" width="300" title="Q-Table"></img>  

- \- no reward
- o  small reward
- x  trap
- V end of maze

## Conclusion  
By doing this project, we successfully parallelized Q-learning and provided a feasible strategy to parallelize reinforcement learning. However, advantages and disadvantages both exist in this solution. For instance, the parallelization does not seem to improve the calculation time. This points out that there are trade-offs by using parallel programming in q-learning. In our case, the trade off is time needed for communication among processors. The balance point or the “sweet spot” needs to be found to balance the communication and the performance. However, for a large maze and a small number of episodes, parallel q-learning can give a better global view of the maze compared to serial q-learning. For a single episode, serial q-learning will terminate whenever it hits an obstacle. Some parts of the maze may never be reached. Parallel q-learning is exploring many parts of the maze at the same time for just a single episode and thus gives a better chance to view the maze as a whole. Overall, this project gives us a chance to explore the implementation of parallel programming in machine learning and offer us different perspectives viewing some algorithms. We also had lots fun doing this project and it’s a great experience! :)


## References
[Q-Learning](https://www.freecodecamp.org/news/diving-deeper-into-reinforcement-learning-with-q-learning-c18d0db58efe/)
