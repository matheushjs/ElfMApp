#ifndef CLI_MAIN
#define CLI_MAIN

#include <QtCore>
#include <QVector>
#include <QMediaPlayer>

class QMediaPlaylist;
class CommandReader;

class Player : public QObject
{
	Q_OBJECT

	QMediaPlayer *player;
	QMediaPlaylist *playlist;
	CommandReader *reader;

	// Players for holding metadata of each song
	QVector<QMediaPlayer *> infoPlayers;

	QString durationString(int ms) const;

public:
	Player(const QList<QUrl> &urls, QObject *parent = 0);
	~Player();
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
    void rename(void);
	void listPlaylist(void);
	void setSong(int index);
	void showInfo(void);

	// Slots for the MediaPlayer
	void displayErrorMessage(void);
	void onStatusChanged(QMediaPlayer::State status);
	void onMediaStatusChanged(QMediaPlayer::MediaStatus status);

signals:
	void finished(void);
};

#endif // CLI_MAIN
