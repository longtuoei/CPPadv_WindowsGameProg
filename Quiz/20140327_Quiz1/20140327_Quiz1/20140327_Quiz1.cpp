// 20140327_Quiz1.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <windows.h>

#define MAX_BUFFER	4096

void ReadDataFromFile( std::string fileName, int lineNumber )
{
	std::ifstream	myFile( fileName.c_str() );

	std::vector<std::string> stringVector;
	std::string readString;

	if ( !myFile )
	{
		return;
	}

	int counter = 0;

	while ( std::getline( myFile, readString ) )
	{
		if ( stringVector.size() < static_cast<unsigned int>(lineNumber) )
		{
			stringVector.push_back( readString );
		}
		else
		{
			stringVector.at( counter % lineNumber ) = readString;
			++counter;
		}
	}

	myFile.close();

	for ( int i = 0; i < lineNumber; ++i)
	{
		printf_s( "%s \n", stringVector.at( ( counter + i ) % lineNumber ).c_str() );
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	std::string fileName = "./data.txt";

	ReadDataFromFile( fileName, 5 );

	getchar();
	return 0;

	return 0;
}

