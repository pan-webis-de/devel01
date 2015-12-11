//written by Fabian D.
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include "Jzon.h"
using namespace std;

typedef unsigned int uint;

#define PREDICTION_FILE "./prediction"
#define NUMBER_OF_DIGITS_IN_FOLDER_NAME 5

struct answerList
{
	string unknown_text;
	string author;
	float score;

	answerList* nextAnswer;
};

void processAnswers(answerList* llist)
{
	Jzon::Format customFormat;
	customFormat.newline = true;
	customFormat.spacing = true;
	customFormat.useTabs = true;
	customFormat.indentSize = 1;
	
	Jzon::Object root;
	Jzon::Array answers;

	while (llist)
	{
		Jzon::Object answerOne;
		answerOne.Add("unknown_text", llist->unknown_text);
		answerOne.Add("author", llist->author);
		answerOne.Add("score", llist->score);

		answers.Add(answerOne);
		llist = llist->nextAnswer;
	}

	root.Add("answers", answers);

	Jzon::FileWriter::WriteFile("results.json", root, customFormat);
}

string getCandidateName(uint candidateNumber)
{
	string candidateName = "candidate";
	ostringstream outstream;
	outstream << candidateNumber;
	string counterString = outstream.str();
	for (uint i = 0; i < NUMBER_OF_DIGITS_IN_FOLDER_NAME - counterString.length(); i++)
	{
		candidateName += '0';
	}
	candidateName += counterString;
	return candidateName;
}

string getUnknownTextFileName(uint fileNumber)
{
	string unknownTextFileName = "unknown";

	ostringstream outstream;
	outstream << fileNumber;
	string counterString = outstream.str();
//	string counterString = to_string(fileNumber);
	for (uint i = 0; i < NUMBER_OF_DIGITS_IN_FOLDER_NAME - counterString.length(); i++)
	{
		unknownTextFileName += '0';
	}
	unknownTextFileName += counterString + ".txt";
	return unknownTextFileName;
}

float getScore(string& predictionLine, uint candidateNumber)
{
	uint spacePosition = 0;
	for (uint i = 0; i < candidateNumber; i++)
	{
		spacePosition = predictionLine.find(' ', spacePosition + 1);
	}
	uint endPosition = predictionLine.find(' ', spacePosition + 1);

	string scoreString = predictionLine.substr(spacePosition + 1, endPosition - (spacePosition + 1));
	return strtof(scoreString.c_str(), NULL);	//strtof //stof
}

uint getCandidateNumber(string& predictionLine)
{
	uint spacePosition = predictionLine.find(' ');
	string candidateString = predictionLine.substr(0, spacePosition);
	return strtol(candidateString.c_str(), NULL, 10);	//stoi//alternative: strtol //#include <stdlib.h>
}

void exitWithHelp()
{
	cout << "Usage: JsonWriter.exe prediction_file output_json_file" << endl;
	exit(1);
}

int main(int argc, char* argv[])
{
	cout << "JsonWriter: " << endl;

	if (argc != 3)//it should be exact 2
	{
		exitWithHelp();
	}	
	
	fstream predictionStream;
	predictionStream.open(argv[1], ios::in);

	if (!predictionStream.good())
	{
		cout << "cannot open " << PREDICTION_FILE << endl;
		return 1;
	}

	string line;
	getline(predictionStream, line);
	cout << line << endl;

	Jzon::Format customFormat;
	customFormat.newline = true;
	customFormat.spacing = true;
	customFormat.useTabs = true;
	customFormat.indentSize = 1;

	Jzon::Object root;
	Jzon::Array answers;
	for (uint unknownTextCounter = 1; getline(predictionStream, line); unknownTextCounter++)
	{
		if (unknownTextCounter % 100 == 0)
		{
			cout << unknownTextCounter << "...";
		}

		uint candidateNumber = getCandidateNumber(line);

		Jzon::Object textData;
		textData.Add("unknown_text", getUnknownTextFileName(unknownTextCounter));
		textData.Add("author", getCandidateName(candidateNumber));
		textData.Add("score", getScore(line, candidateNumber));

		answers.Add(textData);
	}
	root.Add("answers", answers);

	Jzon::FileWriter::WriteFile(argv[2], root, customFormat);

	return 0;
}
