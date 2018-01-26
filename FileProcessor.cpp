//
//  FileProcessor.cpp
//  ece180-assignment1
//
//  Created by rick gessner on 1/10/18.
//

#include "FileProcessor.hpp"

#include <iostream>
#include <fstream>
#include <cmath>
#include<iterator>
#include<algorithm>
#include<sstream>
#include<vector>
#include<string>

// Defining the value of PI
#define PI 3.14159265
// ObjectCount is a static class member, 
// It is used to generate a fileId for each of the processed file
int FileProcessor::objectCount = 0;

/*
* Function: FileProcessor
* ----------------------------
*  Constructor, Used to initialize the number of Bigrams to 0, rootPath to the current directory
*  The static data member objectCount is incremented and a fileId is associated to each file that is processed
*
*  aPath - The root path/ loaction of the current directory
*  returns: No return type
*/
FileProcessor::FileProcessor(const std::string aPath) : rootPath(aPath) {
  //complete the initialize process, if any...
	// Number of bigrams in the file is initialized to 0
	this->numOfBigrams = 0;
	objectCount++;
	fileId = objectCount;
}

/*
 * STUDENT: ProcessController calls this for each of your word-files
 */


 /*
 * Function: saveProcessedWord
 * ----------------------------
 *  Each word is accounted for in the wordCollection map, if a given word
 *	already exists in the map, it is not added again and it's count is incremented
 *
 *	word - The processed word after removing punctuations, stopwords, digits, alphanumeric words and converting it to lower case
 *  returns: None
 */
void FileProcessor::saveProcessedWord(std::string& word) {
	// Check if the word is present in the word collection
	if (this->wordCollection.find(word) == this->wordCollection.end()) {
		// Word not present, add it to the word collection map 
		this->wordCollection.emplace(word, 1);
		// Append it to the output file
		std::cout << word << ",";
	}
	else {
		// Word present, add count
		this->wordCollection[word] += 1;
	}
}

/*
* Function: saveProcessedBigram
* ----------------------------
*   Each processed-bigram is stored into a bigramCollection map.
*	Each bigram is accounted for only once as a key and it's count is stored as a value
*
*	bigram - The processed bigram after removing punctuations, digits, alphanumeric
			 words and converting it to lower case
*   returns: None
*/
void FileProcessor::saveProcessedBigram(std::string & bigram)
{
	// Check if the bigram is present in the bigram collection
	if (this->bigramCollection.find(bigram) == this->bigramCollection.end()) {
		// bigram not present, add it to the bigram collection map 
		this->bigramCollection.emplace(bigram, 1);
	}
	else {
		// Bigram present, add count
		this->bigramCollection[bigram] += 1;
	}
}

/*
* Function: saveBigramLiklihood
* ----------------------------
*   Each bigram in the bigram collection map data member is stored in a json file
*	The liklihood of each bigram B is also computed using = B[n] / N; where B[n] 
*   is the number of occurances of bigram B and N is the total number of bigrams in the document
*
*   returns: None
*/
void FileProcessor::saveBigramLikelihood()
{
	// Opening a json file stream handle to store the bigrams and their likelihoods
	std::ofstream bigramOpFile;

	// Split filename string to remove the .txt part and retrieve only name
	std :: stringstream ss(this->filename);
	std :: string item;
	std :: vector<std::string> tokens;
	while (getline(ss, item, '.')) {
		tokens.push_back(item);
	}

	

	bigramOpFile.open(rootPath + std::string("/output/bigrams/bigrams_") + tokens[0] + std::string(".json"));

	bigramOpFile << "{" << std::endl;
	double probability;
	for (auto &itr : this->bigramCollection) {
		// itr.second holds the count of each bigram, numOfBigrams is N : total bigrams in the document,
		// itr.first holds the bigram name
		probability = double(itr.second) / (this->numOfBigrams);
		bigramOpFile << "\t{\"bigram\" : \"" << itr.first << "\", \"probability\" : " << probability << "}," << std::endl;
	}
	bigramOpFile << "}" << std::endl;
	bigramOpFile.close();
	return;
}

/*
* Function: computeInnerProduct
* ----------------------------
*   The dot product between two vectors is computed using the following:
*	Vec1 = [w1_a,w2_a,w3_a, ... wd_a] and Vec2 = [w1_b,w2_b,w3_b, ... wd_b]
*	Inner product = Vec1 . Vec2 = w1_a*w1_b + w2_a*w2_b + .... + wd_a*wd_b
*   where wi_a; wi_b are the number of words 'i' in document a and b respectively
*
*	docVectorA, docVectorB -  The word collection maps of decumentA, document B in the format specified above
*   returns: The inner product between two document vectors - vec1.vec2
*/
int FileProcessor::computeInnerProduct(const std::map<std::string, int>& docVectorA, const std::map<std::string, int>& docVectorB)
const 
{
	// Inner Product initialized to 0
	int innerProduct = 0 ;

	// Word and word count in first document A
	std::string wordA;
	int dimValueA;
	for (auto& itr : docVectorA) {
		wordA = itr.first;
		dimValueA = itr.second;
		if (docVectorB.find(wordA) != docVectorB.end()) {
			// Word present in second document vector, append product to inner_product
			innerProduct += (dimValueA * docVectorB.at(wordA));
		}
	}
	return innerProduct;
}


/*
* Function: computeAbsoluteValue
* ----------------------------
*   The absolute value of a document vector is computed using the following:
*	Vec1 = [w1,w2,w3, ... wd],where wi is the number of words 'i' in the document
*   absValue(Vec1) = (w1*w1 + w2*w2 + w3*w3 + .... + wd*wd ) ^ 1/2
*
*	decVector -  The document word collection map of a document in the format specified above
*   returns: The absolute value of a document vector absValue(Vec1)
*/
double FileProcessor::computeAbsoluteValue(const std::map<std::string, int>& docVector) const
{
	// Initialize the absolute value to zero
	double absValue = 0;
	for (auto& itr : docVector) {
		// Multiply the count values and append to the sum
		absValue += (itr.second * itr.second);
	}
	return sqrt(absValue);
}


/*
* Function: computeDocumentDistance
* ----------------------------
*   The COSINE distance between two document vectors is computed using the following:
*	Vec1 = [w1_a,w2_a,w3_a, ... wd_a] and Vec2 = [w1_b,w2_b,w3_b, ... wd_b]
*   where wi_a; wi_b are the number of words 'i' in document a and b respectively
*	Theta = arc-cos( Vec1.Vec2 / |Vec1|.|Vec2| )
*
*	decVectorB -  The word collection map of document B in the format specified above
*   returns: The cosine distance theta between two documents
*/
double FileProcessor::computeDocumentDistace  (const FileProcessor& fp) const
{
	double docDistance;
	//Inner Product
	int theInnerProduct = computeInnerProduct(this->wordCollection, fp.wordCollection);
	std::cout << "InnerProduct between the files are : " << theInnerProduct<< std::endl;

	// Absolute Value
	double absValDocA = computeAbsoluteValue(this->wordCollection);
	std::cout << "Absolute value of file A : " << absValDocA<<std::endl;
	double absValDocB = computeAbsoluteValue(fp.wordCollection);
	std::cout << "Absolute value of file B : " << absValDocB << std::endl;
	
	// Cosing distance between two documents
	docDistance = acos(theInnerProduct/(absValDocA*absValDocB)) * 180.0 / PI;
	return docDistance;
}

/*
* Function: fileTokenizer
* ----------------------------
*   The words in the file are cleaned and pre-processed, post which the unique words are stored 
*   in a wordCollection map along with their count and also in an output text file
*   Unique bigrams are then formulated and stored along with their counts in a bigram collection map
*
*	fullPath -  The location to the file which has to be tokenized
*   returns: None
*/

void FileProcessor::fileTokenizer(std::string& fullPath) {
	
	// Create an output filename by appending 'output_' to the input file name
	// Generate an output file stream handle for a file and redirect the cout buffer to it's read buffer

	std::string opFilename = rootPath + std::string("output/unique_words/output_") + this->filename;
	std::ofstream resultout(opFilename);
	std::streambuf *coutbuf = std::cout.rdbuf();
	std::cout.rdbuf(resultout.rdbuf()); 
	
	// Strings to hold the current word read from the file (theWord), previous word read (thePrevWord)
	// and bigrams created from the words in the file
	std::string theWord, thePrevWord, theBigram;
	
	// Input file stream handle file for the input file(fullPath) that is to be read
	std::ifstream file(fullPath);
	// Each word, which is separated by space in the file, is processed iteratively below
	while (file >> theWord) {
		
		// Pre-process each word, remove punctuations, convert to lower case
		wordTokenizer(theWord);

		// Skip if any of the words are alphanumeric, digit, or empty
		if ((std::any_of(theWord.begin(), theWord.end(), isdigit)) || theWord.empty()) {
			continue;
		}
		// Save words in word collection if they are not stopwords
		if ((this->stopwordCollection.find(theWord) == this->stopwordCollection.end())) {

			// Save the pre-processed word into the word collection map and the output file
			saveProcessedWord(theWord);
		}
		// Storing the first word encountered in the file as a previous word
		if (this->numOfBigrams == 0) {
			thePrevWord = theWord;
		}
		else {
			// Construct a bigram by appending the current word to the the previous word
			theBigram = thePrevWord + std::string("-") + theWord;
			// Update the previous word as the current word
			thePrevWord = theWord;
			// Save the pre-processed bigram into a bigram collection 
			saveProcessedBigram(theBigram);
		}
		this->numOfBigrams++;
	}
	// Reset the cout buffer to the standard output again
	std::cout.rdbuf(coutbuf); 
	coutbuf = nullptr;

	//Number of Bigrams is equal to the number of words minus 1
	this->numOfBigrams--;
	std::cout << "The total number of bigrams are " << numOfBigrams << std::endl;

	std::cout << "The total number of unique words are : " << wordCollection.size() << std::endl;
	std::cout << "The total number of unique Bi-grams are : " << bigramCollection.size() << std::endl;
	
}

/*
* Function:readStopWords
* ----------------------------
*   The stopwords are read from a stopword file and all the stopwords
*	are stored in a stopwordCollection set
*
*	StopwordPath - The full location/path to the stopword file
*   returns: None
*/
void FileProcessor::readStopwords(std::string & StopwordPath)
{
	// Open the soptword file as an input filestream
	std::ifstream stopwordFile(StopwordPath);
	// stopword holds each processed word read from the stopword file
	std::string stopword;
	if (stopwordFile.is_open())
	{
		while (getline(stopwordFile, stopword))
		{
			// Discard characters in a word which are new lines, empty spaces and commas.
			stopword.erase(std::remove(stopword.begin(), stopword.end(), '\n'), stopword.end());
			stopword.erase(std::remove(stopword.begin(), stopword.end(), ','), stopword.end());
			stopword.erase(std::remove(stopword.begin(), stopword.end(), ' '), stopword.end());
			// Add the processed stopword into the stopword collection
			this->stopwordCollection.emplace(stopword);
		}
		stopwordFile.close();
	}
	else {
		std::cout << "Error: Unable to open file " << StopwordPath << std::endl;
	}
}


/*
* Function: wordTokenizer
* ----------------------------
*   Processed the each word in the file. Discard any punctuations
*	Convert the word to lower case
*	
*	word - The raw-text read from an input file
*   returns: The processed word
*/
void FileProcessor::wordTokenizer(std::string & word)
{
	// Discard characters in a word which are punctuations
	word.erase(std::remove_if(word.begin(),word.end(), ispunct), word.end());
	// Transform the word into lower-case
	std::transform(word.begin(), word.end(), word.begin(), ::tolower);
}

FileProcessor& FileProcessor::run(const char* aFilename) {

	this->filename = std::string(aFilename);
	std::cout << "Processing File : " <<aFilename<< std::endl;
	std::string stopwordFilename = "stopwords.txt";
	std::string theFullPath = rootPath + std::string("/content/") + std::string(aFilename);
	std::string theStopwordPath = rootPath + std::string("/content/") + std::string(stopwordFilename);

	//--------------------------------------------------------
	// STUDENTS: Add code here to perform assignment tasks...
	//--------------------------------------------------------
	// Stopwords are read from the stopword file and stored in a set
	readStopwords(theStopwordPath);
	// Each file is processed and tokenized to store the words, and bigrams
	fileTokenizer(theFullPath);
	// The processed bigrams are then used to compute their likelihood and are stored in json files
	saveBigramLikelihood();
	return *this;
}


