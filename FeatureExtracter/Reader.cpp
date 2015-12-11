//written by Fabian D.
#include "Reader.h"

Reader::Reader(string mainFolderPath) :
mainFolderPath(mainFolderPath)
{
	candidateCounter = 1;
	candidateTextCounter = 0;
	unknownTextCounter = 0;

	if (*--mainFolderPath.end() != '/' && *--mainFolderPath.end() != '\\')
	{
		this->mainFolderPath.push_back('/');
	}

	string fullCandidateTextPath = this->mainFolderPath + "candidate00001/known00001.txt";

	fstream inTextStream;
	inTextStream.open(fullCandidateTextPath.c_str(), ios::in);

	if (!inTextStream.good())
	{
		cout << "cannot find " << fullCandidateTextPath << endl;
		exit(2);
	}
	
	inTextStream.close();
}

Reader::~Reader()
{

}

//generates the name of the candidateFolder
string Reader::getCandidateFolderName()
{
	string candidateFolder = "candidate";
	ostringstream outstream;
	outstream << candidateCounter;
	string counterString = outstream.str();

	for (uint i = 0; i < NUMBER_OF_DIGITS_IN_FOLDER_NAME - counterString.length(); i++)
	{
		candidateFolder += '0';
	}
	candidateFolder += counterString;
	return candidateFolder;
}

//generates the name of the textFile
string Reader::getCandidateTextFileName()
{
	string textFileName = "known";
	ostringstream outstream;
	outstream << candidateTextCounter;
	string counterString = outstream.str();

	for (uint i = 0; i < NUMBER_OF_DIGITS_IN_FOLDER_NAME - counterString.length(); i++)
	{
		textFileName += '0';
	}
	textFileName += counterString + ".txt";
	return textFileName;
}

string Reader::getUnknownTextFileName()
{
	string unknownTextFileName = "unknown";
	ostringstream outstream;
	outstream << unknownTextCounter;
	string counterString = outstream.str();

	for (uint i = 0; i < NUMBER_OF_DIGITS_IN_FOLDER_NAME - counterString.length(); i++)
	{
		unknownTextFileName += '0';
	}
	unknownTextFileName += counterString + ".txt";
	return unknownTextFileName;
}

bool Reader::getNextCandidateText(string& nextCandidateText, uint& candidateNumber, uint& candidateTextFileNumber)
{
	string newLine;
	string counterString;
	string candidateName;
	string candidateTextFileName;
	candidateTextCounter++;
	nextCandidateText = "";
	
	candidateName = getCandidateFolderName();
	candidateTextFileName = getCandidateTextFileName();

	string fullCandidateTextPath = mainFolderPath + candidateName + '/' + candidateTextFileName;

	fstream inTextStream;
	inTextStream.open(fullCandidateTextPath.c_str(), ios::in);

	if (!inTextStream.good())
	{
		candidateTextCounter = 1;
		candidateCounter++;

		candidateName = getCandidateFolderName();
		candidateTextFileName = getCandidateTextFileName();
		inTextStream.open((mainFolderPath + candidateName + '/' + candidateTextFileName).c_str(), ios::in);
		if (!inTextStream.good())
		{
			return false;
		}
	}

	while (getline(inTextStream, newLine))
	{
		nextCandidateText += newLine + '\n';
	}
	inTextStream.close();
	candidateNumber = this->candidateCounter;
	candidateTextFileNumber = this->candidateTextCounter;
	return true;
}

bool Reader::getNextUnknownText(string& nextUnknownText, uint& unknownTextFileNumber)
{
	string newLine;
	string counterString;
	string unknownTextFileName;
	unknownTextCounter++;
	nextUnknownText = "";

	unknownTextFileName = getUnknownTextFileName();

	fstream inTextStream;
	inTextStream.open((mainFolderPath + "/unknown/" + unknownTextFileName).c_str(), ios::in);

	if (!inTextStream.good())
	{
		return false;
	}

	while (getline(inTextStream, newLine))
	{
		nextUnknownText += newLine + '\n';
	}
	inTextStream.close();
	unknownTextFileNumber = unknownTextCounter;
	return true;
}
