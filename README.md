# Scheduling with multiple servers

Final project for the Design and Analysis of Algorithms class in the Computer Engineering undergraduate course at the Universidade Federal da Paraíba (UFPB).

It implements the metaheuristic greedy randomized adaptive search procedure with variable neighborhood descent (GRASP-VND) for solving a scheduling problem with multiple servers.
The aim is to schedule jobs in cloud servers or locally while minimizing the total cost. Each job has an associated cost and processing time in each server.
Each server has a limited time capacity.

## Input file format

\<number of jobs\>\
\<number of servers\>\
\<fixed cost for running a job locally\>

\<array of cloud servers capacities\>

\<array of processing times for the 1st job in each cloud server\>\
...\
\<array of processing times for the nth job in each cloud server\>

\<array of costs for the 1st job in each cloud server\>\
...\
\<array of costs for the nth job in each cloud server\>
