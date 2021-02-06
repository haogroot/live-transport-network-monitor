#include <iostream>
#include <iomanip>
#include <thread>

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

using tcp = boost::asio::ip::tcp;

void Log (boost::system::error_code ec)
{
    std::cerr << "[" 
              << std::setw(6) << "0x" << 
              std::hex << std::this_thread::get_id()
             << "] " << (ec ? "Error: " : "OK")
             << (ec ? ec.message() : "")
             << std::endl;
}

void OnConnect (boost::system::error_code ec)
{
    Log (ec);
}

int main()
{
    std::cerr << "[" << std::setw(6) << "0x" << 
        std::hex << std::this_thread::get_id() << 
        "] main" << std::endl;
    // Always start with an I/O context object.
    boost::asio::io_context ioc {};
    
    tcp::socket socket {boost::asio::make_strand(ioc)};

    boost::system::error_code ec {};

    tcp::resolver resolver {ioc};

    //auto address {boost::asio::ip::address::from_string("1.1.1.1")};
    //tcp::endpoint endpoint {address, 80};
    auto endpoint {resolver.resolve("google.com", "80", ec)};

    if (ec) {
        Log (ec);
        return -1;
    }

    size_t nThreads {4};


    for (size_t idx = 0; idx < nThreads; ++idx)
    {
        socket.async_connect (*endpoint, OnConnect);
    }

    std::vector<std::thread> threads {};
    threads.reserve (nThreads);
    for (size_t i=0; i<nThreads; i++)
    {
        threads.emplace_back ([&ioc]() {
                ioc.run();
                });
    }

    for (size_t i=0; i<nThreads; i++)
    {
        threads[i].join();
    }
    return 0;
}
