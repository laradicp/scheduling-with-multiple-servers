#include <iostream>
#include <vector>
#include <chrono>
#include "Scheduler.h"

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        std::cout << "Usage: ./scheduler instance" << std::endl;
        return 0;
    }

    Scheduler scheduler = Scheduler(argv[1], 0.25);
    int bestCost = scheduler.data.getLocalProcessingCost()*scheduler.data.getNumOfJobs();
    std::vector<int> bestSolution;
    for(int j = 0; j < scheduler.data.getNumOfJobs(); j++)
    {
        bestSolution.push_back(-1);
    }

    auto start = std::chrono::system_clock::now();

    for(int i = 0; i < 100; i++)
    {
        scheduler.initialSolution();
        scheduler.vnd();

        if(scheduler.getSolutionCost() < bestCost)
        {
            bestCost = scheduler.getSolutionCost();
            bestSolution = scheduler.getSolution();
        }
    }

    std::chrono::duration<double> time = std::chrono::system_clock::now() - start;

    scheduler.printSolution(bestSolution);
    std::cout << "Time:\t\t" << time.count() << std::endl;
    
    return 0;
}
