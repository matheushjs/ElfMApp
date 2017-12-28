#ifndef CLI_MAIN
#define CLI_MAIN

#include <QtCore>
#include <QString>
#include <QTextStream>
#include <iostream>

class CLI_Main : public QObject
{
	Q_OBJECT

	QTextStream cout;
	QTextStream cin;

public:
	CLI_Main(QObject *parent = 0) : QObject(parent), cout(stdout), cin(stdin) {}

public slots:
	void run(){
		cout << "Filename: ";
		cout.flush();

		QString str;
		cin >> str;

		emit finished();
	}

signals:
	void finished();
};

#endif // CLI_MAIN
