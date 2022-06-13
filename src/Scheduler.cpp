#include "Scheduler.h"

Scheduler::Scheduler(std::string filePath, double r)
{
    // initialize data
    data = Data(filePath);

    solutionCost = 0;

    randomness = r;

    for(int s = 0; s < data.getNumOfServers(); s++)
    {
        remainingCapacity.push_back(data.getServerCapacity(s));
    }

    for(int j = 0; j < data.getNumOfJobs(); j++)
    {
        solution.push_back(-1);
    }

    srand(time(NULL));
}

void Scheduler::initialSolution()
{
    int startJob = rand()%data.getNumOfJobs();
    for(int j = startJob; j < data.getNumOfJobs(); j++)
    {
        int s = selectServer(j);
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
        int s = selectServer(j);
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

bool Scheduler::compareServers(CandidateServer s1, CandidateServer s2)
{
    return s1.relativeCost <= s2.relativeCost;
}

int Scheduler::selectServer(int j)
{
    std::vector<CandidateServer> candidateServers;

    candidateServers.push_back(CandidateServer(-1, data.getLocalProcessingCost()));

    for(int s = 0; s < data.getNumOfServers(); s++)
    {
        if(data.getProcessingTime(s, j) <= remainingCapacity[s])
        {
            candidateServers.push_back(CandidateServer(s,
                data.getProcessingTime(s, j)*data.getProcessingCost(s, j)/remainingCapacity[s]));
        }
    }

    std::sort(candidateServers.begin(), candidateServers.end(), compareServers);

    if((int)randomness*candidateServers.size() > 0)
    {
        int i = rand()%(int)(randomness*candidateServers.size());
        return candidateServers[i].server;
    }

    return candidateServers[0].server;
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
