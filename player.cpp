#include <QString>
#include <QMediaPlayer>

#include <iostream>

#include "player.h"
#include "command_reader.h"

Player::Player(QObject *parent)
  : QObject(parent),
	player(new QMediaPlayer(this))
{
	CommandReader *reader = new CommandReader(this);

	connect(reader, &CommandReader::gotLine, this, &Player::processLine, Qt::BlockingQueuedConnection);

	reader->start();
}

void Player::processLine(QString str){
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
