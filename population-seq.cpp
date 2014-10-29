/*
   File: population.cpp
*/

#include "population.h"
#include <cstdlib>
#include <vector>
#include <iostream>
#include <istream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <assert.h>
#include <list>
#include "explode.h"


using namespace std;
int a = 20;

/*
Constructor: Population

*/
Population::Population(string initial_paths_file, string tsp_data_file,
                       int size, double elitism, double mutation_rate) {
   int city;
   double cost;
   vector<double> line_values;
   set<int> city_pair;

   this->elitism = elitism;
   this->size = size;
   this->initial_paths_file = initial_paths_file;
   this->tsp_data_file = tsp_data_file;
   this->mutation_rate = mutation_rate;

   // Load the cost table and throw away the first line (# of cities)
   ifstream cost_file;
   cost_file.open(tsp_data_file.c_str());
   cost_file >> city;

   // populate the cost table with key/value pairs of (citypair, travel_cost)
   while( cost_file >> city ) {
      city_pair.insert(city);
      cost_file >> city;
      city_pair.insert(city);
      cost_file >> cost;
      this->cost_table[city_pair] = cost;
      city_pair.clear();
   }
   cost_file.close();
}


/*
Function: Avg_Fitness

*/
double Population::Avg_Fitness() {
   double total_fitness = 0;
   int i;

   // Calculate total fitness of the population.
  	
   for(i = 0; i < (int)this->current_individuals.size(); i++) {
      total_fitness +=  this->current_individuals[i].Raw_Fitness();
   }   
   return total_fitness / (double) this->current_individuals.size();
}


/*
Function: Fittest

*/
Individual Population::Fittest() {
   
   // return the most fit individual in the population
   sort(this->current_individuals.begin(), this->current_individuals.end());
   return this->current_individuals.front();
}


/*
Function: Reproduce

*/
void Population::Reproduce() {
   this->Selection();
   this->Breed();
}


/*
Function: Selection

   Pairs up individuals for breeding. The higher an individual's fitness
   the more likely it will be selected to breed.
*/
void Population::Selection() {
   int i=0, j, prev_ind_index;
   bool asexual;
   double total_pop_fitness = 0;
   double breeding_chance, current_chance;
   vector<double> percent_contrib;
   vector< vector<int> > breed_pair;
   vector<Individual>::iterator it;

   // 1) Calculate total fitness of the population.
  
   for(i = 0; i < (int)this->current_individuals.size(); i++) {
      total_pop_fitness += this->current_individuals[i].Raw_Fitness();
   }
   
   // 2) Build lookup table of percent contribution to total.
    for(it = this->current_individuals.begin(); it != this->current_individuals.end(); it++) {
      percent_contrib.push_back((*it).Raw_Fitness() / total_pop_fitness);
   }
   /* 
      3) Make breeder pairs:
         Until we have paired up enough breeders:
            Select using the 'roulette wheel' approach. 'Spin' the roulette
            to get a breeding chance. Then iterate through the individuals,
            summing their percent contribution until we hit the lucky 
            'partition' of the roulette wheel: where the cumulative percent
            contribution is greater than the breeding chance.
            
         We will match up a number of pairs equal to the ceiling of
         (POP SIZE / 2). This is because each breeding pair should, 
         ideally, breed 2 children for a total of POP SIZE children.
   */   
   while(this->breeders.size() < (this->size * 1.5)) { // 110% of pop size...
      
      for(i = 0; i < 2; i++) { // Make a couple.
         current_chance = 0;
         breeding_chance = rand() / (double) (RAND_MAX + 1.0);
         
         // Find the lucky individual.
         for(j = 0; j < (int)percent_contrib.size(); j++) {
            current_chance += percent_contrib[j];
            
            if(current_chance > breeding_chance) { // lucky individual found!
               current_chance = 0;
               break;
            }
         }
         
         // Keep track of indices of selected inds to prevent asexual selection.
         if(i == 0) {
            prev_ind_index = j;
            asexual = false;
         }
         if( (i == 1) && (prev_ind_index == j) ) {
            asexual = true;
         }
      
         // Add the selected individual's path to a breeding pair.
         breed_pair.push_back((this->current_individuals[j]).Chromosome());
      }
      
      // asexual reproduction prohibited
      if(!asexual) {
         vector< vector<int> > breed_pair_copy (breed_pair);
         this->breeders.push_back(breed_pair_copy);
      }
      breed_pair.clear();
   }
}

/*
Function: Breed

   Breeds a population of new individuals from the breeding pairs.
   Includes the following steps:
      1) Crossover
      2) Mutation
      3) Filter repeats
*/
void Population::Breed() {
   int i, size;

   while(this->new_individuals.size() < this->size) {  
     
      // 1) Crossover
      size = this->breeders.size(); 

      for(i = 0; i < size; i++) {
	this->Crossover(this->breeders[i][0], this->breeders[i][1]);
      }
       // 2) and 3)
      this->Mutation();
      this->Filter();
   }
   this->breeders.clear();
}

/*
Function: Crossover

   Breed the pair to generate a child.
   
   Uses the Greedy Subtour Crossover approach, developed by Sengoku
   and Yoshihara:
      Sengoku H., Yoshihara I., A Fast TSP Solver Using GA on JAVA, 
      Proc. of the 3rd AROB, 1998, pp. 283-288.
   
*/
void Population::Crossover(vector<int> parent_a, vector<int> parent_b) {

   list<int> child;
   vector<int> child_vec;
   list<int> cities_remaining;
   vector<int>::iterator it;
   list<int>::iterator itl;
   int num_cities;
   int random_city, random_city_i, p_a_index, p_b_index;
   bool get_from_a, get_from_b;

   get_from_a = get_from_b = true;
   num_cities = parent_a.size();

   // We'll update the remaining cities as we add them to the child.   
   for(int i = 1; i <= num_cities; i++) {
      cities_remaining.push_back(i);
   }

   // Choose the first city at random and add it to the child.
   random_city = ((rand() % num_cities) + 1);
   it = find( parent_a.begin(), parent_a.end(), random_city);
   
   random_city = *it;
   child.push_back(random_city);
   cities_remaining.remove(random_city);

   // Set indices for p_a_index, p_b_index to be the index of the first city.
   for(unsigned int i = 0; i < parent_a.size(); i++) {
      if(parent_a[i] == random_city) {
         p_a_index = i;
      }
      if(parent_b[i] == random_city) {
         p_b_index = i;
      }
   }

   /* 
      Until both of the parents have a collision:
         1) Grab a city from parent a, working backwards

         2) Grab a city from parent b, working forwards
      
      A collision is when the city we'd grab is already in the child.
      
      See the paper on Greedy Subtour Crossover for a better
      explanation of what is going on.
   */
   while(get_from_a || get_from_b) {
      
      if(get_from_a) {
         p_a_index--; // working backwards...
         // if out of bounds
         if(p_a_index < 0 || p_a_index > num_cities - 1) {
            get_from_a = false;
         }
         else {      
            // Check whether the city is already present in the child.
	    itl = std::find(child.begin(), child.end(), parent_a[p_a_index]);
            if(itl == child.end()) { // then the city is not yet in the child.
               
               child.push_front(parent_a[p_a_index]); // Add to front!
               cities_remaining.remove(parent_a[p_a_index]);
            }
            else {
               get_from_a = false;
            }
         }
      }
      
      if(get_from_b) {
         p_b_index++; // working forwards...

         if(p_b_index < 0 || p_b_index > num_cities - 1) { // then we're out of bounds
            get_from_b = false;
         }
         else {

            // Check whether the city is already present in the child.         
	    itl = std::find(child.begin(), child.end(), parent_b[p_b_index]);
            if(itl == child.end()) { // then the city is not yet in the child.
               
               child.push_back(parent_b[p_b_index]); // Add to end!
	       cities_remaining.remove(parent_b[p_b_index]);
            }
            else {
               get_from_b = false;
            }
         }
      }
   }

   // Add the remaining cities to the end of the child at random.
   while(cities_remaining.size() > 0) {
      random_city_i = rand() % cities_remaining.size();
      itl = cities_remaining.begin();
      for(int i = 0; i < random_city_i; i++) {
         itl++;
      }
      child.push_back(*itl);
      cities_remaining.remove(*itl);
   }
      
   // add child to new_individuals
   for(itl = child.begin(); itl != child.end(); itl++) {
      child_vec.push_back(*itl);
   }


     if((int)child_vec.size() == num_cities)
     {
      Individual new_individual (child_vec);
      this->new_individuals.push_back(new_individual);
     }
   
}

/*
   Mutate the individuals in the population with a specified rate.
*/
void Population::Mutation() {
   double mutation_chance;
   vector<Individual>::iterator it;
   
   for(it = this->new_individuals.begin(); it != this->new_individuals.end(); it++) {
      mutation_chance = rand() / (double) (RAND_MAX + 1.0);
      if(this->mutation_rate < mutation_chance)
         (*it).Mutate();
   }
}

/*
   Filter repeats: avoid wasting resources by filtering 
   out those new individuals that have already been evaluated
   in some prior generation.
*/
void Population::Filter() {
   vector<Individual>::iterator iter;
   iter = this->new_individuals.begin();
   
   while(iter != this->new_individuals.end()) {
      if(this->uniques.find(iter->Chromosome()) != this->uniques.end()) { // repeat!   
         
         // Do the erase, keep the new, valid, already incremented (by erase!) iterator
         iter = this->new_individuals.erase(iter);
      }
      else {
         // manually increment our iterator
         iter++;
      }
   }
}

/*
   Merge the current population and the population of freshly evaluated
   individuals into the next generation of the current population:
      1) Choose the best <elitism>% members of the current population
      2) Fill the remaining slots with the best members of the
         union of the current and new populations.
   
   Elitism ensures that a portion of the best performers from the current
   population live to breed again.
*/
void Population::Merge(bool init) {
   vector<Individual> next_individuals;
   vector<Individual> unused_individuals;
   vector<Individual>::iterator itI;
   
   // The very first merge is not very complicated.
   if(init) {
      for(itI = this->new_individuals.begin(); 
          itI != this->new_individuals.end(); itI++) {
      
         Individual new_ind = *itI;
         this->current_individuals.push_back(new_ind);
      }
   }
   
   // Standard merge for all generations other than the very first.
   else {
      // pick the best <elitism>% members of old population
      int i, elite_count = (int)floor((elitism * size) + 0.5);

      sort(this->current_individuals.rbegin(), this->current_individuals.rend());

      for(i = 0; i < elite_count; i++) {
         next_individuals.push_back(this->current_individuals.back());
         this->current_individuals.pop_back();
      }

      // pool old and new populations and sort
      unused_individuals.insert(unused_individuals.end(),
                                this->new_individuals.begin(),
                                this->new_individuals.end());
      unused_individuals.insert(unused_individuals.end(),
                                this->current_individuals.begin(),
                                this->current_individuals.end());

      sort(unused_individuals.rbegin(), unused_individuals.rend());

      //  fill remaining slots with the best of old and new populations
      int non_elite_count = size - elite_count;
      for(i = 0; i < non_elite_count; i++) {
         next_individuals.push_back(unused_individuals.back());
         unused_individuals.pop_back();
      }
      this->current_individuals = next_individuals;
   }
   this->new_individuals.clear();
}

void Population::Genesis() {
   string line_in;
   vector<int> path;
   ifstream path_file_stream;
   path_file_stream.open(this->initial_paths_file.c_str());
   
   // create individuals with initial paths
   for(unsigned int i = 0; i < this->size; i++) {
      getline(path_file_stream, line_in);
      path = int_explode(" ", line_in);
      this->new_individuals.push_back(Individual(path));
   }
   path_file_stream.close();
}

void Population::Evaluate() {
   vector<int> temp_chromosome;
   set<int> city_pair;
   int i, j;
   double fitness_sum = 0;
   
   // for each indiviual in the new population

   for(i = 0; i < (int)new_individuals.size(); i++) {
      temp_chromosome = new_individuals[i].Chromosome();
      // sum the cost for each adjacent city-pair in an indiviual's chromsome

      for(j = 0; j < (int)temp_chromosome.size() - 1; j++) {
         city_pair.insert(temp_chromosome[j]);
         city_pair.insert(temp_chromosome[j+1]);
         fitness_sum += this->cost_table[city_pair];
         city_pair.clear();
      }

      // Take into consideration the cost to return to the destination
      city_pair.insert(temp_chromosome[0]);
      city_pair.insert(temp_chromosome[temp_chromosome.size()-1]);
      fitness_sum += this->cost_table[city_pair];
      city_pair.clear();

      new_individuals[i].Raw_Fitness(fitness_sum);
      // reset the sum for the next individual
      fitness_sum = 0;
      
      // add an entry in the uniques hash
     
      uniques[new_individuals[i].Chromosome()] = 1;
      
   } 
}

/*
   Used for algorithms that need to compare individuals.
*/
bool operator<(const Individual& lhs, const Individual& rhs) {
  return ((Individual)lhs).Raw_Fitness() < ((Individual)rhs).Raw_Fitness();
}
