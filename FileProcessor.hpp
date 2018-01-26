//
//  FileProcessor.hpp
//  ece180-assignment1
//
//  Created by rick gessner on 1/10/18.
//

#ifndef FileProcessor_hpp
#define FileProcessor_hpp

#include <string>
#include <map>
#include <set>

class FileProcessor {
public:
	FileProcessor(const std::string aPath);

	void readStopwords(std::string& StopwordPath);

	double computeDocumentDistace (const FileProcessor& fp) const ;

	FileProcessor& run(const char* anInputFilename);
 
protected:
	void saveProcessedWord(std::string& word);

	void saveProcessedBigram(std::string& bigram);

	void wordTokenizer(std::string& word);

	void fileTokenizer(std::string& fullPath);

	int computeInnerProduct(const std::map<std::string, int>& docVectorA, const std::map<std::string, int>& docVectorB) const;

	double computeAbsoluteValue(const std::map<std::string, int>& docVector) const;

	void saveBigramLikelihood();

	// Static data member to keep track of the total number of files in the document
	static int objectCount;
	// Each file that is processed is given a unique fileId in the order that it is processed
	int fileId;
	// To store the current directory address and also the name of the file being processed
	std::string rootPath,filename;
	// map to keep track of unique words and their counts
	std::map<std::string, int> wordCollection;
	// map to keep track of unique bigrams and their counts
	std::map<std::string, int> bigramCollection;
	// set to keep track of unique stopwords
	std::set<std::string> stopwordCollection;
	int numOfBigrams;  
};

#endif /* FileProcessor_hpp */
