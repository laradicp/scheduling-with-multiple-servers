#include "Data.h"

Data::Data()
{
    numOfJobs = numOfServers = localProcessingCost = 0;
}

Data::Data(std::string filePath)
{
    // open data file
    std::ifstream dataFile;
    dataFile.open(filePath);
    if(!dataFile.is_open())
    {
        std::cout << "Problem opening file " << filePath << " for reading." << std::endl;
        exit(1);
    }

    // read data values
    dataFile >> numOfJobs;
    dataFile >> numOfServers;
    dataFile >> localProcessingCost;
    for(int s = 0; s < numOfServers; s++)
    {
        int serverCapacity;
        dataFile >> serverCapacity;
        serversCapacities.push_back(serverCapacity);
    }
    for(int s = 0; s < numOfServers; s++)
    {
        std::vector<int> sProcessingTimes;
        for(int j = 0; j < numOfJobs; j++)
        {
            int processingTime;
            dataFile >> processingTime;
            sProcessingTimes.push_back(processingTime);
        }
        processingTimes.push_back(sProcessingTimes);
    }
    for(int s = 0; s < numOfServers; s++)
    {
        std::vector<int> sProcessingCosts;
        for(int j = 0; j < numOfJobs; j++)
        {
            int processingCost;
            dataFile >> processingCost;
            sProcessingCosts.push_back(processingCost);
        }
        processingCosts.push_back(sProcessingCosts);
    }
}

int Data::getNumOfJobs()
{
    return numOfJobs;
}

int Data::getNumOfServers()
{
    return numOfServers;
}

int Data::getLocalProcessingCost()
{
    return localProcessingCost;
}

int Data::getServerCapacity(int s)
{
    return serversCapacities[s];
}

int Data::getProcessingTime(int s, int j)
{
    return processingTimes[s][j];
}

int Data::getProcessingCost(int s, int j)
{
    return processingCosts[s][j];
}
