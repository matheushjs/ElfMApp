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
		{{"n", "next"}, "Advances to next song", "integer displacement"},
		{{"b", "back"}, "Plays the previous song", "integer displacement"},
		{{"r", "rate"}, "Changes playback rate", "floating-point rate"},
		{{"q", "quit"}, "Exits the program"},
		{{"z", "adv"}, "Forwards current song by some seconds", "floating-point seconds"},
		{{"x", "rew"}, "Rewinds current song by some seconds", "floating-point seconds"},
		{{"d", "rm"}, "Deletes current song from drive"},
		{{".", "redo"}, "Repeats last command"},
		{{"l", "list"}, "Lists current playlist"},
		{{"s", "select"}, "Selects specific song from playlist", "integer index"}
	});
}

void CommandReader::process(const QString &str){
	QStringList list = (" " + str + " ").split(" ");
	parser.process(list);

	for(const QString &s: list)
		std::cout << "{" << s.toStdString() << "}";
	std::cout << "\n";

	QString val;

	if(parser.isSet("next")){
		QString val = parser.value("next");

		int jump = 1;
		if(val.size() != 0)
			jump = val.toInt();

		if(jump >= 0)
			emit nextRequest(jump);

		return;
	}

	if(parser.isSet("back")){
		QString val = parser.value("back");

		int jump = 1;
		if(val.size() != 0)
			jump = val.toInt();

		if(jump >= 0)
			emit backRequest(jump);

		return;
	}

	if(parser.isSet("rate")){
		QString val = parser.value("rate");

		double rate = 1.0;
		if(val.size() != 0)
			rate = val.toFloat();

		std::cout << val.toStdString() << "\n";

		if(rate > 0.0)
			emit rateRequest(rate);

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
