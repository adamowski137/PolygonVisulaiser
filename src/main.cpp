#include "Ui/Window.hpp"
#include <thread>
#include <iostream>

int main()
{
    try
    {
        Window window{1000, 1000};
        window.runWindow();
    }
    catch(std::string x)
    {
        std::cout<<x<<std::endl;
    }
    std::cerr<<"finished"<<std::endl;

}