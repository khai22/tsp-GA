This program is to solved Travelling salesman problem by using genetic algorithm with approach on sequential and parallel program. For this version, we had complete to do;

	1.Sequential approach
	2.OpenMP approach

In this program, we are using randomly generate TSP by using python code and the optimize solution been calculate using concorde TSP solver http://www.math.uwaterloo.ca/tsp/concorde/index.html


To Run the program:

1) Generate a tsp: 
      python tsp-builder.py <# no of cities> <min cost> <max cost>

2) Find the optimal solution (look for it in concorde's output):
      ./concorde tsp.tsp

3) Load the optimal solution into the *tspga-omp.cpp/tspga-seq.cpp setting:
      #define BEST_PATH <optimal solution>

	*	if you want to run the code in openmp change in tspga-omp.cpp
		else if you want to run the TSP in sequential change in tspga-seq.cpp

4) Generate initial paths for the program (make sure the # of paths equals 
   the population size!):
      python tsp-initial-paths.py <# cities> <# of paths to generate>

	  
5) In openmp code, if you want to control the parallelism setting the number of OpenMP threads change line 19 in population-omp.cpp;
	
	int a = <*num of thread you want to use>;

6) Rebuild the code;
	make clean (to clean the make file if you has build it before)
	make

7) Run the program:

	for sequential run; ./sequential

	for openmp run; ./openmp
 
      
      

Shortcuts:

To re-run the program with sequential/openmp: use step 8

To re-run the program with a different number of threads: use steps 5,6,7

To re-run the program on a new TSP: redo all the steps
