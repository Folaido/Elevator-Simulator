//
// Created by FF on 2020-10-07.
//

#ifndef ELEVATOR_ELEVATOR_H
#define ELEVATOR_ELEVATOR_H

#include <_stdio.h>
#include "priorityqueue.h"

typedef struct Elevator{
    FILE *fp;     // output file pointer
    PriorityQueue *first;
    PriorityQueue *second;
    PriorityQueue *third;
    int direction;      // 0: up, 1: down
    int current_floor;
    int timestamp;
    int status;    // 0: stop, 1: move, 2: rest
    int counter;     // internal use only
    int wait_time;
    int turn_around_time;
    int total_passengers;
} Elevator;

extern void handle_call(Elevator *elevator, Request *request);
extern void tick(Elevator *elevator);

#endif //ELEVATOR_ELEVATOR_H
