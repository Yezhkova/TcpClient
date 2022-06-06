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

    std::string my_username = ui->myUsernameEdit->text().toStdString();
    std::string address = ui->addressEdit->text().toStdString();
    std::string port = ui->portEdit->text().toStdString();
    m_client.connect(address, port, boost::posix_time::pos_infin);//boost::posix_time::seconds(10));
    m_client.sendCommandMessage("init", my_username, boost::posix_time::pos_infin);//boost::posix_time::seconds(10));
    LOG(my_username)
    //in a separate thread
    m_readThread = std::thread( [this] {
        try
        {
            for(;;)
            {
                std::string command = m_client.readLine(boost::posix_time::pos_infin);//    boost::posix_time::seconds(10));

                if (command == "list")
                {
                    std::string userList = m_client.readLine(boost::posix_time::pos_infin);//boost::posix_time::seconds(10));
                    acceptUserList(userList);

                }
                else if (command == "msg")
                {
                    // read receiver name;
                    std::string sender = m_client.readLine(boost::posix_time::pos_infin);//boost::posix_time::seconds(10));
                    std::string textOfMessage = m_client.readLine(boost::posix_time::pos_infin);//boost::posix_time::seconds(10));
                    std::string tmp = sender + " : " + textOfMessage + "<br>";
                    QString temp = ui->feed->text();
                    temp.append(QString::fromStdString(tmp));
                    ui->feed->setText( temp );
                    ui->feed->setAlignment(Qt::AlignTop | Qt::AlignLeft);
                }

            }
        }
        catch ( std::runtime_error &e) // catch everything
        {
            LOG("Cannot read line: " << e.what())
            std::terminate();
        }

    } );
}

void MainWindow::acceptUserList(std::string& userList)
{
    ui->sessionsList->clear();
    char* str = &userList[0];
    char *token = strtok(str, ";");
    while (token != NULL)
    {
        ui->sessionsList->addItem(token);
        token = strtok(NULL, ";");
    }
}

void MainWindow::on_sendButton_clicked()
{
    std::string my_message = ui->typeMessage->toPlainText().toStdString();
    if(!my_message.empty())
    {
        m_client.sendCommandMessage("msg", my_message, boost::posix_time::pos_infin);
    }
    ui->typeMessage->clear();
}


void MainWindow::on_disconnectButton_clicked()
{
    this->close();
    m_client.sendCommandMessage("disconnect", "", boost::posix_time::pos_infin);
}

