#include "player.h"

#define qtu(i) ((i).toUtf8().constData())

#include <QtGui>
#include <QMessageBox>
#include <QMenuBar>
#include <QAction>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QUrl>

Mwindow::Mwindow()
{
	// create vlc player
	vlcPlayer = new CVlcPlayer();

	/* Interface initialization */
	initUI();
}

Mwindow::~Mwindow()
{
	delete vlcPlayer;
}

void Mwindow::initUI()
{

	/* Menu */
	QMenu *fileMenu = menuBar()->addMenu("&File");
	QMenu *editMenu = menuBar()->addMenu("&Edit");

	QAction *Open = new QAction("&Open", this);
	QAction *Quit = new QAction("&Quit", this);
	QAction *playAc = new QAction("&Play/Pause", this);
	QAction *fsAc = new QAction("&Fullscreen", this);
	QAction *aboutAc = new QAction("&About", this);

	Open->setShortcut(QKeySequence("Ctrl+O"));
	Quit->setShortcut(QKeySequence("Ctrl+Q"));

	fileMenu->addAction(Open);
	fileMenu->addAction(aboutAc);
	fileMenu->addAction(Quit);
	editMenu->addAction(playAc);
	editMenu->addAction(fsAc);

	connect(Open, SIGNAL(triggered()), this, SLOT(openFile()));
	connect(playAc, SIGNAL(triggered()), this, SLOT(play()));
	connect(aboutAc, SIGNAL(triggered()), this, SLOT(about()));
	connect(fsAc, SIGNAL(triggered()), this, SLOT(fullscreen()));
	connect(Quit, SIGNAL(triggered()), qApp, SLOT(quit()));

	/* Buttons for the UI */
	playBut = new QPushButton("Play");
	QObject::connect(playBut, SIGNAL(clicked()), this, SLOT(play()));

	QPushButton *stopBut = new QPushButton("Stop");
	QObject::connect(stopBut, SIGNAL(clicked()), this, SLOT(stop()));

	QPushButton *muteBut = new QPushButton("Mute");
	QObject::connect(muteBut, SIGNAL(clicked()), this, SLOT(mute()));

	QPushButton *fsBut = new QPushButton("Fullscreen");
	QObject::connect(fsBut, SIGNAL(clicked()), this, SLOT(fullscreen()));

	volumeSlider = new QSlider(Qt::Horizontal);
	QObject::connect(volumeSlider, SIGNAL(sliderMoved(int)), this, SLOT(changeVolume(int)));
	volumeSlider->setValue(80);

	slider = new QSlider(Qt::Horizontal);
	slider->setMaximum(1000);
	QObject::connect(slider, SIGNAL(sliderMoved(int)), this, SLOT(changePosition(int)));

	/* A timer to update the sliders */
	connect(vlcPlayer, SIGNAL(positionChanged(float)), this, SLOT(updateInterface(float)));

	/* Central Widgets */
	QWidget *centralWidget = new QWidget;
	videoWidget = new QWidget;

	videoWidget->setAutoFillBackground(true);
	QPalette plt = palette();
	plt.setColor(QPalette::Window, Qt::black);
	videoWidget->setPalette(plt);

	/* Put all in layouts */
	QHBoxLayout *layout = new QHBoxLayout;
	//	layout->setMargin(0);
	layout->addWidget(playBut);
	layout->addWidget(stopBut);
	layout->addWidget(muteBut);
	layout->addWidget(fsBut);
	layout->addWidget(volumeSlider);

	QVBoxLayout *layout2 = new QVBoxLayout;
	//	layout2->setMargin(0);
	layout2->addWidget(videoWidget);
	layout2->addWidget(slider);
	layout2->addLayout(layout);

	centralWidget->setLayout(layout2);
	setCentralWidget(centralWidget);
	resize(600, 400);

	/* Integrate the video in the interface */
	vlcPlayer->integrateIntoWidget(videoWidget);
}

void Mwindow::openFile()
{

	/* The basic file-select box */
	QString sFile = QFileDialog::getOpenFileName(this, tr("Load a file"));

	/* Create a new Media */
	CVlcMedia* pMedia = new CVlcMedia(GetVlcInstance(), sFile, false);

	/* Open media and start playback */
	vlcPlayer->open(pMedia);

	/* Release the media */
	delete pMedia;

	/* Update playback button */
	playBut->setText("Pause");
}

void Mwindow::play()
{
	if (!vlcPlayer)
		return;

	if (vlcPlayer->isPlaying())
	{
		/* Pause */
		vlcPlayer->pause();
		playBut->setText("Play");
	}
	else
	{
		/* Play again */
		vlcPlayer->play();
		playBut->setText("Pause");
	}
}

int Mwindow::changeVolume(int vol)
{ /* Called on volume slider change */

	vlcPlayer->setVolume(vol);
	return 0;
}

void Mwindow::changePosition(int pos)
{ /* Called on position slider change */
	vlcPlayer->setPosition(static_cast<float>(pos) / 1000.0);
}

void Mwindow::updateInterface(float fNewPos)
{ // Update interface and check if song is finished
	if (!vlcPlayer)
		return;

	/* update the timeline */
	slider->setValue(static_cast<int>(fNewPos * 1000.0));

	/* Stop the media if needed */
	if (vlcPlayer->getState() == libvlc_Ended)
		this->stop();
}

void Mwindow::stop()
{
	if (vlcPlayer)
	{
		/* stop the media player */
		vlcPlayer->stop();

		/* Reset application values */
		slider->setValue(0);
		playBut->setText("Play");
	}
}

void Mwindow::mute()
{
	if (vlcPlayer)
	{
		if (volumeSlider->value() == 0)
		{ // if already muted...

			this->changeVolume(80);
			volumeSlider->setValue(80);
		}
		else
		{ // else mute volume

			this->changeVolume(0);
			volumeSlider->setValue(0);
		}
	}
}

void Mwindow::about()
{
	QMessageBox::about(this, "Qt libVLC player demo", QString::fromUtf8(libvlc_get_version()));
}

void Mwindow::fullscreen()
{
	if (isFullScreen())
	{
		showNormal();
		menuWidget()->show();
	}
	else
	{
		showFullScreen();
		menuWidget()->hide();
	}
}

void Mwindow::closeEvent(QCloseEvent *event)
{
	vlcPlayer->stop();
	event->accept();
}
