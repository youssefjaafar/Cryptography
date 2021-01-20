#include <QApplication>
#include "mainwindow.h"
#include "rc4.h"

/**
 *\mainpage Gameboi
 *\author Marwan Ghamlouch - Youssef Jaafar
 *\date 11-12-2020
 *
 */
// A5 Encryption scheme:
// The algorithm takes228 bits of plain text as input and outputs 228 bits of
// cipher text.  Each block of 228 bits is called a ”frame”, where the first 114
// bits represents data sent from unit. A to unit B, and the last 114 bits are data received by
//unit A from unit B. Each frame has a duration of 4.615ms, allowing 2ˆ8 frames to be sent every second


// Although the A5 encryption is a simple XOR with a key stream, it is the generation of the key stream that makes it secure

// A5/1 consist of 6 steps, where the first 5 steps generatethe pseudo random key stream,
// and the last step appliesthe key stream to the plain-text, thereby generating thecipher text.
int main(int argc, char **argv)
{
    QApplication app (argc, argv);

    mainWindow *window = new mainWindow();
    window->show();
    return app.exec();
}
