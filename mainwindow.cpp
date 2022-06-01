#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "log.h"
#include "tcpblockingclient.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_client(*this)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_connectButton_clicked()
{

//    m_client.runContext();
//    LOG("I am running context")
//    m_client.connectSocket(address, port);
//    LOG("I have connected socket")
//    m_client.sendInitMessage(my_username);
//    LOG("I have sent init message")
//    //m_client.readNextMessage();
//    LOG("I have read next message")

    std::string my_username = ui->myUsernameEdit->text().toStdString();
    std::string address = ui->addressEdit->text().toStdString();
    std::string port = ui->portEdit->text().toStdString();
    TcpBlockingClient c;
    c.connect(address, port, boost::posix_time::pos_infin);//boost::posix_time::seconds(10));

    c.sendInitMessage(my_username, boost::posix_time::pos_infin);//boost::posix_time::seconds(10));

    //in a separate thread
    std::string line = c.readLine(boost::posix_time::pos_infin);//boost::posix_time::seconds(10));
    //LOG("Line : " << line)
    acceptUserList(line);
    // Keep going until we get back the line that was sent.
    //      if (line == argv[3])
    //        break;


}

void MainWindow::acceptUserList(std::string& userList)
{
        char* str = &userList[0];

        // Returns first token
        char *token = strtok(str, ";");

        // Keep printing tokens while one of the
        // delimiters present in str[].
        while (token != NULL)
        {
            ui->sessionsList->addItem(token);
            printf("%s\n", token);
            token = strtok(NULL, ";");
        }
}


