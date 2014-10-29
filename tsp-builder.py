"""
   File: tsp-initial-paths.py
   
   Description: Generates a cost table for a tsp given:
      a) number of cities in the tsp
      b) minimum desired cost
      c) maximum desired cost
      
   Usage: python tsp-builder.py <# cities> <min cost> <max cost>
   
   The cost table is written to a file specified in this script.

   GA File Format:
   # cities
   cityA cityB cost
   cityA cityC cost
   ...
   
   Concorde File Format:
   ...
   Concorde Header
   ...
   Edge weights
   
   The costs are integers randomly generated in the range min cost to max cost.

"""
import sys
import random
import itertools

out_fname = "tsp.dat"   # TSP for the GA
c_out_fname = "tsp.tsp" # TSP for concorde

"""
   For the GA:
   0) Write the number of cities to a file.
   1) Generate all possible combinations of city pairings.
   2) Write pairings to a file, with a random cost per pairing.
   
   For Concorde TSP Solver:
   0) write the header
   1) write the edge weights
"""
def main():
   if len(sys.argv) != 4:
      print "Usage: python tsp-builder.py <# cities> <min cost> <max cost>"
      sys.exit()
   
   # Setup
   num_cities = int(sys.argv[1])
   min_cost = int(sys.argv[2])
   max_cost = int(sys.argv[3])
   global out_fname
   outfile = open(out_fname, "w")
   c_outfile = open(c_out_fname, "w")
   
   # GA Header
   outfile.write(str(num_cities) + "\n")
   
   # Concorde Header
   c_outfile.write("TYPE : TSP\n")
   c_outfile.write("DIMENSION : %d\n" % num_cities)
   c_outfile.write("EDGE_WEIGHT_TYPE : EXPLICIT\n")
   c_outfile.write("EDGE_WEIGHT_FORMAT : UPPER_ROW\n") 
   c_outfile.write("EDGE_WEIGHT_SECTION :\n")
   
   # Generate pairings, write them to the output files.
   for pairing in itertools.combinations(range(0, num_cities), 2):
      cost = int(random.uniform(min_cost, max_cost))
      c_outfile.write(str(cost) + "\n")
      outfile.write(str(pairing[0]+1) + " " + str(pairing[1]+1) + " " + str(cost) + "\n")

   print "Successfully wrote " + out_fname
   print "Successfully wrote " + c_out_fname

if __name__ == "__main__":
   main()
