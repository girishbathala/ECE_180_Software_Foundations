//
//  TestHarness.hpp
//  ece180-assignment1
//
//  Created by rick gessner on 1/10/18.
//

#ifndef GradingHarness_hpp
#define GradingHarness_hpp

#include <stdio.h>

class GradingHarness {
public:
  GradingHarness();
  
  GradingHarness& runTests(const char* anInputPath);
  GradingHarness& runActual(const char* anInputPath);
  
};

#endif /* GradingHarness_hpp */
