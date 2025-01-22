#include <iostream>
#include "FuncA.h"

int CreateHTTPserver();

int main() {
    std::cout << "Starting HTTP Server..." << std::endl;

    int result = CreateHTTPserver();

    if (result == 0) {
        std::cout << "Server terminated successfully." << std::endl;
    }
    else {
        std::cerr << "Server encountered an error!" << std::endl;
    }

    return result;
}

