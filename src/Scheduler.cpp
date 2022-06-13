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

bool Scheduler::swap()
{
    int bestCost = solutionCost;
    bool swapped = false;
    int swap1 = -1;
    int swap2 = -1;
    int bestTime1 = 0;
    int bestTime2 = 0;

    for(int i = 0; i < data.getNumOfJobs() - 1; j++)
    {
        for(int j = i+1; j < data.getNumOfJobs(); j++)
        {
            if (solution[i] == solution[j])
                continue;

            int newTime1 = data.getProcessingTime(solution[i], j) - data.getProcessingTime(solution[i], i);
            int newTime2 = data.getProcessingTime(solution[j], i) - data.getProcessingTime(solution[j], i);
            if (newTime1 <= remainingCapacity(solution[i]) && newTime2 <= remainingCapacity(solution[j]))
            {
                int currentCost = solutionCost;
                currentCost -= data.getProcessingCost(solution[i], i);
                currentCost -= data.getProcessingCost(solution[j], j);
                currentCost += data.getProcessingCost(solution[i], j);
                currentCost += data.getProcessingCost(solution[j], i);
                if(currentCost < bestCost)
                {
                    bestTime1 = newTime1;
                    bestTime2 = newTime2;

                    swapped = true;
                    bestCost = currentCost;

                    swap1 = i;
                    swap2 = j;
                }
            }
        }
    }
    
    if (swapped)
    {
        int auxVar = solution[swap1];
        solution[swap1] = solution[swap2];
        solution[swap2] = auxVar;
        solutionCost = bestCost;
        remainingCapacity[swap1] += newTime1;
        remainingCapacity[swap2] += newTime2;
    }

    return swapped
}

void Scheduler::vnd()
{
    int k = 0;
    bool swapped = false;
    bool inserted = false;
    while(k < 2)
    {
        swapped = false;
        inserted = false;

        if (k == 0)
            swapped = Swap();
        else if(k == 1)
            inserted = Insertion();

        if(swapped || inserted)
            k = 0;
        else
            k++;

    }

}