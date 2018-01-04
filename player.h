#ifndef CLI_MAIN
#define CLI_MAIN

#include <QtCore>

class QMediaPlayer;
class QString;

class Player : public QObject
{
	Q_OBJECT

	QMediaPlayer *player;

public:
	Player(QObject *parent = 0);

public slots:
	void processLine(QString str);

signals:
	void finished();
};

#endif // CLI_MAIN
