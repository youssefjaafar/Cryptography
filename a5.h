#ifndef A5_H
#define A5_H

#include <QObject>

class A5 : public QObject
{
    Q_OBJECT
public:
    explicit A5(QObject *parent = nullptr);

    unsigned long long int sessionKey = 0;
    unsigned long long int frameNumber = 0;

    //these are the three LFSR we will use to genrate the key and encrypt our frames
    //we use arrays 1. for the irregular regsiter sizes and 2.easier to connect to visual elements
    int LFSR1[19] = {0};
    int LFSR2[22] = {0};
    int LFSR3[23] = {0};
    int keyStream[228] = {0};


public:
 void useSessionKey();
 void useFrameNumber();
 void irregularClocking();
 void generateKeyStream();

 QString encrypt(QString message);
 QString decrypt(QString message);

 void resetRegisters();

 void printRegisters();

};

#endif // A5_H
