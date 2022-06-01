#include "mainwindow.h"
//#include <QtCore/QCoreApplication> // for console application (no ui)
#include <iostream>
#include <conio.h>
#include <QApplication> //for " QApplication a(argc, argv); "

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    //QCoreApplication a(argc, argv);


//    std::cout << "I AM ALIVE\n" << std::endl;

//
//    std::array<int8_t, 32> key2 = {1,2,3,4,6,6,7,8,9,13};

//
//    Messenger messenger2 ( "Oksana", key2, 15002 );

//    boost::asio::ip::tcp::endpoint endpoint1( boost::asio::ip::make_address("127.0.0.1"), 15001 );
//    boost::asio::ip::tcp::endpoint endpoint2( boost::asio::ip::make_address("127.0.0.1"), 15002 );

//    messenger1.sendMessage( endpoint2, "Hello" );
//    messenger2.sendMessage( endpoint1, "one more message" );
//    messenger1.sendMessage( endpoint2, "Thanks!" );

//    //sleep(5);



    return a.exec();
}
