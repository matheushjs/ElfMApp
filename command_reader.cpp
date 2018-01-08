#include "command_reader.h"

#include <QString>
#include <QStringList>
#include <QCommandLineParser>

#include <iostream>
#include <string>

CommandReader::CommandReader(QObject *parent)
  : QThread(parent),
	lastCommand(""),
	quitFlag(false)
{
	// -abc treated as 'abc' instead of 'a' 'b' 'c'
	parser.setSingleDashWordOptionMode(QCommandLineParser::ParseAsLongOptions);

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
		{{"s", "select"}, "Selects specific song from playlist", "integer index"},
		{{"h", "help"}, "Displays this help message"}
	});
}

void CommandReader::process(const QString &str){
	QStringList list = (" " + str + " ").split(" ");

	if(!parser.parse(list)){
		emit errorMessage("Could not parse. Use -h for displaying the available commands.");
		return;
	}

	// In case the command is 'redo', we will restore 'lastCommand' using variable 'aux'
	QString aux = lastCommand;
	lastCommand = str;

	QString val;
	bool good;

	if(parser.isSet("next")){
		val = parser.value("next");
		int jump = val.toInt(&good);

		if(val.size() == 0) jump = 1, good = true; // Default value

		if(good) emit nextRequest(jump);
		else emit errorMessage("Invallid argument. Expected integer.");

		return;
	}

	if(parser.isSet("back")){
		val = parser.value("back");
		int jump = val.toInt(&good);

		if(val.size() == 0) jump = 1, good = true; // Default value

		if(good) emit backRequest(jump);
		else emit errorMessage("Invallid argument. Expected integer.");

		return;
	}

	if(parser.isSet("rate")){
		val = parser.value("rate");
		double rate = val.toDouble(&good);

		if(val.size() == 0) rate = 1, good = true; // Default value

		if(good) emit rateRequest(rate);
		else emit errorMessage("Invallid argument. Expected double.");

		return;
	}

	if(parser.isSet("quit")){
		emit quitRequest();
		quitFlag = true;
		return;
	}

	if(parser.isSet("adv")){
		val = parser.value("adv");
		double step = val.toDouble(&good);

		if(val.size() == 0) step = 5, good = true; // Default value

		if(good) emit advanceRequest(step);
		else emit errorMessage("Invallid argument. Expected double.");

		return;
	}

	if(parser.isSet("rew")){
		val = parser.value("rew");
		double step = val.toDouble(&good);

		if(val.size() == 0) step = 5, good = true; // Default value

		if(good) emit rewindRequest(step);
		else emit errorMessage("Invallid argument. Expected double.");

		return;
	}

	if(parser.isSet("rm")){
		emit removeRequest();
		return;
	}

	if(parser.isSet("redo")){
		lastCommand = aux;

		// Creating a new object will spare us from possible bugs
		QString proc = lastCommand;
		process(proc);

		return;
	}

	if(parser.isSet("list")){
		emit listRequest();
		return;
	}

	if(parser.isSet("select")){
		val = parser.value("select");
		int idx = val.toInt(&good);

		if(good) emit selectRequest(idx);
		else emit errorMessage("Invallid argument. Expected integer.");

		return;
	}

	if(parser.isSet("help")){
		emit helpRequest();
		return;
	}
}

void CommandReader::run(){
	using namespace std;

	while(!quitFlag){
		string str;
		getline(cin, str);
		process(QString(str.c_str()));
	}
}

QString CommandReader::helpMessage() const {
	return parser.helpText();
}
