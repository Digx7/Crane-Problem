///////////////////////////////////////////////////////////////////////////////
// cranes_timing.cpp
//
// Example code showing how to run each algorithm while measuring
// elapsed times precisely. You should modify this program to gather
// all of your experimental data.
//
///////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include <random>
#include <iostream>
#include <vector>
#include <fstream>

#include "timer.hpp"

#include "cranes_algs.hpp"

void print_bar() {
  std::cout << std::string(79, '-') << std::endl;
}

size_t get_random_in_range(size_t min, size_t max)
{
  std::random_device rd;   // Obtain a random seed from the hardware
  std::mt19937 gen(rd());  // Standard mersenne_twister_engine seeded with rd()
  
  size_t lowerBound = min;  // Lower bound of the range
  size_t upperBound = max; // Upper bound of the range
  
  std::uniform_int_distribution<size_t> distribution(lowerBound, upperBound);
  
  size_t randomValue = distribution(gen);  // Generate a random value within the range
  
  return randomValue;
}

struct results
{
  size_t n;
  cranes::coordinate rows, 
                    columns;
  double exhaustiveTime, 
          dynamicTime;

  void dont_run_exhaustive(){
    exhaustiveTime = -1;
  }

  void print_all(){
    std::cout << "n=" << n
            << ", exhausitve time= " << exhaustiveTime
            << ", dynamic time= " << dynamicTime
            << std::endl;
  }

  void print_exhaustive(){
    std::cout << exhaustiveTime
              << std::endl;
  }

  void print_dynamic(){
    std::cout << dynamicTime
              << std::endl;
  }
};


results run(size_t n) {
  
  const size_t EXHAUSTIVE_SEARCH_MAX_N = 30;

  //const size_t n = 15;

  assert(n > 0);

  cranes::coordinate rows = n / 2,
                     columns = n - rows;

  unsigned cells = rows * columns,
           crane_count = cells / 5,  // 20%
           building_count = cells / 10; // 10%
  std::mt19937 gen;
  cranes::grid input = cranes::grid::random(rows, columns, crane_count, building_count, gen);

  Timer timer;
  double elapsed;

  results results_;
  results_.n = n;
  results_.rows = rows;
  results_.columns = columns;

  print_bar();
  std::cout << "n=" << n
            << ", rows=" << rows
            << ", columns=" << columns
            << std::endl << std::endl;

  input.print();

  print_bar();
  std::cout << "exhaustive optimization" << std::endl;
  if (n > EXHAUSTIVE_SEARCH_MAX_N) {
    std::cout << std::endl << "(n too large, skipping exhaustive optimization)" << std::endl;
    results_.dont_run_exhaustive();
  } else {
    timer.reset();
    auto exhaustive_output = crane_unloading_exhaustive(input);
    elapsed = timer.elapsed();
    exhaustive_output.print();
    std::cout << std::endl << "elapsed time=" << elapsed << " seconds" << std::endl;
    results_.exhaustiveTime = elapsed;
  }

  print_bar();
  std::cout << "dynamic programming" << std::endl;
  timer.reset();
  auto dyn_prog_output = crane_unloading_dyn_prog(input);
  elapsed = timer.elapsed();
  dyn_prog_output.print();
  std::cout << std::endl << "elapsed time=" << elapsed << " seconds" << std::endl;
  results_.dynamicTime = elapsed;

  print_bar();

  return results_;

}

int main() {

  size_t numberOfTests = 10;
  size_t maxSizePerTest = 40;
  //size_t min = 4;
  //size_t max = 40;
  std::vector<results> allResults;

  /*
    for(size_t i = 2; i <= maxSizePerTest; ++i)
    {
      std::cout << "Test " << i << std::endl;

      allResults.push_back( run(i) );
      print_bar();
    }
  */

  std::ofstream outputFile("output.txt");  // Open the file for writing

  if (outputFile.is_open()) {
        outputFile << "This is your test results" << std::endl << std::endl;

        for(size_t i = 1; i <= numberOfTests; ++i)
        {
          allResults.clear();

          for(size_t j = 2; j <= maxSizePerTest; ++j)
          {
            std::cout << "Test " << i << " Grid " << j << std::endl;

            allResults.push_back( run(j) );
            print_bar();
          }

          outputFile << std::endl
                      << "Test " 
                      << i 
                      << std::endl
                      << "Exhaustive results"
                      << std::endl;
          
          for(size_t j = 0; j < allResults.size(); ++j)
          {
            outputFile << allResults[j].exhaustiveTime
                        << std::endl;
          }

          outputFile << std::endl
                      << "Dynamic results"
                      <<std::endl;

          for(size_t j = 0; j < allResults.size(); ++j)
          {
            outputFile << allResults[j].dynamicTime
                        << std::endl;
          }
        }

        outputFile.close();
  }

  return 0;
}
