# Parallel Implementation of Q-Learning  
## Code structure
- ``q-learning.cpp`` contains major implementations of q-learning algorithm. ``generateMaze.cpp`` generates random sizes of mazes. ``findSolution.cpp`` finds out if any feasible solution is produced by ``q-learning.cpp``.
\
\
![flowchart]<center><img src="/images/flowchart.png"></center>
## How to run code
- On SCC, excute command ``load module openmpi``.
- Run Makefile and makeMaze to get all excutables.
- First excutes ``makeMaze``, then run ``q-learning`` and ``findSolution``.
- Because the libary we are using in python is not pre-installed on SCC, so ``solution.txt`` and ``maze.txt`` are needed to be download to local, and put under the same directory as ``draw.py``.
- Run ``draw.py`` to get a visualized result.
## Introduction  
### What is Q-Learning  
&nbsp;&nbsp;Q-Learning is a value-based Reinforcement Learning algorithm that deals with the problem of learning to control autonomous agents. The learning process works based on interactions by trial and error with a dynamic environment which provides reward signals for each action the agent executes.  
![image of a simple maze]<center><img src="/images/simpleMaze.png"  width="300"></center>
![image of procedure]<center><img src="/images/procedure.jpg"  width="400"></center>
![image of Q-Table]<center><img src="/images/Q-Table.png"  width="400"></center>
### Update Function  
![equation]<center><img src="/images/equation.png"  width="600"></center>
## Parallelization  
## Results  
## References
[Q-Learning](https://www.freecodecamp.org/news/diving-deeper-into-reinforcement-learning-with-q-learning-c18d0db58efe/)
