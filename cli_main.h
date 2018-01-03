#ifndef CLI_MAIN
#define CLI_MAIN

#include <QtCore>
#include <QString>
#include <QTextStream>
#include <QMediaPlayer>
#include <iostream>

#include <unistd.h>

class CLI_Main : public QObject
{
	Q_OBJECT

	QTextStream cout;
	QTextStream cin;

	QMediaPlayer *player;

public:
	CLI_Main(QObject *parent = 0) : QObject(parent), cout(stdout), cin(stdin), player(new QMediaPlayer) {}

public slots:
	void run(){
		cout << "Filename: ";
		cout.flush();

		QString str("/home/mathjs/ComputerS/projects/build-ElfMApp-Desktop-Debug/Lasting.mp3");
		//cin >> str;

		player->setMedia(QUrl::fromLocalFile(str));
		player->setVolume(50);
		player->setPlaybackRate(5.0);
		player->play();

		if(player->state() == QMediaPlayer::PlayingState){
			cout << "Yahaloo!\n";
		} else if(player->state() == QMediaPlayer::StoppedState){
			cout << "Iya da!\n";
		} else if(player->state() == QMediaPlayer::PausedState){
			cout << "maa maa\n";
		}

		connect(player, SIGNAL(stateChanged(QMediaPlayer::State)), this, SIGNAL(finished()));
	}

signals:
	void finished();
};

#endif // CLI_MAIN
