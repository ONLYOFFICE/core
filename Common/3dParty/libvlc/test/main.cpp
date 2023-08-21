#include <QApplication>
#include "player.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    Mwindow player;
    player.show();

    return app.exec();
}
