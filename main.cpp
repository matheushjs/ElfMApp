// main.cpp
#include <QtCore>

#include "cli_main.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	CLI_Main *cli = new CLI_Main(&a);
	QObject::connect(cli, SIGNAL(finished()), &a, SLOT(quit()));

	return a.exec();
}
