#include<iostream>
#include<string.h>

using namespace std;

int doSomething(int anArg1) {
	cout<<"In doSomething"<<endl;
	cout<<"anArg1 is "<<anArg1<<endl;

	int theInt = 10;


	cout<<"Here"<<endl;
      	if(theInt=anArg1) {
  	  char* theStr = (char *)malloc(100*sizeof(char));
	  strcpy(theStr, "Wow, they matched!");
	  return 10;
      	}
      	if(!theInt) {
	  return anArg1;
	}
      	return 0;
}

int main() {
	cout<<"Hello World, in Main"<<endl;
	doSomething(0);
      	return 0;
}
