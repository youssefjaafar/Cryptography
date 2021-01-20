#include "a5.h"
#include "qdebug.h"

A5::A5(QObject *parent) : QObject(parent)
{



}

//helper functions
void shiftLeft(int a[],int n){ //helper function to shift array left
    // int temp = a[0]; //remember first element
    for(int i=n-1;i>=0;i--)
    {
        a[i+1] = a[i]; //move all element to the left except first one
    }
    a[0] = 0;
}

void A5::resetRegisters(){
    for (int i = 0; i < 228; i++){

    if (i < 19){
        LFSR1[i] = 0;
        }

    if (i < 22){
        LFSR2[i] = 0;
        }

    if (i < 23){
        LFSR3[i] = 0;
        }

    keyStream[i] = 0;
    }
}

void A5::useSessionKey(){

    for (int i = 63; i >= 0; i--){
        //Clocking LFSRs with session key

        //get the bit from k (https://stackoverflow.com/questions/2249731/how-do-i-get-bit-by-bit-data-from-an-integer-value-in-c)
        int k = i;
        unsigned long long mask =  1ULL << k;
        unsigned long long masked_n = sessionKey & mask;
        unsigned long long thebit = masked_n >> k;

        //LFSR1
        int newbit = LFSR1[18]^LFSR1[17]^LFSR1[16]^LFSR1[13]^thebit;
        shiftLeft(LFSR1,19);
        LFSR1[0] = newbit;


        //LFSR2
        newbit = LFSR2[21]^LFSR2[20]^thebit;
        shiftLeft(LFSR2,22);
        LFSR2[0] = newbit;

        //LFSR3
        newbit = LFSR3[22]^LFSR3[21]^LFSR3[20]^LFSR3[7]^thebit;
        shiftLeft(LFSR3,23);
        LFSR3[0] = newbit;
    }

}

void A5::useFrameNumber(){

    for (int i = 21; i >= 0; i--){
        //Clocking LFSRs with frame number

        //get the bit from k (https://stackoverflow.com/questions/2249731/how-do-i-get-bit-by-bit-data-from-an-integer-value-in-c)
        int k = i;
        unsigned long long mask =  1ULL << k;
        unsigned long long masked_n = frameNumber & mask;
        unsigned long long thebit = masked_n >> k;

        //LFSR1
        int newbit = LFSR1[18]^LFSR1[17]^LFSR1[16]^LFSR1[13]^thebit;
        shiftLeft(LFSR1,19);
        LFSR1[0] = newbit;


        //LFSR2
        newbit = LFSR2[21]^LFSR2[20]^thebit;
        shiftLeft(LFSR2,22);
        LFSR2[0] = newbit;

        //LFSR3
        newbit = LFSR3[22]^LFSR3[21]^LFSR3[20]^LFSR3[7]^thebit;
        shiftLeft(LFSR3,23);
        LFSR3[0] = newbit;
    }
}

void A5::irregularClocking(){

    bool majority;

    for (int i = 99; i >= 0; i--){
        //Irregular clocking LFSR based on majority rule
        if ((LFSR1[8] + LFSR2[10] + LFSR3[10]) > 1 ){
            majority = 1;
        }
        else
        {
            majority = 0;
        }


        //LFSR1
        int newbit;
        if (LFSR1[8] == majority){
            newbit = LFSR1[18]^LFSR1[17]^LFSR1[16]^LFSR1[13];
            shiftLeft(LFSR1,19);
            LFSR1[0] = newbit;
        }

        //LFSR2
        if (LFSR2[10] == majority){
            newbit = LFSR2[21]^LFSR2[20];
            shiftLeft(LFSR2,22);
            LFSR2[0] = newbit;
        }

        //LFSR3
        if (LFSR3[10] == majority){
            newbit = LFSR3[22]^LFSR3[21]^LFSR3[20]^LFSR3[7];
            shiftLeft(LFSR3,23);
            LFSR3[0] = newbit;
        }
    }

}

void A5::generateKeyStream(){
    bool majority;

    for (int i = 227; i >= 0; i--){
        //Irregular clocking LFSR and genearting key stream

        if ((LFSR1[8] + LFSR2[10] + LFSR3[10]) > 1 ){
            majority = 1;
        }
        else
        {
            majority = 0;
        }

        //LFSR1
        int newbit;
        if (LFSR1[8] == majority){
            newbit = LFSR1[18]^LFSR1[17]^LFSR1[16]^LFSR1[13];
            shiftLeft(LFSR1,19);
            LFSR1[0] = newbit;
        }

        //LFSR2
        if (LFSR2[10] == majority){
            newbit = LFSR2[21]^LFSR2[20];
            shiftLeft(LFSR2,22);
            LFSR2[0] = newbit;
        }

        //LFSR3
        if (LFSR3[10] == majority){
            newbit = LFSR3[22]^LFSR3[21]^LFSR3[20]^LFSR3[7];
            shiftLeft(LFSR3,23);
            LFSR3[0] = newbit;
        }

        keyStream[i] = LFSR1[18]^LFSR2[21]^LFSR3[22];

    }
}

QString A5::encrypt(QString message){
    int encryptedMessage[114] = {0};
    int plaintext[114] = {0};

    bool fOk;
    unsigned long long messageBinary;

    messageBinary = message.toULongLong();

    for (int i = 63; i >=0; i--){

        //get the bit from k (https://stackoverflow.com/questions/2249731/how-do-i-get-bit-by-bit-data-from-an-integer-value-in-c)
        int k = i;
        unsigned long long mask =  1ULL << k;
        unsigned long long masked_n = messageBinary & mask;
        unsigned long long thebit = masked_n >> k;
        plaintext[i] = thebit;

    }


    for (int i = 113; i>=0; i--){
        encryptedMessage[i] = plaintext[i] ^ keyStream[114+i];
    }


    QString EncryptedMessageFinal;
    for (int i = 114; i >= 0; i = i - 4){
        QString binaryString = QString::number(encryptedMessage[i])+QString::number(encryptedMessage[i-1])+QString::number(encryptedMessage[i-2])+QString::number(encryptedMessage[i-3]);
        bool fOk;
        int iValue = binaryString.toInt(&fOk, 2);  //2 is the base
        binaryString = QString::number(iValue, 16);  //The new base is 16
        EncryptedMessageFinal += binaryString;
    }
    //get the last two bits
    QString binaryString = QString::number(encryptedMessage[1])+QString::number(encryptedMessage[0])+"00";
    int iValue = binaryString.toInt(&fOk, 2);  //2 is the base
    binaryString = QString::number(iValue, 16);  //The new base is 16
    EncryptedMessageFinal += binaryString;

    return EncryptedMessageFinal;
}

QString A5::decrypt(QString message){
    int decryptedMessage[114] = {0};
    int ciphertext[114] = {0};

    bool fOk;
    unsigned long long messageBinary;

    messageBinary = message.toULongLong();

    for (int i = 63; i >=0; i--){

        //get the bit from k (https://stackoverflow.com/questions/2249731/how-do-i-get-bit-by-bit-data-from-an-integer-value-in-c)
        int k = i;
        unsigned long long mask =  1ULL << k;
        unsigned long long masked_n = messageBinary & mask;
        unsigned long long thebit = masked_n >> k;
        ciphertext[i] = thebit;

    }


    for (int i = 113; i>=0; i--){
        decryptedMessage[i] = ciphertext[i] ^ keyStream[i];
    }


    QString DecryptedMessageFinal;
    for (int i = 114; i >= 0; i = i - 4){
        QString binaryString = QString::number(decryptedMessage[i])+QString::number(decryptedMessage[i-1])+QString::number(decryptedMessage[i-2])+QString::number(decryptedMessage[i-3]);
        bool fOk;
        int iValue = binaryString.toInt(&fOk, 2);  //2 is the base
        binaryString = QString::number(iValue, 16);  //The new base is 16
        DecryptedMessageFinal += binaryString;
    }
    //get the last two bits
    QString binaryString = QString::number(decryptedMessage[1])+QString::number(decryptedMessage[0])+"00";
    int iValue = binaryString.toInt(&fOk, 2);  //2 is the base
    binaryString = QString::number(iValue, 16);  //The new base is 16
    DecryptedMessageFinal += binaryString;

    return DecryptedMessageFinal;
}

void A5::printRegisters()
{
        //LSB is indexed at 0

        QDebug deb = qDebug().nospace();        

        deb << "\n";

        for (int i = 18; i >= 0; i--){
        deb << LFSR1[i];
        }
        deb << "\n";
        for (int i = 21; i >= 0; i--){
        deb << LFSR2[i];
        }
        deb << "\n";
        for (int i = 22; i >= 0; i--){
        deb << LFSR3[i];
        }
        deb << "\n";

        for (int i = 227; i >= 0; i--){
            deb << keyStream[i];
            if (i == 114){
                deb << "\n";
            }

        }

}

