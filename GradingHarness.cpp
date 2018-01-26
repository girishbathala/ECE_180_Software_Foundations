//
//  GradingHarness
//  ece180-assignment1
//
//  Created by rick gessner on 1/10/18.
//

#include <iostream>
#include <fstream>
#include <ctime>
#include "GradingHarness.hpp"
#include "ProcessController.hpp"

GradingHarness::GradingHarness()  {}

/*
 * STUDENTS: Don't touch this code
 */
GradingHarness& GradingHarness::runActual(const char* anInputPath) {  
  ProcessController theController(anInputPath);
  theController.runActual();
  return *this;
}

/*
 * STUDENTS: Don't touch this code...
 */
GradingHarness& GradingHarness::runTests(const char* anInputPath) {
  
  std::ofstream testout(std::string(anInputPath)+std::string("/testoutput.txt"));
  std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
  std::cout.rdbuf(testout.rdbuf()); //redirect
  
  ProcessController theController(anInputPath);
  theController.runTests();
  
  std::cout.rdbuf(coutbuf); //reset to standard output again
  coutbuf=nullptr;
  
  return *this;
}
