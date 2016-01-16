//Written by Fabian D.
#include <iostream>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <Windows.h>
using namespace std;

void exitWithHelp()
{
	cout << "Usage: devel01.exe input_path output_path" << endl;
	exit(1);
}

int main(int argc, char* argv[])
{
	cout << "Wrapper: " << endl;

	if (argc != 3)//it should be exact 2
	{
		exitWithHelp();
	}
	
	string inputPath = argv[1];
	string outputPath = argv[2];
	replace(inputPath.begin(), inputPath.end(), '\\', '/');
	replace(outputPath.begin(), outputPath.end(), '\\', '/');
	
	char currentPathCString[256];
	GetCurrentDirectory(sizeof(currentPathCString), currentPathCString);
	string currentPath = currentPathCString;
	replace(currentPath.begin(), currentPath.end(), '\\', '/');

	system(("\"" + currentPath + "/FeatureExtracter.exe\" \"" + inputPath + "\" train.dat test.dat").c_str());
	system(("\"" + currentPath + "/svm-train.exe\" -b 1 -c 20 -t 1 -d 3 train.dat model").c_str());
	system(("\"" + currentPath + "/svm-predict.exe\" -b 1 test.dat model predict").c_str());
	system(("\"" + currentPath + "/JsonWriter.exe\" \"" + currentPath + "/predict\" \"" + outputPath + "/answers.json\"").c_str());

	return 0;
}
