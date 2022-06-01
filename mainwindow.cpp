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
//    std::string my_username = ui->myUsernameEdit->text().toStdString();
//    std::string address = ui->addressEdit->text().toStdString();
//    bool ok;
//    int port = ui->portEdit->text().toInt(&ok);
//    if (!ok) {
//      std::cout << "Conversion failed. Repeat conversion" <<std::endl;
//    }
//    m_client.runContext();
//    LOG("I am running context")
//    m_client.connectSocket(address, port);
//    LOG("I have connected socket")
//    m_client.sendInitMessage(my_username);
//    LOG("I have sent init message")
//    //m_client.readNextMessage();
//    LOG("I have read next message")

    TcpBlockingClient c;
    c.connect("localhost", "15001", boost::posix_time::seconds(10));

    c.write_line("init\nuser1\n", boost::posix_time::seconds(10));

    //in a separate thread
    std::string line = c.read_line(boost::posix_time::seconds(10));
    std::cout << "I read init message:\n" << line << '\n';
    // Keep going until we get back the line that was sent.
    //      if (line == argv[3])
    //        break;


}

void MainWindow::acceptUserList(std::string userList)
{

}


