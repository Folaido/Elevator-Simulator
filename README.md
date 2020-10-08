# Elevator Simulator

## Description: 
A simulator for a single elevator system implemented by the elevator scheduling algorithm in C.

## Data Structure
PriorityQueue I: Stores all calls in the same direction, and the elevator can answer them in current round.\
PriorityQueue II: Stores all calls in the opposite direction.\
PriorityQueue III: Stores all calls in the same direction, but the elevator cannot answer them in this round.

When elevator is moving up, PQ I and III are min-heap by the floor number and PQ is max-heap.\
When elevator is moving down, PQ I amd III should be max-heap and PQ II should be min-heap.

## How to run: 
> ./elevator [input file name]

## Input File:
Every call in the file should format in this schema:\
[Start_time (s)] [# of passengers] [Start_floor (s)] [End_floor (s)]

Each call should be seperated by a line break.\
Example of an valid input file:\
1 0 4 2\
2 0 1 4\
3 1 3 3\
4 2 2 2\
5 2 2 1

Constraints:
- 1 <= floors <= 5
- No duplicate start time is allowed.
- All jobs should be sorted by arrival time in increasing order (from top to bottom).
- The file should Not contain any other irrelevant characters (including spaces and line breaks).

## Output
All results will be written into a file named [elevator.log].\
The output file name is fixed and the new result will cover the existing one.

The output file will contain:
- Logs sorted in increasing order by timestamp.
- Average wait time for all passengers.
- Average turnaround time for all passengers.

*The logs are not merged when the time is time same.

An sample output for a FIFO scheduling corresonding to the sample input:\
Time 0: The elevator picked up 2 passengers at floor 1.\
Time 12: The elevator picked up 2 passengers at floor 3.\
Time 22: The elevator dropped off 2 passengers at floor 5.\
Time 22: The elevator dropped off 2 passengers at floor 5.\
Time 24: The elevator picked up 4 passengers at floor 5.\
Time 34: The elevator picked up 1 passengers at floor 3.\
Time 39: The elevator picked up 3 passengers at floor 2.\
Time 44: The elevator dropped off 1 passengers at floor 1.\
Time 44: The elevator dropped off 3 passengers at floor 1.\
Time 44: The elevator dropped off 4 passengers at floor 1.\
Average wait time = 223/12 = 18.58s.\
Average turnaround time = 392/12 = 32.67s.
