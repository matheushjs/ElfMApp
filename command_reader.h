#ifndef COMMAND_READER_H
#define COMMAND_READER_H

#include <QThread>

class QString;

class CommandReader : public QThread
{
	Q_OBJECT

public:
	CommandReader(QObject *parent = 0);
	void run();

signals:
	void gotLine(QString string);
};

#endif // COMMAND_READER_H

