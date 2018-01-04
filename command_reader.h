#ifndef COMMAND_READER_H
#define COMMAND_READER_H

#include <QThread>
#include <QCommandLineParser>

class QString;

class CommandReader : public QThread
{
	Q_OBJECT

	QCommandLineParser parser;

	void process(const QString &str);

public:
	CommandReader(QObject *parent = 0);
	void run();

signals:
	void gotLine(QString string);

	void nextRequest(int jump);
	void backRequest(int jump);
	void rateRequest(double rate);
};

#endif // COMMAND_READER_H

