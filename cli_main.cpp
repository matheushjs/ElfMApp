#include <QString>

#include <iostream>

#include "cli_main.h"
#include "command_reader.h"

CLI_Main::CLI_Main(QObject *parent)
  : QObject(parent),
	player(new QMediaPlayer(this))
{
	CommandReader *reader = new CommandReader(this);

	connect(reader, &CommandReader::gotLine, this, &CLI_Main::processLine, Qt::BlockingQueuedConnection);

	reader->start();
}

void CLI_Main::processLine(QString str){
	std::cout << str.toStdString() << "\n";

	QString path = QDir().absoluteFilePath("Lasting.mp3");

	if(player->state() == QMediaPlayer::PlayingState){
		return;
	}

	player->setMedia(QUrl::fromLocalFile(path));
	player->setVolume(50);
	player->setPlaybackRate(5.0);
	player->play();

	connect(player, SIGNAL(stateChanged(QMediaPlayer::State)), this, SIGNAL(finished()));
}
