#include "Ui/Window.hpp"
#include <thread>
#include <iostream>

int main()
{
    try
    {
        int width = 1920;
        int height = 1080;

        Window window{width, height};
        window.runWindow();
    }
    catch(std::string x)
    {
        std::cout<<x<<std::endl;
    }
    std::cerr<<"finished"<<std::endl;

}