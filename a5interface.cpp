#include "a5interface.h"
#include "a5.h"
#include "qdebug.h"
#include <QThread>

A5Interface::A5Interface(QWidget *parent) :QWidget(parent)
{
    step = 0;
    startButton = new QPushButton("Let's go!");

    sessionKey = new QLabel("Session Key (hex): ");
    sessionKeyField = new QLineEdit();

    showSteps = new QCheckBox("Show steps");
    showSteps->setLayoutDirection(Qt::RightToLeft);

    frameNumber = new QLabel("Frame Number (hex) : ");
    frameNumberField = new QLineEdit();

    messageToEncrypt = new QLabel("Insert plaintext: ");
    messageToEncryptField = new QLineEdit();
    messageToEncryptField->setPlaceholderText("Optional");


    messageToDecrypt = new QLabel(" Insert ciphertext: ");
    messageToDecryptField = new QLineEdit();
    messageToDecryptField->setPlaceholderText("Optional");

    encryptionKey = new QLabel("Generated Encryption Key:");
    encryptionKey->setVisible(false);

    decryptionKey = new QLabel("Generated Decryption Key:");
    decryptionKey->setVisible(false);

    encryptedText = new QLabel(); //only if message exists
    decryptedText = new QLabel(); //only if message exists


    error = new QLabel("Error:");
    error->setStyleSheet("QLabel {color : red;}");
    error->setVisible(false);

    topGrid = new QGridLayout;
    mainV = new QVBoxLayout;

    //start of top grid

    topGrid->addWidget(sessionKey,0,0);
    topGrid->addWidget(sessionKeyField,0,1,1,2);

    topGrid->addWidget(showSteps,0,3);

    topGrid->addWidget(frameNumber,1,0);
    topGrid->addWidget(frameNumberField,1,1,1,2);

    topGrid->addWidget(messageToEncrypt,2,0);
    topGrid->addWidget(messageToEncryptField,2,1);

    topGrid->addWidget(messageToDecrypt,2,2);
    topGrid->addWidget(messageToDecryptField,2,3);

    topGrid->addWidget(encryptionKey,3,0,1,3);
    topGrid->addWidget(decryptionKey,4,0,1,3);

    topGrid->addWidget(encryptedText,5,0,1,3);
    topGrid->addWidget(decryptedText,6,0,1,3);


    mainV->addItem(topGrid);
    mainV->addWidget(error,1,0);
    mainV->addWidget(startButton,2,0);



    // widgets concerning showing steps
    stepDesc = new QLabel("Step 0: Reset Registers");
    stepDesc->setVisible(false);
    stepDesc->setAlignment(Qt::AlignCenter);



    legend = new QGridLayout();
    legend->setSpacing(10);

    QLabel* legendLabel = new QLabel("Legend: ");
    legendLabel->setVisible(false);
    QLabel* tappedLabel = new QLabel("Tapped Bit");
    tappedLabel->setStyleSheet("QLabel {background-color: #a9c4eb}");
    tappedLabel->setAlignment(Qt::AlignCenter);
    tappedLabel->setVisible(false);
    QLabel* clockingLabel = new QLabel("Clocking Bit");
    clockingLabel->setStyleSheet("QLabel {background-color: red}");
    clockingLabel->setAlignment(Qt::AlignCenter);
    clockingLabel->setVisible(false);

    legend->addWidget(legendLabel,0,0);
    legend->addWidget(tappedLabel,0,1);
    legend->addWidget(clockingLabel,0,2);

    //LFSR1 representation
    LFSR1Grid = new QGridLayout();

    QLabel* LFSR1Label = new QLabel("LFSR 1: ");
    LFSR1Label->setAlignment(Qt::AlignCenter);
    LFSR1Label->setVisible(false);
    LFSR1Grid->addWidget(LFSR1Label,0,0);

    for (int i = 0; i <= 18; i++){
        QLabel* slot = new QLabel("0");
        if (i == 8){
            slot->setStyleSheet("QLabel {border : 1px solid black; background-color: red}");
        }
        else if (i == 18 || i == 17 || i == 16 || i == 13){
            slot->setStyleSheet("QLabel {border : 1px solid black; background-color: #a9c4eb}");
        }
         else{
            slot->setStyleSheet("QLabel {border : 1px solid black;}");
            }
        LFSR1.push_back(slot);
        slot->setVisible(false);
        slot->setAlignment(Qt::AlignCenter);
        LFSR1Grid->addWidget(slot,0,18-i + 1);
    }


    LFSR2Grid = new QGridLayout();

    QLabel* LFSR2Label = new QLabel("LFSR 2: ");
    LFSR2Label->setAlignment(Qt::AlignCenter);
    LFSR2Label->setVisible(false);
    LFSR2Grid->addWidget(LFSR2Label,0,0);

    for (int i = 0; i <= 21; i++){
        QLabel* slot = new QLabel("0");

        if (i == 10){ //clocking bit
            slot->setStyleSheet("QLabel {border : 1px solid black; background-color: red}");
        }
        else if (i == 20 || i == 21){
            slot->setStyleSheet("QLabel {border : 1px solid black; background-color: #a9c4eb}");
        }
         else {
            slot->setStyleSheet("QLabel {border : 1px solid black;}");
        }

        LFSR2.push_back(slot);
        slot->setVisible(false);
        slot->setAlignment(Qt::AlignCenter);
        LFSR2Grid->addWidget(slot,0,21-i+1);
    }

    LFSR3Grid = new QGridLayout();

    QLabel* LFSR3Label = new QLabel("LFSR 3: ");
    LFSR3Label->setAlignment(Qt::AlignCenter);
    LFSR3Label->setVisible(false);
    LFSR3Grid->addWidget(LFSR3Label,0,0);

    for (int i = 0; i <= 22; i++){
        QLabel* slot = new QLabel("0");
        if (i == 10){
            slot->setStyleSheet("QLabel {border : 1px solid black; background-color: red}");
        }
        else if (i == 22 || i == 21 || i == 20 || i == 17){
            slot->setStyleSheet("QLabel {border : 1px solid black; background-color: #a9c4eb}");
        }
         else{
            slot->setStyleSheet("QLabel {border : 1px solid black;}");
            }
        LFSR3.push_back(slot);
        slot->setVisible(false);
        slot->setAlignment(Qt::AlignCenter);
        LFSR3Grid->addWidget(slot,0,22-i + 1);
    }


    nextStepButton = new QPushButton("Next Step");
    nextStepButton->setVisible(false);

    mainV->addWidget(stepDesc);
    mainV->addItem(legend);
    mainV->addItem(LFSR1Grid);
    mainV->addItem(LFSR2Grid);
    mainV->addItem(LFSR3Grid);
    mainV->addWidget(nextStepButton);

    setLayout(mainV);

    a5 = new A5();

   QObject::connect(nextStepButton, SIGNAL(clicked(bool)), this, SLOT(nextStep()));
   QObject::connect(startButton, SIGNAL(clicked(bool)), this, SLOT(start()));

}

bool A5Interface::start(){

    //reset all values concerned with viusals
    step = 0;
    nextStepButton->setText("Next Step");
    stepDesc->setText("Step 0: Reset Registers");
    this->updateVisuals();//in the case the steps were being shown

    error->setVisible(false);
    encryptedText->setVisible(false);
    decryptedText->setVisible(false);

    if (!validate()){ //if the input is invalid
        return false; //return flase
    }

    bool convert;

    this->a5->sessionKey = this->sessionKeyField->text().toULongLong(&convert, 16);
    this->a5->frameNumber = this->frameNumberField->text().toULongLong(&convert, 16);

    QDebug deb = qDebug().nospace();

    deb << step;
    deb << "\n";

    deb << this->a5->sessionKey << " ";
    deb << this->a5->frameNumber;

    this->a5->resetRegisters();
    this->a5->useSessionKey();
    this->a5->useFrameNumber();
    this->a5->irregularClocking();
    this->a5->generateKeyStream();



    if(!messageToEncryptField->text().isEmpty()){
        encryptedText->setText("Encrypted Message: 0x" + this->a5->encrypt(messageToEncryptField->text()));
        encryptedText->setVisible(true);
    }

    if(!messageToDecryptField->text().isEmpty()){
        decryptedText->setText("Decrypted Message: 0x" + this->a5->decrypt(messageToDecryptField->text()));
        decryptedText->setVisible(true);
    }

    deb << "\n";

    //get encryption stream
    QString EncryptionStream;
    for (int i = 227; i >= 114; i = i - 4){
        QString binaryString = QString::number(this->a5->keyStream[i])+QString::number(this->a5->keyStream[i-1])+QString::number(this->a5->keyStream[i-2])+QString::number(this->a5->keyStream[i-3]);
        bool fOk;
        int iValue = binaryString.toInt(&fOk, 2);  //2 is the base
        binaryString = QString::number(iValue, 16);  //The new base is 16
        EncryptionStream += binaryString;
    }
    //get the last two bits
    QString binaryString = QString::number(this->a5->keyStream[115])+QString::number(this->a5->keyStream[114])+"00";
    bool fOk;
    int iValue = binaryString.toInt(&fOk, 2);  //2 is the base
    binaryString = QString::number(iValue, 16);  //The new base is 16
    EncryptionStream += binaryString;

    deb << EncryptionStream;
    encryptionKey->setText("Generated Encryption Key: 0x" + EncryptionStream);
    encryptionKey->setVisible(true);

    deb << "\n";


    QString DecryptionStream;
    for (int i = 113; i >= 0; i = i - 4){
        QString binaryString = QString::number(this->a5->keyStream[i])+QString::number(this->a5->keyStream[i-1])+QString::number(this->a5->keyStream[i-2])+QString::number(this->a5->keyStream[i-3]);
        bool fOk;
        int iValue = binaryString.toInt(&fOk, 2);  //2 is the base
        binaryString = QString::number(iValue, 16);  //The new base is 16
        DecryptionStream += binaryString;
    }
    //get the last two bits
    binaryString = QString::number(this->a5->keyStream[1])+QString::number(this->a5->keyStream[0])+"00";
    iValue = binaryString.toInt(&fOk, 2);  //2 is the base
    binaryString = QString::number(iValue, 16);  //The new base is 16
    DecryptionStream += binaryString;

    deb << DecryptionStream;
    decryptionKey->setText("Generated Decryption Key: 0x" + DecryptionStream);
    decryptionKey->setVisible(true);

    //Show steps ...............or not?

    if (showSteps->isChecked())
    {
        stepDesc->setVisible(true);

        legend->itemAtPosition(0,0)->widget()->setVisible(true);
        legend->itemAtPosition(0,1)->widget()->setVisible(true);
        legend->itemAtPosition(0,2)->widget()->setVisible(true);

        LFSR1Grid->itemAtPosition(0,0)->widget()->setVisible(true);
        LFSR2Grid->itemAtPosition(0,0)->widget()->setVisible(true);
        LFSR3Grid->itemAtPosition(0,0)->widget()->setVisible(true);

        for (int i = 0; i <= 18; i++){
            LFSR1[i]->setVisible(true);
        }

        for (int i = 0; i <= 21; i++){
            LFSR2[i]->setVisible(true);
        }

        for (int i = 0; i <= 22; i++){
            LFSR3[i]->setVisible(true);
        }

        nextStepButton->setVisible(true);
    }
    else {

        stepDesc->setVisible(false);

        legend->itemAtPosition(0,0)->widget()->setVisible(false);
        legend->itemAtPosition(0,1)->widget()->setVisible(false);
        legend->itemAtPosition(0,2)->widget()->setVisible(false);

        LFSR1Grid->itemAtPosition(0,0)->widget()->setVisible(false);
        LFSR2Grid->itemAtPosition(0,0)->widget()->setVisible(false);
        LFSR3Grid->itemAtPosition(0,0)->widget()->setVisible(false);

        for (int i = 0; i <= 18; i++){
            LFSR1[i]->setVisible(false);
        }

        for (int i = 0; i <= 21; i++){
            LFSR2[i]->setVisible(false);
        }

        for (int i = 0; i <= 22; i++){
            LFSR3[i]->setVisible(false);
        }

        nextStepButton->setVisible(false);
    }


    return true;
}


void A5Interface::nextStep(){
    //show steps
//    this->a5->resetRegisters();//reset registers
    if (step == 0){
    this->a5->resetRegisters();
    stepDesc->setText("Step 0: Reset Registers");
    this->updateVisuals();
    }

    if (step == 1){
    stepDesc->setText("Step 1: Clocking in the Session Key");
    this->a5->useSessionKey();
    this->updateVisuals();
    }

    if (step == 2){
    stepDesc->setText("Step 2: Clocking in the Frame Number");
    this->a5->useFrameNumber();
    this->updateVisuals();
    }

    if (step == 3){
    stepDesc->setText("Step 3: Irregularly Clock the registers");
    this->a5->irregularClocking();
    this->updateVisuals();
    }

    if (step == 4){
    stepDesc->setText("Step 4: Generate the Key Stream");
    this->a5->generateKeyStream();
    this->updateVisuals();
    nextStepButton->setText("Done!");
    }

    step++;
};

void A5Interface::updateVisuals(){

    for (int i = 0; i <= 18; i++){
        LFSR1[i]->setText(QString::number(this->a5->LFSR1[i]));
    }

    for (int i = 0; i <= 21; i++){
        LFSR2[i]->setText(QString::number(this->a5->LFSR2[i]));
    }

    for (int i = 0; i <= 22; i++){
        LFSR3[i]->setText(QString::number(this->a5->LFSR3[i]));
    }
}


bool A5Interface::validate()
{
    //check if a field is missing
    if (sessionKeyField->text().isEmpty() || frameNumberField->text().isEmpty()){
    error->setText("Error: Session Key and Frame Number are required fields");
    error->setVisible(true);
    return false;
    }

    QRegularExpression hexMatcher("^(0x)?[0-9A-F]{1,16}$",
                                  QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch match = hexMatcher.match(sessionKeyField->text());
    if (!match.hasMatch())
    {
        error->setText("Error: Please enter a valid session key. Max length is 64 bits.");
        error->setVisible(true);
        return false;
    }

    QRegularExpression hexMatcherFrame("^(0x)?[0-9A-F]{1,6}$",
                                  QRegularExpression::CaseInsensitiveOption);
    match = hexMatcherFrame.match(frameNumberField->text());
    if (!match.hasMatch())
    {
        error->setText("Error: Please enter a valid frame number. Max length is 64 bits.");
        error->setVisible(true);
        return false;
    }

    return true;
};
