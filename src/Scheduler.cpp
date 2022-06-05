#include "Scheduler.h"

Scheduler::Scheduler(std::string filePath)
{
    // initialize data
    data = Data(filePath);

    solutionCost = 0;

    for(int s = 0; s < data.getNumOfServers(); s++)
    {
        remainingCapacity.push_back(data.getServerCapacity(s));

        std::vector<int> server;
        solution.push_back(server);
    }
}

void Scheduler::initialSolution()
{
    int startJob = 0; // change to rand
    for(int j = startJob; j < data.getNumOfJobs(); j++)
    {
        int s = selectBestServer(j);
        if(s == -1)
        {
            solutionCost += data.getLocalProcessingCost();
        }
        else
        {
            solution[s].push_back(j);
            solutionCost += data.getProcessingCost(s, j);
            remainingCapacity[s] -= data.getProcessingTime(s, j);
        }
    }
    for(int j = 0; j < startJob; j++)
    {
        int s = selectBestServer(j);
        if(s == -1)
        {
            solutionCost += data.getLocalProcessingCost();
        }
        else
        {
            solution[s].push_back(j);
            solutionCost += data.getProcessingCost(s, j);
            remainingCapacity[s] -= data.getProcessingTime(s, j);
        }
    }

    return;
}

int Scheduler::selectBestServer(int j)
{
    int bestServer = -1;
    double bestRelativeCost = __FLT_MAX__;
    for(int s = 0; s < data.getNumOfServers(); s++)
    {
        if(data.getProcessingTime(s, j) <= remainingCapacity[s])
        {
            double relativeCost = data.getProcessingTime(s, j)*data.getProcessingCost(s, j)/
                remainingCapacity[s];
            if(relativeCost < bestRelativeCost)
            {
                bestRelativeCost = relativeCost;
                bestServer = s;
            }
        }
    }

    return bestServer;
}

void Scheduler::printSolution()
{
    for(int s = 0; s < data.getNumOfServers(); s++)
    {
        std::cout << "Server " << s << ":" << std::endl;
        for(int i = 0; i < solution[s].size(); i++)
        {
            std::cout << "\t" << solution[s][i] << std::endl;
        }
    }

    std::cout << "Total cost:\t" << solutionCost << std::endl;

    return;
}
