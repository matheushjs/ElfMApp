#include <QString>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QTimer>

#include <iostream>

#include "player.h"
#include "command_reader.h"

Player::Player(QObject *parent)
  : QObject(parent)
  , player(new QMediaPlayer(this))
  , playlist(new QMediaPlaylist(this))
{
	CommandReader *reader = new CommandReader(this);

	connect(reader, &CommandReader::nextRequest, this, [this](int a){ this->jump(a); });
	connect(reader, &CommandReader::backRequest, this, [this](int a){ this->jump(-a); });
	connect(reader, &CommandReader::rateRequest, this, &Player::setPlaybackRate);

	player->setPlaylist(playlist);
	playlist->setCurrentIndex(0);
	player->setVolume(100);

	reader->start();
	QTimer::singleShot(0, this, [this](){ this->jumpTo(0); });
}

void Player::addToPlaylist(const QList<QUrl> urls){
	for( const QUrl &url: urls){
		playlist->addMedia(url);
	}
}

void Player::jumpTo(int index){
	int count = playlist->mediaCount();
	if(count == 0) return;

	if(index < count && index >= 0){
		playlist->setCurrentIndex(index);
		player->play();
	}
}

void Player::jump(int displacement){
	int count = playlist->mediaCount();
	int request = playlist->currentIndex() + displacement;

	if(count == 0) return;

	while(request < 0)
		request += count;

	request = request % count;
	jumpTo(request);
}

void Player::seek(double secs){
	player->setPosition( (int) (secs * 1000) );
}

void Player::setPlaybackRate(double rate){
	int pos = player->position();
	int index = playlist->currentIndex();
	player->pause();
	player->setPlaybackRate(rate);
	playlist->setCurrentIndex(index);
	player->setPosition(pos);
	player->play();
}

void Player::displayErrorMessage(){
	std::cerr << player->errorString().toStdString() << "\n";
}

int Player::currentIndex() const {
	return playlist->currentIndex();
}
