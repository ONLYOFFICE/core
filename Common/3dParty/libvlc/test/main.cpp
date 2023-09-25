#include <QApplication>
#include "player.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

	// set lowest verbosity level
	CVlcInstance::setVerbosityLevel(0);
    Mwindow player;
    player.show();

    return app.exec();
}
