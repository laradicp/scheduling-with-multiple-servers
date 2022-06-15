#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <iostream>
#include <vector>
#include <algorithm>
#include "Data.h"

class Scheduler
{
    private:
        int solutionCost;
        double randomness;
        std::vector<int> remainingCapacity;
        std::vector<int> solution;

        struct CandidateServer
        {
            int server;
            int relativeCost;
            CandidateServer(int s, int rc)
            {
                server = s;
                relativeCost = rc;
            }
        };

        int selectServer(int j);
        static bool compareServers(CandidateServer s1, CandidateServer s2);
        bool swap();
        bool insertion();
        
    public:
        Data data;

        Scheduler(std::string filePath);
        void initialSolution();
        void vnd();

        void printSolution();
        void printSolution(std::vector<int> solution);
        std::vector<int> getSolution();
        int getSolutionCost();
};

#endif
