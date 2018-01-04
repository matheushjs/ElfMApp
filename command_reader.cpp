#include "command_reader.h"

#include <QString>
#include <QStringList>
#include <QCommandLineParser>

#include <iostream>
#include <string>

CommandReader::CommandReader(QObject *parent)
  : QThread(parent)
{
	// -abc treated as 'abc' instead of 'a' 'b' 'c'
	parser.setSingleDashWordOptionMode(QCommandLineParser::ParseAsLongOptions);
	parser.addHelpOption();

	parser.addOptions({
		{{"p", "play"}, "Show progress during copy"},
		{{"n", "next"}, "Advances to next song"},
		{{"b", "back"}, "Plays the previous song"}
	});
}

void CommandReader::process(const QString &str){
	QStringList list = (" " + str).split(" ");
	parser.process(list);

	for(const QString &s: list)
		std::cout << s.toStdString() << "|";
	std::cout << "\n";

	QString val;

	if(parser.isSet("play")){
		std::cout << "Play!\n";
		return;
	}

	if(parser.isSet("next")){
		std::cout << "Next!\n";
		return;
	}

	if(parser.isSet("back")){
		std::cout << "Back!\n";
		return;
	}
}

void CommandReader::run(){
	using namespace std;

	while(true){
		string str;
		getline(cin, str);
		process(QString(str.c_str()));
	}
}
