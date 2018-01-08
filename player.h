#ifndef CLI_MAIN
#define CLI_MAIN

#include <QtCore>
#include <QVector>

class QMediaPlayer;
class QMediaPlaylist;

class Player : public QObject
{
	Q_OBJECT

	QMediaPlayer *player;
	QMediaPlaylist *playlist;

	// Players for holding metadata of each song
	QVector<QMediaPlayer *> infoPlayers;

	void addToPlaylist(const QList<QUrl> urls);

public:
	Player(const QList<QUrl> urls, QObject *parent = 0);
	int currentIndex() const;
	int mediaCount() const;

public slots:
	// Slots for responding to the user
	void next(int jump);
	void back(int jump);
	void setPlaybackRate(double rate);
	void forward(double step);
	void rewind(double step);
	void deleteCurrentFromDrive(void);
	void listPlaylist(void);
	void setSong(int index);

	// Slots for the MediaPlayer
	void displayErrorMessage(void);

signals:
	void finished(void);
};

#endif // CLI_MAIN
