// main.cpp
#include <QtCore>
#include <QString>
#include <QList>

#include <iostream>

#include "player.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	if(argc == 1){
		std::cout << "Usage: " << argv[0] << " [song files]\n";
		return 1;
	}

	QList<QUrl> list;
	for(int i = 1; i < argc; i++){
		QUrl path = QUrl::fromLocalFile(QDir().absoluteFilePath(QString(argv[i])));
		list.push_back(path);
	}

	Player *cli = new Player(list, &a);
	QObject::connect(cli, SIGNAL(finished()), &a, SLOT(quit()));

	return a.exec();
}
