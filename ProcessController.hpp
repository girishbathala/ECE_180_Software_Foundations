//
//  ProcessController.hpp
//  ece180-assignment1
//
//  Created by rick gessner on 1/15/18.
//

#ifndef ProcessController_hpp
#define ProcessController_hpp

#include <string>
#include "FileProcessor.hpp"

class ProcessController {
public:
  
  ProcessController(const char *aRootPath);
  
  ProcessController& runActual();
  ProcessController& runTests();

private:
  
  ProcessController& processWordFiles();
  ProcessController& saveSummary(const FileProcessor &f1, const FileProcessor &f2, const FileProcessor &f3);
    
  std::string rootPath;
};

#endif /* ProcessController_hpp */
