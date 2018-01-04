#ifndef CLI_MAIN
#define CLI_MAIN

#include <QtCore>

class QMediaPlayer;
class QMediaPlaylist;
class QString;

class Player : public QObject
{
	Q_OBJECT

	QMediaPlayer *player;
	QMediaPlaylist *playlist;

public:
	Player(QObject *parent = 0);

public slots:
	void processLine(QString str);

	void addToPlaylist(const QList<QUrl> urls);
	void jump(int index);
	// void playlistPositionChanged(int currentItem);
	void seek(double secs);
	void setPlaybackRate(double rate);
	// void statusChanged(QMediaPlayer::MediaStatus status);
	// void stateChanged(QMediaPlayer::State state);
	void displayErrorMessage();

signals:
	void finished();
};

#endif // CLI_MAIN
