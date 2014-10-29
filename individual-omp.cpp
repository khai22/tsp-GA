/*
   File: individual.cpp
*/

#include <algorithm>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <vector>
#include "individual.h"

using namespace std;

   Individual::Individual(vector<int> path) : chromosome(path), raw_fitness(0) { }
   
   double Individual::Raw_Fitness() { return this->raw_fitness; }

   void Individual::Raw_Fitness(double value) { this->raw_fitness = value; }
   
   vector<int> Individual::Chromosome() { return this->chromosome; }
   
   /*
      Selects two cities at random and exchanges them.
   */
   void Individual::Mutate() {
      int city1, city2;
      // Generate two random indices in { 0, ..., length(chromosome) }
      city1 = rand() % this->chromosome.size();
      city2 = rand() % this->chromosome.size();
      
      // Ensure the cities are unique.
      while(city1 == city2)
         city2 = rand() % this->chromosome.size();
      swap(this->chromosome[city1], this->chromosome[city2]);
   }
   
   void Individual::Print() {
      for(vector<int>::iterator iter = this->chromosome.begin();
          iter != this->chromosome.end(); ++iter) {
          fprintf(stderr, "%d ", *iter);
      }
      fprintf(stderr, "\n");
      fprintf(stderr, "%.0f\n", this->raw_fitness);
   }

