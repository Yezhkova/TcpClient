#include "tcpclient.h"
#include "log.h"
#include "mainwindow.h"

TcpClient::TcpClient(MainWindow& mainWin) :
    m_socket(m_io_context),
    m_mainWindow(mainWin)
{

}

TcpClient::~TcpClient()
{
    LOG("TcpClient killed")
}

std::string TcpClient::connectSocket(std::string addr, int port)
{

    boost::asio::ip::address address = boost::asio::ip::make_address(addr);
    boost::asio::ip::tcp::endpoint endpoint(address, port);

    boost::system::error_code error;
    m_socket.connect(endpoint, error);
    if(!error)
    {
        std::cout << "The connection has been established!\n";

    }
    else
    {
        std::cerr << "Something went wrong :( ";
        std::cerr << error.message();
        return error.message();
    }
    return "";
}

std::string TcpClient::sendInitMessage(std::string username)
{
    std::string message = "init\n" + username + "\n";
    //std::cout << message << std::endl; // DEBUG
    //std::cout<<std::boolalpha<<m_socket.is_open()<<std::endl; // DEBUG
    boost::asio::async_write( m_socket, boost::asio::buffer( message ),
                              [this]( boost::system::error_code error, std::size_t bytes_transferred ) // passed a & to access s
    {
        if(error)
        {
            LOG( "Async_write error " << error.message( ));
        }
        else
        {
            // std::cout << "message "<< message << ", sent "<<bytes_transferred << " bytes\n"; // DEBUG
            std::cout << "TcpClient: bytes transferred: "<< bytes_transferred << " bytes\n"; // DEBUG
            readNextMessage();
        }
    } );

    return "";
}

void TcpClient::readNextMessage()
{
    //LOG("I am inside reading next message")
    boost::asio::async_read_until(m_socket, m_streambuf, '\n', [this]
                                  (boost::system::error_code error, std::size_t bytes_transferred)
    {
        LOG("Bytes trasferred: " << bytes_transferred)

        if(!error)
        {
            const std::string delimiter = "\n";
            std::string command{
                buffers_begin(m_streambuf.data()),
                        buffers_begin(m_streambuf.data()) + bytes_transferred
                        - delimiter.size()};
            m_streambuf.consume(bytes_transferred);
            LOG("command: " + command)

                    if(command == "list")
            {
                // read usrname;
                boost::asio::async_read_until(m_socket, m_streambuf, '\n', [this]
                                              (boost::system::error_code error, std::size_t bytes_transferred)
                {
                    const std::string delimiter = "\n";
                    std::string userList{
                        buffers_begin(m_streambuf.data()),
                                buffers_begin(m_streambuf.data()) + bytes_transferred
                                - delimiter.size()};
                    m_streambuf.consume(bytes_transferred);
                    std::cout << "List = " << userList; // DEBUG
                    m_mainWindow.acceptUserList(userList);
                });

            }
        }
        else
        {
            LOG(error.message())

        }
    });

}

void TcpClient::runContext()
{
    m_runThread = std::thread([this] {
        m_io_context.run();
        //readNextMessage();
    });
}
