# EFP3.0

This README file is under constructing.

Solver for Epistemic Planning (tested on iOS 10.15.x)

Compilation


      make
  

Requirements:

  flex, bison and a C++ compiler such as g++.

To run the solver: 

      make

      ./cpa+ inputFileName                #for EFP3.0 with breath-first-search algorithm
      
      ./cpa+ inputFileName -heuristic     #for EFP3.0 with best-first-search algorithm and maxSubGoal heuristic
      
E.g., ./cpa+ Examples/Coin_In_The_Box/Coin_in_the_Box__false_ann__pl_5.txt
