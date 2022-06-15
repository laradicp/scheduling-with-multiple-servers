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
    // reset solution
    solutionCost = 0;
    for(int s = 0; s < data.getNumOfServers(); s++)
    {
        remainingCapacity[s] = data.getServerCapacity(s);
    }

    int startJob = rand()%data.getNumOfJobs();
    for(int j = startJob; j < data.getNumOfJobs(); j++)
    {
        int s = selectServer(j);
        if(s == -1)
        {
            solution[j] = -1;
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
            solution[j] = -1;
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

bool Scheduler::swap()
{
    int bestCost = solutionCost;
    bool swapped = false;
    int swap1 = -1;
    int swap2 = -1;
    int bestTime1 = 0;
    int bestTime2 = 0;

    for(int i = 0; i < data.getNumOfJobs() - 1; i++)
    {
        for(int j = i + 1; j < data.getNumOfJobs(); j++)
        {
            if(solution[i] == solution[j])
                continue;

            bool timeConstraint = true;
            int newTime1 = 0;
            int newTime2 = 0;
            int capacity1 = 0;
            int capacity2 = 0;

            if(solution[i] >= 0)
            {
                newTime1 = data.getProcessingTime(solution[i], j) - data.getProcessingTime(solution[i], i);
                capacity1 = remainingCapacity[solution[i]];
            }
            if(solution[j] >= 0)
            {
                newTime2 = data.getProcessingTime(solution[j], i) - data.getProcessingTime(solution[j], j);
                capacity2 = remainingCapacity[solution[j]];
            }

            if(newTime1 <= capacity1 && newTime2 <= capacity2)
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

    if(swapped)
    {
        solutionCost = bestCost;

        if(solution[swap1] >= 0)
            remainingCapacity[solution[swap1]] -= bestTime1;
        if(solution[swap2] >= 0)
            remainingCapacity[solution[swap2]] -= bestTime2;

        int auxVar = solution[swap1];
        solution[swap1] = solution[swap2];
        solution[swap2] = auxVar;
    }

    return swapped;
}

bool Scheduler::insertion()
{
    int bestCost = solutionCost;
    int jobToInsert = -1;
    int serverToInsert = -2;
    int prevServer = -2;
    bool inserted = false;

    for(int j = 0; j < data.getNumOfJobs(); j++)
    {
        for(int s = -1; s < data.getNumOfServers(); s++)
        {
            if(solution[j] == s)
                continue;
            
            if((s == -1) || (data.getProcessingTime(s, j) <= remainingCapacity[s]))
            {
                int currentCost = solutionCost;
                currentCost -= data.getProcessingCost(solution[j], j);
                currentCost += data.getProcessingCost(s, j);

                if(currentCost < bestCost)
                {
                    bestCost = currentCost;
                    prevServer = solution[j];
                    jobToInsert = j;
                    serverToInsert = s;
                    inserted = true;
                }

            }
            
        }
    }
    if(inserted)
    {
        if(prevServer != -1)
            remainingCapacity[prevServer] += data.getProcessingTime(prevServer, jobToInsert);
        if(serverToInsert != -1)
            remainingCapacity[serverToInsert] -= data.getProcessingTime(serverToInsert, jobToInsert);
        solutionCost = bestCost;
        solution[jobToInsert] = serverToInsert;
    }

    return inserted;
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

        if(k == 0)
            swapped = swap();
        else if(k == 1)
            inserted = insertion();

        if(swapped || inserted)
            k = 0;
        else
            k++;

    }

}

void Scheduler::printSolution()
{
    // jobsInServers indicates which jobs are in each server
    // s = n corresponds to server n - 1
    std::vector<std::vector<int> > jobsInServers(data.getNumOfServers() + 1,
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

void Scheduler::printSolution(std::vector<int> solution)
{
    // jobsInServers indicates which jobs are in each server
    // s = n corresponds to server n - 1
    int solutionCost = 0;
    std::vector<std::vector<int> > jobsInServers(data.getNumOfServers() + 1,
        std::vector<int>(0, 0));
    for(int j = 0; j < data.getNumOfJobs(); j++)
    {
        jobsInServers[solution[j] + 1].push_back(j);

        if(solution[j] == -1)
        {
            solutionCost += data.getLocalProcessingCost();
        }
        else
        {
            solutionCost += data.getProcessingCost(solution[j], j);
        }
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

std::vector<int> Scheduler::getSolution()
{
    return solution;
}

int Scheduler::getSolutionCost()
{
    return solutionCost;
}
