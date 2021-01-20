#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QWidget>
#include <QtWidgets>

#include "a5interface.h"


class mainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit mainWindow(QWidget *parent = nullptr);
    QLabel* title;

    QPushButton* A5button;
    QPushButton* RC4button;

    QGridLayout* topGrid;
    QVBoxLayout* mainV;

public slots:
    void gotoA5();
    void gotoRC4();


};

#endif // MAINWINDOW_H
