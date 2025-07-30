//TO DO: Write a string parser here
#include<iostream>
#include <fstream>
#include <string>

void configParser()
{
    std::cout<<"---------------------------------"<<std::endl;
    std::cout<<"This log is from configParser.cpp"<<std::endl;

    std::string filePath;
    std::ifstream conf("src/PetEngine.conf");
    while (std::getline(conf, filePath))
    {
        std::cout<<filePath<<std::endl;      
    }
    std::cout<<"---------------------------------"<<std::endl;
}

