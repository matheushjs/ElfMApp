#include <QString>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QMediaMetaData>
#include <QTimer>

/* std::cout is only used in this class Player
 * std::cin, only in the class CommandReader
 * In this way, we avoid concurrently usage of these streams
 */
#include <iostream>

#include "player.h"
#include "command_reader.h"

Player::Player(const QList<QUrl> urls, QObject *parent)
  : QObject(parent)
  , player(new QMediaPlayer(this))
  , playlist(new QMediaPlaylist(this))
{
	CommandReader *reader = new CommandReader(this);

	connect(reader, SIGNAL(nextRequest(int)), this, SLOT(next(int)));
	connect(reader, SIGNAL(backRequest(int)), this, SLOT(back(int)));
	connect(reader, SIGNAL(rateRequest(double)), this, SLOT(setPlaybackRate(double)));
	connect(reader, SIGNAL(quitRequest()), this, SIGNAL(finished()));
	connect(reader, SIGNAL(advanceRequest(double)), this, SLOT(forward(double)));
	connect(reader, SIGNAL(rewindRequest(double)), this, SLOT(rewind(double)));
	connect(reader, SIGNAL(removeRequest()), this, SLOT(deleteCurrentFromDrive()));
	connect(reader, SIGNAL(listRequest()), this, SLOT(listPlaylist()));
	connect(reader, SIGNAL(selectRequest(int)), this, SLOT(setSong(int)));
	connect(reader, &CommandReader::errorMessage, [](QString str){ std::cout << str.toStdString() << "\n"; });
	connect(reader, &CommandReader::helpRequest, [reader](){ std::cout << reader->helpMessage().toStdString() << "\n"; });

	player->setPlaylist(playlist);
	player->setVolume(100);

	infoPlayers.fill(new QMediaPlayer(this), urls.size());

	for(int i = 0; i < urls.size(); i++){
		playlist->addMedia(urls[i]);
		infoPlayers[i]->setMedia(urls[i]);
	}

	reader->start();
	QTimer::singleShot(0, this, [this](){ this->setSong(0); });
}

int Player::currentIndex() const {
	return playlist->currentIndex();
}

int Player::mediaCount() const {
	return playlist->mediaCount();
}

void Player::next(int jump){
	if(jump <= 0) return;
	setSong( (currentIndex() + jump) % mediaCount() );
}

void Player::back(int jump){
	if(jump <= 0) return;
	int idx = currentIndex() - jump;
	while(idx < 0) idx += mediaCount();
	setSong(idx);
}

void Player::setPlaybackRate(double rate){
	if(rate < 0 || rate >= 5) return;

	int pos = player->position();
	int index = currentIndex();
	player->pause();
	player->setPlaybackRate(rate);
	playlist->setCurrentIndex(index);
	player->setPosition(pos);
	player->play();
}

void Player::forward(double step){
	if(step < 0) return;
	player->setPosition(player->position() + step * 1000);
}

void Player::rewind(double step){
	if(step < 0) return;
	player->setPosition(player->position() - step * 1000);
}

void Player::deleteCurrentFromDrive(){
	std::cerr << "To be implemented\n";
}

void Player::listPlaylist(){
	int i = 0;
	for(QMediaPlayer *&p: infoPlayers){
		if(p->isMetaDataAvailable()){
			std::cout << i++ << ": " << p->metaData(QMediaMetaData::Title).toString().toStdString() << "\n";
		} else {
			std::cout << i++ << ": MData Unavailable\n";
		}
	}
}

void Player::setSong(int index){
	if(index < 0 || index >= mediaCount()) return;
	player->pause();
	playlist->setCurrentIndex(index);
	player->play();
}

void Player::displayErrorMessage(){
	std::cerr << player->errorString().toStdString() << "\n";
}
