#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <iostream>
#include <vector>
#include "Data.h"

class Scheduler
{
    private:
        Data data;
        int solutionCost;
        std::vector<int> remainingCapacity;
        std::vector<std::vector<int>> solution;

        int selectBestServer(int j);
        
    public:
        Scheduler(std::string filePath);
        void initialSolution();
        void printSolution();
};

#endif
