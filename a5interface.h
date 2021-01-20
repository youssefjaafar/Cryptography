#ifndef A5INTERFACE_H
#define A5INTERFACE_H

#include <QWidget>
#include <QObject>
#include <QWidget>
#include <QtWidgets>
#include "a5.h"


class A5Interface : public QWidget
{
    Q_OBJECT
public:
    explicit A5Interface(QWidget *parent = nullptr);
    QLabel* sessionKey; //required
    QLabel* frameNumber; //required
    QLabel* error;

    QLabel* messageToEncrypt;
    QLabel* messageToDecrypt;

    QLabel* encryptionKey; //generated
    QLabel* decryptionKey; //generated

    QLabel* encryptedText; //only if message exists
    QLabel* decryptedText; //only if message exists

    QCheckBox* showSteps;

    QLineEdit* sessionKeyField;
    QLineEdit* frameNumberField;

    QLineEdit* messageToEncryptField;
    QLineEdit* messageToDecryptField;

    QPushButton* startButton;
    QGridLayout* topGrid;
    QVBoxLayout* mainV;

    A5* a5;

    QGridLayout* legend;

    QGridLayout* LFSR1Grid;
    QGridLayout* LFSR2Grid;
    QGridLayout* LFSR3Grid;

    QLabel* stepDesc;
    int step;

    std::vector<QLabel*> LFSR1;
    std::vector<QLabel*> LFSR2;
    std::vector<QLabel*> LFSR3;
    QPushButton* nextStepButton;


public slots:
    void updateVisuals(); //update the state of the displayed LSFRs
    bool validate(); //checks if value inserted are valid
    bool start();
    void nextStep();

signals:

};

#endif // A5INTERFACE_H
