#ifndef DATA_H
#define DATA_H

#include <iostream>
#include <vector>
#include <fstream>

class Data
{
    private:
        int numOfJobs;
        int numOfServers;
        int localProcessingCost;
        std::vector<int> serversCapacities;
        std::vector<std::vector<int>> processingTimes;
        std::vector<std::vector<int>> processingCosts;
        
    public:
        Data();
        Data(std::string filePath);
        int getNumOfJobs();
        int getNumOfServers();
        int getLocalProcessingCost();
        int getServerCapacity(int s);
        int getProcessingTime(int s, int j);
        int getProcessingCost(int s, int j);
};

#endif
