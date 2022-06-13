#include <iostream>
#include <vector>
#include "Scheduler.h"

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        std::cout << "Usage: ./scheduler instance" << std::endl;
        return 0;
    }

    Scheduler scheduler = Scheduler(argv[1]);

    scheduler.initialSolution();
    //scheduler.Vnd();
    scheduler.printSolution();
    
    return 0;
}