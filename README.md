# tec-GA
tec1 exploring genetic algorithms in hw/sw



"GA's, guess, evolve and improve fitness". Breeding, Mutation, Crossover and Fitness test

"Genetic algorithms simulate the process of natural selection which means those species who can adapt to changes in their environment are able to survive and reproduce and go to next generation. In simple words, they simulate `survival of the fittest` among individual of consecutive generation for solving a problem." [1]
Therefore Genetic algorithms are a type of optimization algorithm that can be used to help solve programming problems. They work by simulating the process of natural selection, where the fittest solutions are more likely to survive and reproduce. Over time, this can help to find the best solution to a problem.


## uses for Genetic algorithms
- to optimize complex algorithms.
1. Break the algorithm down into smaller, more manageable pieces.
2. Identify the most time-consuming parts of the algorithm and focus on optimizing those.
3. Use profiling tools to identify bottlenecks and areas for improvement.
4. Experiment with different optimization techniques and compare the results.
5. Refactor the code to improve readability and maintainability.
- to find the best possible solution to a problem.
- to optimize machine learning algorithms.
- to improve the accuracy of predictions made by artificial neural networks.
- to find the shortest path between two points.
There are many different algorithms for finding the shortest path between two points, such as Dijkstra's algorithm, A*, and the Floyd-Warshall algorithm. But they are not GAs. see wiki.
- to schedule tasks in a way that minimizes overall cost.
- to design efficient algorithms for solving difficult problems.- to automatically generate programs that solve specific problems.
- to generate new and innovative solutions to problems.
- to optimize existing solutions to problems.

## pathfinding algorithm
find the shortest path between two points
by simulating the process of natural selection. The algorithm will start with a set of potential paths and then evaluate each path based on how well it performs. The fittest paths will be more likely to survive and reproduce, and over time this can help to find the best solution to the problem. code example `xy.c`. test in https://www.onlinegdb.com/online_c_compiler


## genetic programming 
in MINT [2] [3]


## Dijkstra's algorithm 
is a pathfinding algorithm that finds the shortest path between two nodes in a graph. It is a greedy algorithm, meaning that it always chooses the path that appears to be the best at the time, without considering the long-term effects of its choices.

## Floyd-Warshall 
algorithm is an algorithm for finding the shortest paths between all pairs of nodes in a graph. It is a dynamic programming algorithm, meaning that it breaks the problem down into smaller subproblems and solves each one in turn.

## A* is a pathfinding algorithm 
that finds the shortest path between two nodes in a graph. It is a heuristic algorithm, meaning that it uses a heuristic to guide its search. The most common heuristic for A* is the Manhattan distance (the sliding-tile puzzles is called Manhattan distance. It is computed by counting the number of moves along the grid that each tile is displaced from its goal position, and summing these values over all tiles), which calculates the distance between two points in a grid.



## Ref
- [1] https://www.geeksforgeeks.org/genetic-algorithms/
- [2] https://www.cs.ucdavis.edu/~vemuri/classes/ecs271/The%20GP%20Tutorial.htm
- [3] https://en.wikipedia.org/wiki/Genetic_programming
