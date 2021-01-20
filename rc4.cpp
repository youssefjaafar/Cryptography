#include "rc4.h"
#include "ui_rc4ui.h"

RC4::RC4(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RC4)
{
    ui->setupUi(this);

    //coloring :D

    QString style = "QMenuBar::item:selected { background: white; } QMenuBar::item:pressed {  background: white; }";

    ui->menuBar->setStyleSheet(style);


    ui->comboBox->setStyleSheet("background-color: #097770; color: white;");
    ui->centralWidget->setStyleSheet("background-color: #e0cdbe;");
    ui->pushButton_encrypt->setStyleSheet("background-color: #a9c0a6;");
    ui->pushButton_decrypt->setStyleSheet("background-color: #a9c0a6;");

  //  ui->menuEdit->setStyleSheet("background-color: #edca82; color: black;");
  //  ui->menuFile->setStyleSheet("background-color: #edca82; color: black;");

    ui->lineEdit_rc4_key->setPlaceholderText("Enter Key here, example: 101 or 0xA32B");
    ui->plainTextEdit_plain->setPlaceholderText("Enter plaintext to encrypt, example: 101 or 0xA32B");
    ui->plainTextEdit_encrypted->setPlaceholderText("Enter ciphertext to decrypt, example: 101 or 0xA32B");

    ui->lineEdit_rc4_key->setStyleSheet("background-color: #edca82; color: black;");
    ui->plainTextEdit_plain->setStyleSheet("background-color: #edca82; color: black;");
    ui->plainTextEdit_encrypted->setStyleSheet("background-color: #edca82; color: black;");
}

RC4::~RC4()
{
    delete ui;
}

void RC4::on_pushButton_encrypt_clicked()
{
    QString plainText = ui->plainTextEdit_plain->toPlainText();
    QString encryptedText = "to be implemented";

    if(ui->comboBox->currentIndex() == 0)
    {
        QString key = ui->lineEdit_rc4_key->text();
        encryptedText = ewRC4(plainText, key);
    }

    ui->plainTextEdit_encrypted->document()->setPlainText(encryptedText);
}

void RC4::on_pushButton_decrypt_clicked()
{
    QString encryptedText = ui->plainTextEdit_encrypted->toPlainText();
    QString plainText = "to be implemented";

    if(ui->comboBox->currentIndex() == 0)
    {
        QString key = ui->lineEdit_rc4_key->text();
        plainText = dwRC4(encryptedText, key);
    }

    ui->plainTextEdit_plain->document()->setPlainText(plainText);
}

void RC4::on_actionReset_Fields_triggered()
{
    ui->plainTextEdit_plain->clear();
    ui->plainTextEdit_encrypted->clear();
    ui->lineEdit_rc4_key->clear();
    ui->textEdit->clear();
}

void RC4::on_actionClear_Key_triggered()
{
    ui->lineEdit_rc4_key->clear();
}


void RC4::on_actionClear_Plain_Text_triggered()
{
    ui->plainTextEdit_plain->clear();
}

void RC4::on_actionClear_Encrypted_Text_triggered()
{
    ui->plainTextEdit_encrypted->clear();
}

void RC4::on_actionClear_Steps_triggered()
{
    ui->textEdit->clear();
}


void RC4::on_actionExit_triggered()
{
    exit(0);
}

void RC4::on_actionGo_Back_to_Main_Menu_triggered()
{
    this->close();
    QPointer<mainWindow> window = new mainWindow();
    window->show();
}

void RC4::on_comboBox_currentTextChanged(const QString &arg1)
{

    if(arg1 == "RC4")
    {
        ui->stackedWidget->setCurrentIndex(4);
    }

}


// RC4 Methods
//init
QString RC4::ewRC4(QString msg1, QString key1)
{
    QString msg = CondHexToBin(msg1);
    QString key = CondHexToBin(key1);
    //QString key = CondHexToBin(key1);
    std::vector<int> k;
    if (key.length() == 0){
        return " ";
    }
    // initialize key
    std::string stdCppKey = key.toStdString();
    for(int i=0; i<stdCppKey.length(); ++i)
        k.push_back(int(stdCppKey[i] - '0'));

    std::vector<int> s(256);
    std::vector<int> t(256);

    // initialize vector S to [0:255]
    for(int i=0; i<256; ++i)
        s[i] = i;

    // initialize vector T from key K
    for(int i=0; i<256; ++i)
        t[i] = k[i%k.size()];

    // initial Permutation Of S
    int j = 0;
    for(int i=0; i<256; ++i)
    {
        j = (j + s[i] + t[i])%256;
        std::swap(s[i], s[j]);
    }

    // prepare msg
    std::vector< std::bitset<8> > msg_bytes = bytesOfMessage(msg.toStdString());


    // generate a key for each byte of input
    // e.g input: 5 bytes, generates output: 5 bytes
    std::vector< std::bitset<8> > keys = keysGenerator((int)msg_bytes.size(), s);

    return RC4Encryption(msg_bytes, keys);

}

QString RC4::dwRC4(QString msg, QString key)
{
    return ewRC4(msg, key);
}

QString RC4::RC4Encryption(std::vector< std::bitset<8> > msg_bytes, std::vector< std::bitset<8> > keys)
{
    std::string result = "";
    for (int i = 0; i < msg_bytes.size(); ++i)
    {
        std::bitset<8> temp = std::bitset<8>(msg_bytes[i] ^ keys[i]);
        result += temp.to_string();
    }

    // Debugging output
    std::cout << "> Debugging Statements" << std::endl;

    std::string sss = " "; // used to display the steps of the algorithm

    std::cout << "I/P: " ;
    for(int t=0; t<msg_bytes.size(); ++t){
        std::cout << msg_bytes[t];
        sss += msg_bytes[t].to_string();//
    }

    std::cout << std::endl;

    QString steps = "message prepared:";//
    steps = steps.append(QString::fromStdString(sss));//
    sss = "";//

    std::cout << "Key: ";
    for(int t=0; t<keys.size(); ++t){
        std::cout << keys[t].to_string();
        sss += keys[t].to_string();//
    }
    std::cout << std::endl;
    std::cout << "O/P: " << result << std::endl;

    steps = steps.append("\n");//
    steps = steps.append("key generated: ");//
    steps = steps.append(QString::fromStdString(sss));//
    steps = steps.append("\n");//
    steps = steps.append(QString::fromStdString("the above will be XORed to get the output"));

     ui->textEdit->setText(steps);//

    if(ui->checkBox->isChecked()){
        return BinToHex(QString::fromStdString(result));
    }

    ///////////////////////////////
    /*
    std::string sss = " ";
    for (int i = 0; i < msg_bytes.size(); ++i)
    {
        std::bitset<8> temp = std::bitset<8>(msg_bytes[i]);
        sss += temp.to_string();
    }

    QString steps = "message prepared:";
    steps = steps.append(QString::fromStdString(sss));
    sss = "";
    for (int i = 0; i < keys.size(); ++i)
    {

        sss += keys[i].to_string();
    }

    steps = steps.append("\n");
    steps = steps.append("key generated: ");
    steps = steps.append(QString::fromStdString(sss));

     ui->textEdit->setText(steps);
     */
    ///////////////////////

    return QString::fromStdString(result);
}

std::vector< std::bitset<8> > RC4::keysGenerator(int msg_length, std::vector<int> s)
{
    std::vector< std::bitset<8> > keys(msg_length);

    int i=0, j=0, k=0;
    for (int lc = 0; lc < msg_length; ++lc)
    {
        i = (i+1)%256;
        j = (j+s[i])%256;
        std::swap(s[i], s[j]);
        k = s[(s[i]+s[j])%256];
        keys[lc] = std::bitset<8>(k);
    }

    return keys;
}

std::vector< std::bitset<8> > RC4::bytesOfMessage(std::string msg)
{
    std::vector< std::bitset<8> > msg_bytes;

    for(int i=0; i<(msg.length()/8); ++i)
    {
        std::string msg_byte = "";
        for(int j=0; j<8; ++j)
        {
            int idx = j + i*8;
            msg_byte += msg[idx];
        }
        msg_bytes.push_back(std::bitset<8>(msg_byte));
    }

    return msg_bytes;
}


QString RC4::CondHexToBin(QString hexdec1)
{
    std::string hexdec2 = hexdec1.toUtf8().constData();
    //checking is the input is in hexadecimal
    for (int i = 0; i < hexdec2.size(); ++i){
        if (hexdec2[i] == ' ')
            return " ";
    }

    if (hexdec2[0] == '0' && (hexdec2[1] == 'x' || hexdec2[1] == 'X')) {
        std::string hexdec = hexdec2.substr(2, hexdec1.length());
        QString temp = "";
        long int i = 0;

        while (hexdec[i]) {

            switch (hexdec[i]) {
            case '0':
                temp.append("0000");
                break;
            case '1':
                temp.append("0001");
                break;
            case '2':
                temp.append("0010");
                break;
            case '3':
                temp.append("0011");
                break;
            case '4':
                temp.append("0011");
                break;
            case '5':
                temp.append("0101");
                break;
            case '6':
                temp.append("0110");
                break;
            case '7':
                temp.append("0111");
                break;
            case '8':
                temp.append("1000");
                break;
            case '9':
                temp.append("1001");
                break;
            case 'A':
            case 'a':
                temp.append("0011");
                break;
            case 'B':
            case 'b':
                temp.append("1011");
                break;
            case 'C':
            case 'c':
                temp.append("1100");
                break;
            case 'D':
            case 'd':
                temp.append("1101");
                break;
            case 'E':
            case 'e':
                temp.append("1110");
                break;
            case 'F':
            case 'f':
                temp.append("1111");
                break;
            default:
                NULL;
                //std::cout << "\nInvalid hexadecimal digit ";
            }
            i++;
        }
        return temp;
    }
    //if the input is not in hexadecimal, simply return the input, which is supposed to be in binary
    else
    {
        return hexdec1;
    }
}

char RC4::getHexCharacter(std::string str)
{
    if (str.compare("1111") == 0) return 'F';
    else if (str.compare("1110") == 0) return 'E';
    else if (str.compare("1101") == 0) return 'D';
    else if (str.compare("1100") == 0) return 'C';
    else if (str.compare("1011") == 0) return 'B';
    else if (str.compare("1010") == 0) return 'A';
    else if (str.compare("1001") == 0) return '9';
    else if (str.compare("1000") == 0) return '8';
    else if (str.compare("0111") == 0) return '7';
    else if (str.compare("0110") == 0) return '6';
    else if (str.compare("0101") == 0) return '5';
    else if (str.compare("0100") == 0) return '4';
    else if (str.compare("0011") == 0) return '3';
    else if (str.compare("0010") == 0) return '2';
    else if (str.compare("0001") == 0) return '1';
    else if (str.compare("0000") == 0) return '0';
    else if (str.compare("111") == 0) return '7';
    else if (str.compare("110") == 0) return '6';
    else if (str.compare("101") == 0) return '5';
    else if (str.compare("100") == 0) return '4';
    else if (str.compare("011") == 0) return '3';
    else if (str.compare("010") == 0) return '2';
    else if (str.compare("001") == 0) return '1';
    else if (str.compare("000") == 0) return '0';
    else if (str.compare("11") == 0) return '3';
    else if (str.compare("10") == 0) return '2';
    else if (str.compare("01") == 0) return '1';
    else if (str.compare("00") == 0) return '0';
    else if (str.compare("1") == 0) return '1';
    else if (str.compare("0") == 0) return '0';
}

QString RC4::BinToHex(QString rowresult1)
{
    std::string rowresult = rowresult1.toUtf8().constData();
    QString endresult = "0x";
    for (int i = 0; i < rowresult.length(); i = i + 4)
    {
        endresult += getHexCharacter(rowresult.substr(i, 4));
    }
    return endresult;
}

