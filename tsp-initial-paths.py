"""
   File: tsp-initial-paths
   
   Description: Generate a set of random initial paths.
   
   Usage: python tsp-initial-paths.py <# cities> <# of paths to generate>

"""
import sys
import random

out_fname = "initial.dat"

def main():
   if len(sys.argv) != 3:
      print "Usage: python tsp-initial-paths.py <# cities> <# of paths to generate>"
      sys.exit()
   
   # Setup
   num_cities = int(sys.argv[1])
   num_paths = int(sys.argv[2])
   global out_fname
   outfile = open(out_fname, "w")
   print "Generating random initial paths for a " + str(num_cities) + " city TSP."
   
   # Generate the desired number of random initial paths.
   unique_paths = {}
   while len(unique_paths) < num_paths:
      path = range(1, num_cities+1)
      random.shuffle(path)
      unique_paths[tuple(path)] = True
   
   # Dump the uniques to a file.
   line = ""
   for path in unique_paths:
      for city in path:
         line += str(city) + " "
      outfile.write(line[:-1] + "\n")
      line = ""
   print "Successfully wrote " + out_fname

if __name__ == "__main__":
   main()
