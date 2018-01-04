#include <QString>
#include <QMediaPlayer>
#include <QMediaPlaylist>

#include <iostream>

#include "player.h"
#include "command_reader.h"

Player::Player(QObject *parent)
  : QObject(parent)
  , player(new QMediaPlayer(this))
  , playlist(new QMediaPlaylist(this))
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

void Player::addToPlaylist(const QList<QUrl> urls){
	for( const QUrl &url: urls){
		playlist->addMedia(url);
	}
}

void Player::jump(int index){
	int count = playlist->mediaCount();
	if(index < count && index >= 0){
		playlist->setCurrentIndex(index);
		player->play();
	}
}

void Player::seek(double secs){
	player->setPosition( (int) (secs * 1000) );
}

void Player::setPlaybackRate(double rate){
	player->setPlaybackRate(rate);
}

void Player::displayErrorMessage(){
	std::cerr << player->errorString().toStdString() << "\n";
}
