#ifndef PLAYER
#define PLAYER

#include "../vlcplayer.h"

#include <QMainWindow>
#include <QPushButton>
#include <QSlider>
#include <QWidget>

class Mwindow : public QMainWindow
{
	Q_OBJECT

public:
	Mwindow();
	virtual ~Mwindow();

private slots:
	void openFile();
	void play();
	void stop();
	void mute();
	void about();
	void fullscreen();

	int changeVolume(int);
	void changePosition(int);
	void updateInterface(float);

protected:
	virtual void closeEvent(QCloseEvent *);

private:
	QPushButton *playBut;
	QSlider *volumeSlider;
	QSlider *slider;
	QWidget *videoWidget;

	CVlcPlayer* vlcPlayer;

	void initUI();
};

#endif
