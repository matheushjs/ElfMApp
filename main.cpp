// main.cpp
#include <QtCore>

#include "player.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	Player *cli = new Player(&a);
	QObject::connect(cli, SIGNAL(finished()), &a, SLOT(quit()));

	return a.exec();
}
