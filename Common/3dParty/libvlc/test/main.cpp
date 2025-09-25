#include <QApplication>
#include "player.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	// set non-zero verbosity level
	CVlcInstance::setVerbosityLevel(1);
	Mwindow player;
	player.show();

	return app.exec();
}
