#include "command_reader.h"

#include <QString>
#include <iostream>
#include <string>

CommandReader::CommandReader(QObject *parent) : QThread(parent) {}

void CommandReader::run(){
	using namespace std;

	while(true){
		string str;
		getline(cin, str);
		if(str.size() != 0){
			emit gotLine(QString(str.c_str()));
		}
	}
}


