


#include <QApplication>

#include <QDebug>

#include "TMainWnd.h"



int main(int argc, char *argv[])
{
    QApplication app(argc, argv);



    TMainWnd wnd;

    wnd.show();



    return app.exec();
}
