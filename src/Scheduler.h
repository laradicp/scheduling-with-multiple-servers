#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <iostream>
#include <vector>
#include <algorithm>
#include "Data.h"

class Scheduler
{
    private:
        Data data;
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
        
    public:
        Scheduler(std::string filePath, double r = 0.3);
        void initialSolution();
        void printSolution();
        void vnd();
        bool swape();
        bool insertion();
};

#endif
