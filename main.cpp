// main.cpp
#include <QtCore>
#include <QString>

#include <iostream>

#include "player.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	if(argc != 2){
		std::cerr << "Expected arguments.\n";
		return 1;
	}

	QUrl path = QUrl::fromLocalFile(QDir().absoluteFilePath(QString(argv[1])));
	std::cout << path.toString().toStdString() << "\n";

	Player *cli = new Player(QList<QUrl>({ path }), &a);

	QObject::connect(cli, SIGNAL(finished()), &a, SLOT(quit()));

	return a.exec();
}
