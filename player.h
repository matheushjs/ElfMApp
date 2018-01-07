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
	int currentIndex() const;
	int mediaCount() const;

public slots:
	void addToPlaylist(const QList<QUrl> urls);

	void next(int jump);
	void back(int jump);
	void setPlaybackRate(double rate);
	void forward(double step);
	void rewind(double step);
	void deleteCurrentFromDrive(void);
	void listPlaylist(void);
	void setSong(int index);
	void displayErrorMessage(void);

signals:
	void finished(void);
};

#endif // CLI_MAIN
