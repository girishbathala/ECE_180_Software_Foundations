//
//  ProcessController.cpp
//  ece180-assignment1
//
//  Created by rick gessner on 1/15/18.
//

#include "ProcessController.hpp"
#include <fstream>
#include <iostream>

ProcessController::ProcessController(const char *aRootPath) : rootPath(aRootPath) {
}

/*
 * STUDENT: This is where you add code to test your code/classes, as needed...
 */
ProcessController& ProcessController::runTests() {

  std::ofstream testout(rootPath+std::string("/testoutput.txt"));
  std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
  std::cout.rdbuf(testout.rdbuf()); //redirect

  std::cout << "running tests..." << std::endl << std::endl;

  // ----------------------------------
  // STUDENTS... Add your tests here...
  // ----------------------------------
  
  std::string seperator(20, '-');

  FileProcessor fpTest1(rootPath), fpTest2(rootPath), fpTest3(rootPath);
  
  fpTest1.run("test1.txt");
  std::cout << std::endl << seperator << std::endl;
  
  fpTest2.run("test2.txt");
  std::cout << std::endl << seperator << std::endl;

  std::cout<<"Distance between identical files test1.txt and test2.txt is : "<<fpTest1.computeDocumentDistace(fpTest2)<<std::endl;
  std::cout << std::endl << seperator << std::endl;

  fpTest3.run("test3.txt");
  std::cout << "This file test3.txt should have 1 unique word and one unique bi-gram" << std::endl;
  std::cout << std::endl << seperator << std::endl;

  std::cout.rdbuf(coutbuf); //reset to standard output again
  coutbuf=nullptr;

  return *this;
}

/*
 * STUDENT: This is where you add code to manage the tasks in this assignment...
 */
ProcessController& ProcessController::runActual() {
  return processWordFiles();
}

ProcessController& ProcessController::processWordFiles() {
	
	FileProcessor f1(rootPath), f2(rootPath), f3(rootPath);

  // --------/------------------------------------------------------------
  // STUDENTS... This control logic already works; see FileProcessor.cpp
  // --------------------------------------------------------------------
  
  f1.run("alice.txt");
  f2.run("verne.txt");
  f3.run("jfk.txt");

  return saveSummary(f1, f2, f3);
}

/*
* Function: saveSummary
* ----------------------------
*  The cosine distance is used as a similarity measure to check how identical document f1 is with f2,f3.
*  Based on the result, the summary is stored in a results.txt file
*
*  aPath - The fileProcessor objects f1,f2,f3 whose similarity mesure is to be computed
*  returns: ProcessController Object
*/
ProcessController& ProcessController::saveSummary(const FileProcessor &f1, const FileProcessor &f2, const FileProcessor &f3) {

  //theResultFile is filename where you store your summary in assignment root folder...
  const char* theResultFile = "results.txt";
  // ----------------------------------------------------------
  // STUDENTS... Add code here to generate and save summary...
  // ----------------------------------------------------------

  // Compute cosine distance between files using their document vector of words
  double distAB = f1.computeDocumentDistace(f2);
  double distAC = f1.computeDocumentDistace(f3);
  std::cout << distAB << std::endl;
  std::cout << distAC << std::endl;

  // Create an output filename by appending 'output_' to the input file name
  // Generate an output file stream handle for a file and redirect the cout buffer to it's read buffer
  std::ofstream testout(rootPath + std::string("/")+ std::string(theResultFile));
  std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
  std::cout.rdbuf(testout.rdbuf()); //redirect

  std::cout << "Distance AB: " << distAB << " ";
  std::cout << "Distance AC: " << distAC << " ";
  if (distAB > distAC) {
	  std::cout << "Document A is most similar to document C" << std::endl;
  }
  else if (distAC > distAB) {
	  std::cout << "Document A is most similar to document B" << std::endl;
  }
  else {
	  std::cout << "Document A is similar to document C and B equally" << std::endl;
  }

  //Reset the cout buffer to the standard output again
  std::cout.rdbuf(coutbuf);
  coutbuf = nullptr;
  return *this;
}
