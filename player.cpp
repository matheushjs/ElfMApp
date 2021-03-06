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

Player::Player(const QList<QUrl> &urls, QObject *parent)
  : QObject(parent)
  , player(new QMediaPlayer(this))
  , playlist(new QMediaPlaylist(this))
  , reader(new CommandReader(this))
{
	connect(reader, SIGNAL(nextRequest(int)), this, SLOT(next(int)));
	connect(reader, SIGNAL(backRequest(int)), this, SLOT(back(int)));
	connect(reader, SIGNAL(rateRequest(double)), this, SLOT(setPlaybackRate(double)));
	connect(reader, SIGNAL(quitRequest()), this, SIGNAL(finished()));
	connect(reader, SIGNAL(advanceRequest(double)), this, SLOT(forward(double)));
	connect(reader, SIGNAL(rewindRequest(double)), this, SLOT(rewind(double)));
	connect(reader, SIGNAL(removeRequest()), this, SLOT(deleteCurrentFromDrive()));
    connect(reader, SIGNAL(renameRequest()), this, SLOT(rename()));
	connect(reader, SIGNAL(listRequest()), this, SLOT(listPlaylist()));
	connect(reader, SIGNAL(selectRequest(int)), this, SLOT(setSong(int)));
	connect(reader, SIGNAL(infoRequest()), this, SLOT(showInfo()));
	connect(reader, &CommandReader::errorMessage, [](QString str){ std::cout << "\n" << str.toStdString() << "\n"; });
	connect(reader, &CommandReader::helpRequest, [this](){ std::cout << this->reader->helpMessage().toStdString() << "\n"; });

	connect(player, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(onStatusChanged(QMediaPlayer::State)));
	connect(player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(onMediaStatusChanged(QMediaPlayer::MediaStatus)));

	playlist->setPlaybackMode(QMediaPlaylist::Loop);
	player->setPlaylist(playlist);
	player->setVolume(100);

	for(int i = 0; i < urls.size(); i++)
		infoPlayers.push_back(new QMediaPlayer(this));

	for(int i = 0; i < urls.size(); i++){
		playlist->addMedia(urls[i]);
		infoPlayers[i]->setMedia(urls[i]);
	}

	reader->start();
	QTimer::singleShot(0, this, [this](){ this->setSong(0); });
}

Player::~Player(){
	reader->wait();
}

QString Player::durationString(int ms) const {
	int min = ms / (60 * 1000);
	int sec = (ms % (60 * 1000)) / 1000;
	int mili = ms % 1000;
	return QString("%1m %2s %3ms").arg(min).arg(sec).arg(mili);
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
	std::cout << "\nSetting rate to " << rate << "\n";

	int pos = player->position();
	int index = currentIndex();
	player->setPlaybackRate(rate);
	playlist->setCurrentIndex(index);
	player->setPosition(pos);
	player->play();
}

void Player::forward(double step){
	if(step < 0) return;
	player->setPosition(player->position() + step * 1000);
	std::cout << durationString(player->position()).toStdString() << " (forwarded " << step << " seconds)" << "\n";
}

void Player::rewind(double step){
	if(step < 0) return;
	player->setPosition(player->position() - step * 1000);
	std::cout << durationString(player->position()).toStdString() << " (rewinded " << step << " seconds)" << "\n";
}

void Player::deleteCurrentFromDrive(){
	std::cout << "\nRemoving media\n";
	player->stop();

	int idx = currentIndex();
	playlist->removeMedia(idx);
	QMediaPlayer *rm = infoPlayers[idx];
	infoPlayers.remove(idx);

	QMediaContent cont = rm->media();
	QFile fp(cont.canonicalUrl().toLocalFile());
	fp.remove();

	if(mediaCount() == 0){
		std::cout << "\nThere are no more media items\n";
		reader->terminate();
		emit finished();
		return;
	}

	setSong(idx % mediaCount());
}

void Player::rename(){
    std::cout << "\nRenaming media\n";
    player->stop();

    int idx = currentIndex();
    playlist->removeMedia(idx);
    QMediaPlayer *rm = infoPlayers[idx];
    infoPlayers.remove(idx);

    QMediaContent cont = rm->media();
    QFile fp(cont.canonicalUrl().toLocalFile());

    // Get the name of the file
    QString fname = fp.fileName().split("/").back();
    fname = QString("trash_") + fname;
    fp.rename(fname);

    if(mediaCount() == 0){
        std::cout << "\nThere are no more media items\n";
        reader->terminate();
        emit finished();
        return;
    }

    setSong(idx % mediaCount());
}

void Player::listPlaylist(){
	int i = 0;
	std::cout << "\nListing songs\n";
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
	std::cout << "\nPlaying song " << index << "\n";
	playlist->setCurrentIndex(index);
	player->play();
}

void Player::showInfo(void){
	QString position = durationString(player->position());
	QString duration = durationString(player->duration());

	int pos = currentIndex();
	QString title;
	if(infoPlayers[pos]->isMetaDataAvailable()){
		title = infoPlayers[pos]->metaData(QMediaMetaData::Title).toString();
	} else title = "Metadata not yet available.";

	std::cout << "\n"
			  << "\tTitle:    " << "\"" << title.toStdString() << "\"\n"
			  << "\tPosition: " << position.toStdString() << " / " << duration.toStdString() << "\n";
}

void Player::displayErrorMessage(){
	std::cerr << "\n" << player->errorString().toStdString() << "\n";
}

void Player::onStatusChanged(QMediaPlayer::State status){
	switch(status){
	case QMediaPlayer::StoppedState:
		std::cout << "Stopped\n";
		break;
	case QMediaPlayer::PlayingState:
		std::cout << "Playing\n";
		break;
	case QMediaPlayer::PausedState:
		std::cout << "Paused\n";
		break;
	}
}

void Player::onMediaStatusChanged(QMediaPlayer::MediaStatus status){
	if(status == QMediaPlayer::BufferedMedia){
		std::cout << "\nNow Playing:\n";
		std::cout << "\tTitle:     " << "\"" << player->metaData(QMediaMetaData::Title).toString().toStdString() << "\"\n";
		std::cout << "\tDuration:  " << durationString(player->duration()).toStdString() << "\n";
	}
}
