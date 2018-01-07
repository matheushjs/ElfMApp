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
	/* These signals will only validate the type of the user-given parameters.
	 * Validations regarding specifics of the Media Player should be made in the
	 *   receiver object.
	 */
	void nextRequest(int jump);
	void backRequest(int jump);
	void rateRequest(double rate);
	void quitRequest(void);
	void advanceRequest(double step);
	void rewindRequest(double step);
	void removeRequest(void);
	void listRequest(void);
	void selectRequest(int index);
};

#endif // COMMAND_READER_H

