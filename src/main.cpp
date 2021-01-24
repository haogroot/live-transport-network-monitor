#include <iostream>
#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

int main()
{
    boost::system::error_code ec {};
    if (ec) {
        std::cerr << "Error: " << ec.message() << std::endl;
        return -1;
    }
    std::cout << "OK" << std::endl;
    return 0;
}
