#include "Ui/Window.hpp"
#include <thread>
#include <iostream>

int main()
{
    try{
        Window window{400, 400};
        window.runWindow();
        
    }
    catch(std::string x)
    {
        std::cout<<x<<std::endl;
    }
    std::cerr<<"finished"<<std::endl;

}