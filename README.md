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
- First excutes ``makeMaze``, then run ``q-learning`` and ``findSolution``.
- Because the libary we are using in python is not pre-installed on SCC, so ``solution.txt`` and ``maze.txt`` are needed to be download to local, and put under the same directory as ``draw.py``.
- Run ``draw.py`` to get a visualized result.
## Introduction  
### What is Q-Learning  
&nbsp;&nbsp;Q-Learning is a value-based Reinforcement Learning algorithm that deals with the problem of learning to control autonomous agents. The learning process works based on interactions by trial and error with a dynamic environment which provides reward signals for each action the agent executes.  
![image2](/images/images/procedure.jpg | width=300)
### A Simple Example on Q-Learning  
![image3](/images/images/simpleMaze.png | width=300)
Here is a 2x3 maze, with a little rat at the starting point, and its goal is to get to the biggest pile of cheese without being poisoned.  
**ways to solve this problem**  
- Brute Force: Time-consuming, not feasible for large size of maze.   
- Randomly Selection: Randomly select a direction to go at each point. Easily stucked into a infinate loop.  
- Q-learning: Train a model to make a best choice at each step.  
\
![image4](/images/Q-Table.png | width=400)  
Here is the Q-Table for this maze. We first initialize it with all zeros. At the starting point, which is state zero, if the rat choose to move right, now what the algorithm does is update the q-value of this state and action. But how?   

### Update Function  
![image5](/images/equation.png | width=600)
Here is the Bellman equation we use to update Q-Table.
- s: Sate.  
- a: Action.  
- r: Reward.  
- alpha: Learning rate parameter.  
- gamma: Decay rate (future reward discount) parameter.  

## Parallelization  
## Results  
## References
[Q-Learning](https://www.freecodecamp.org/news/diving-deeper-into-reinforcement-learning-with-q-learning-c18d0db58efe/)
