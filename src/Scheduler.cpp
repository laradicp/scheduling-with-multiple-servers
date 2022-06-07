#include "Scheduler.h"

Scheduler::Scheduler(std::string filePath)
{
    // initialize data
    data = Data(filePath);

    solutionCost = 0;

    for(int s = 0; s < data.getNumOfServers(); s++)
    {
        remainingCapacity.push_back(data.getServerCapacity(s));
    }

    for(int j = 0; j < data.getNumOfJobs(); j++)
    {
        solution.push_back(-1);
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
            solution[j] = s;
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
            solution[j] = s;
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
    // jobsInServers indicates which jobs are in each server
    // s = n corresponds to server n - 1
    std::vector<std::vector<int>> jobsInServers(data.getNumOfServers() + 1,
        std::vector<int>(0, 0));
    for(int j = 0; j < data.getNumOfJobs(); j++)
    {
        jobsInServers[solution[j] + 1].push_back(j);
    }

    for(int s = 0; s < data.getNumOfServers() + 1; s++)
    {
        std::cout << "Server " << s - 1 << ":" << std::endl;
        for(int i = 0; i < jobsInServers[s].size(); i++)
        {
            std::cout << "\t" << jobsInServers[s][i] << std::endl;
        }
    }

    std::cout << "Total cost:\t" << solutionCost << std::endl;

    return;
}
