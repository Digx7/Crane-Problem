///////////////////////////////////////////////////////////////////////////////
// cranes_algs.hpp
//
// Algorithms that solve the crane unloading problem.
//
// All of the TODO sections for this project reside in this file.
//
// This file builds on crane_types.hpp, so you should familiarize yourself
// with that file before working on this file.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cassert>
#include <math.h>
#include <vector>
#include <queue>

#include "cranes_types.hpp"

namespace cranes {

// Solve the crane unloading problem for the given grid, using an exhaustive
// optimization algorithm.
//
// This algorithm is expected to run in exponential time, so the grid's
// width+height must be small enough to fit in a 64-bit int; this is enforced
// with an assertion.
//
// The grid must be non-empty.
path crane_unloading_exhaustive(const grid& setting) {

  // grid must be non-empty.
  assert(setting.rows() > 0);
  assert(setting.columns() > 0);

  // Compute maximum path length, and check that it is legal.
  const size_t max_steps = setting.rows() + setting.columns() - 2;
  assert(max_steps < 64);

  path best(setting);

  // TODO: implement the exhaustive search algorithm, then delete this
  // comment.

  std::vector<path> allValidPaths;

  std::queue<path> permutations;
  permutations.push(best); // Start with an empty path
  
  // Fills the vector allValidPaths with all valid permutaions of paths
  while (!permutations.empty()) {
    path current = permutations.front();
    permutations.pop();
    
    size_t length = current.steps().size();
    
    // Print the current permutation if its length is greater than 0
    if (length > 0) {
      allValidPaths.push_back(current);
    }
    
    // Add 'L' and 'D' to the current permutation and enqueue the new permutations
    if (length <= max_steps) {
      path goRight = current;
      if(goRight.is_step_valid(STEP_DIRECTION_EAST))
      {
        goRight.add_step(STEP_DIRECTION_EAST);
        permutations.push(goRight);
      }

      path goDown = current;
      if(goDown.is_step_valid(STEP_DIRECTION_SOUTH))
      {
        goDown.add_step(STEP_DIRECTION_SOUTH);
        permutations.push(goDown);
      }
    }
  }
  
  // Iterates through allValidPaths and sets 
  // best to the one with the most total cranes
  for(size_t i = 0; i < allValidPaths.size(); ++i)
  {
    if(allValidPaths[i].total_cranes() > best.total_cranes())
    {
      best = allValidPaths[i];
    }
  }
  
  //best.print();

  return best;
}

// Solve the crane unloading problem for the given grid, using a dynamic
// programming algorithm.
//
// The grid must be non-empty.
//path crane_unloading_dyn_prog(const grid& setting) {
path crane_unloading_dyn_prog(const grid& setting) {

  path best(setting);

  // grid must be non-empty.
  assert(setting.rows() > 0);
  assert(setting.columns() > 0);

  
  using cell_type = std::optional<path>;

  std::vector<std::vector<cell_type>> A(setting.rows(), std::vector<cell_type>(setting.columns()));

  A[0][0] = path(setting);
  assert(A[0][0].has_value());

  for (coordinate row = 0; row < setting.rows(); ++row) 
  {
    for (coordinate column = 0; column < setting.columns(); ++column) 
    {

      if (setting.get(row, column) == CELL_BUILDING)
      {
        A[row][column].reset();
        continue;
      }

      cell_type from_above = std::nullopt;
      cell_type from_left = std::nullopt;

      if (row != 0 && setting.get(row - 1, column) != CELL_BUILDING)
      {
        from_above = A[row - 1][column];
        if (from_above.has_value()) {
          from_above.value().add_step(cranes::STEP_DIRECTION_SOUTH);
        }
      }

      if (column != 0 && setting.get(row, column - 1) != CELL_BUILDING)
      {
        from_left = A[row][column - 1];
        if (from_left.has_value()) {
          from_left.value().add_step(cranes::STEP_DIRECTION_EAST);
        }
      }

      if(from_above.has_value() && from_left.has_value())
      {
        if(from_above.value().total_cranes() > from_left.value().total_cranes())
        {
          A[row][column] = from_above;
        }
        else
        {
          A[row][column] = from_left;
        }
      }
      else if(from_above.has_value())
      {
        A[row][column] = from_above;
      }
      else if(from_left.has_value())
      {
        A[row][column] = from_left;
      }

      if(A[row][column].has_value())
      {
        if(A[row][column].value().total_cranes() > best.total_cranes())
        {
          best = A[row][column].value();
        }
      }

	    // TODO: implement the dynamic programming algorithm, then delete this
      // comment.

      // assert(best->has_value());
      // std::cout << "total cranes" << (**best).total_cranes() << std::endl;

      // return best;
  	}

  }

  return best;
}

}
