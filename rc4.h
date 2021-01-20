#ifndef RC4_H
#define RC4_H

#include <QMainWindow>
#include <string>
#include <cstring>
#include <cctype>
#include <QMessageBox>
#include <iostream>
#include <bitset>
#include <sstream>
#include <algorithm>
#include <QPointer>
#include <string>

#include "mainwindow.h"

namespace Ui {
class RC4;
}

class RC4 : public QMainWindow
{
    Q_OBJECT

public:
    explicit RC4(QWidget *parent = 0);
    ~RC4();

private slots:
    //the following functions are triggerd when a button is clicked or when an action is triggered.
    void on_pushButton_encrypt_clicked();
    void on_pushButton_decrypt_clicked();

    void on_actionReset_Fields_triggered();
    void on_actionClear_Plain_Text_triggered();
    void on_actionClear_Encrypted_Text_triggered();
    void on_actionExit_triggered();
    void on_actionGo_Back_to_Main_Menu_triggered();
    void on_actionClear_Key_triggered();
    void on_actionClear_Steps_triggered();

    void on_comboBox_currentTextChanged(const QString &arg1);

private:
    Ui::RC4 *ui;
    /**
     * @brief ewRC4 intializes the key, vector S, vector T from the key
     * @param msg of type QString
     * @param key of type QString
     * @return
     */
    QString ewRC4(QString msg1, QString key1);
    /**
     * @brief dwRC4
     * @param msg of type QString
     * @param key of type QString
     * @return ewRC4(msg, key)
     */
    QString dwRC4(QString msg, QString key);

    /**
     * @brief RC4Encryption XORs the message prepared and the key generated
     * @param msg_bytes message prepared
     * @param keys  key genereated
     * @return the output encryption or decryption
     */
    QString RC4Encryption(std::vector< std::bitset<8> > msg_bytes, std::vector< std::bitset<8> > keys);
    /**
     * @brief keysGenerator Pseudo-random generation algorithm
     * For as many iterations as are needed, the PRGA modifies the state and outputs a byte of the keystream.
     * @param msg_length
     * @param s
     * @return byte of the keystream
     */
    std::vector< std::bitset<8> > keysGenerator(int msg_length, std::vector<int> s);
    std::vector< std::bitset<8> > bytesOfMessage(std::string msg);

    /**
     * @brief CondHexToBin converts from hexadecimal to binary in case the input was in hexadecimal
     * @param hexdec1 hexadecimal number of type QString
     * @return QString binary representation of the input number in hexadecimal//binary
     */
    QString CondHexToBin(QString hexdec1);

    /**
     * @brief getHexCharacter helper function for BinToHex
     * @param str
     * @return
     */
    char getHexCharacter(std::string str);

    /**
     * @brief BinToHex converts a QString from binary to hexadecimal
     * @param rowresult1
     * @return
     */
    QString BinToHex(QString rowresult1);


};



#endif // RC4_H
