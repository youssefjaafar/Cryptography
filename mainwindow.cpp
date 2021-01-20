#include "mainwindow.h"
#include "a5interface.h"
#include "rc4.h"


mainWindow::mainWindow(QWidget *parent) : QWidget(parent)
{

    title = new QLabel("Choose the stream cipher you'd like to explore:");

    A5button = new QPushButton("A5");
    RC4button = new QPushButton("RC4");

    topGrid = new QGridLayout;
    mainV = new QVBoxLayout;

    //start of top grid
    mainV->addItem(topGrid);
    mainV->addWidget(title,1,0);
    mainV->addWidget(A5button,2,0);
    mainV->addWidget(RC4button,3,0);

    setLayout(mainV);

    QObject::connect(A5button, SIGNAL(clicked(bool)), this, SLOT(gotoA5()));
    QObject::connect(RC4button, SIGNAL(clicked(bool)), this, SLOT(gotoRC4()));
}

void mainWindow::gotoA5(){


    A5Interface *window = new A5Interface();
    window->show();
    this->close();
    this->deleteLater();

}

void mainWindow::gotoRC4(){
    QPointer<RC4> w = new RC4();
    w->show();
    this->close();
}
