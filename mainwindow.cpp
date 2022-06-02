#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "log.h"
#include "tcpblockingclient.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_client()
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
    m_client.connect(address, port, boost::posix_time::pos_infin);//boost::posix_time::seconds(10));

    m_client.sendInitMessage(my_username, boost::posix_time::pos_infin);//boost::posix_time::seconds(10));

    //in a separate thread
    m_readThread = std::thread( [this] {
        for(;;)
        {
            std::string command = m_client.readLine(boost::posix_time::pos_infin);//    boost::posix_time::seconds(10));

            if(command == "list")
            {
                std::string userList = m_client.readLine(boost::posix_time::pos_infin);//boost::posix_time::seconds(10));
                acceptUserList(userList);

            }
            else if(command == "msg")
            {
                // read receiver name;
                std::cout <<"YEAH!\n";
            }

        }

    } );
}

void MainWindow::acceptUserList(std::string& userList)
{
        char* str = &userList[0];
        char *token = strtok(str, ";");
        while (token != NULL)
        {
            QList<QListWidgetItem *> items = ui->sessionsList->findItems(token, Qt::MatchExactly);
            if(items.size() == 0)
            {
                ui->sessionsList->addItem(token);
                //printf("%s\n", token);
            }
            token = strtok(NULL, ";");

        }
}


