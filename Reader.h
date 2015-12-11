//written by Fabian D.
#pragma once
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include<string.h>
using namespace std;

#define NUMBER_OF_DIGITS_IN_FOLDER_NAME 5
typedef unsigned int uint;


class Reader
{
public:
	Reader(string mainFolderPath);
	~Reader();
	bool getNextCandidateText(string& nextCandidateText, uint& candidateNumber, uint& candidateTextFileNumber);
	bool getNextUnknownText(string& nextUnknownText, uint& unknownTextFileNumber);

private:
	string getCandidateFolderName();
	string getCandidateTextFileName();
	string getUnknownTextFileName();

	string mainFolderPath;
	uint candidateCounter;
	uint candidateTextCounter;
	uint unknownTextCounter;
};

