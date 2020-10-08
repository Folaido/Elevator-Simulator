#include <stdio.h>
#include <stdlib.h>
#include "priorityqueue.h"
#include "elevator.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Incorrect number of arguments provided!");
        exit(1);
    }
    // Initialization for elevator
    Elevator *elevator = (Elevator*)malloc(sizeof(Elevator));
    elevator->fp = fopen("elevator.log", "w");
    elevator->first = pqueue_init(5);
    elevator->second = pqueue_init(5);
    elevator->third = pqueue_init(5);
    elevator->direction = 0;
    elevator->current_floor = 1;
    elevator->timestamp = 0;
    elevator->status = 2;
    elevator->counter = 0;
    elevator->wait_time = 0;
    elevator->turn_around_time = 0;
    elevator->total_passengers = 0;

    // Read request from input file
    FILE *fp = fopen(argv[1], "r");
    int start; int from; int to; int passenger;
    int read = 0;
    while (!feof(fp) || elevator->status != 2) {
        if (!read && !feof(fp)) {
            read = 1;
            fscanf(fp, "%d %d %d %d", &start, &passenger, &from, &to);
        }
        if (read && elevator->timestamp == start) {
            handle_call(elevator, request_init(start, from, to, passenger, 0));
            read = 0;
        }
        tick(elevator);
    }

    // Cleanup
    fclose(fp);
    float avg_wait_time = (float)elevator->wait_time / elevator->total_passengers;
    float avg_turnaround_time = (float)elevator->turn_around_time / elevator->total_passengers;
    fprintf(elevator->fp, "Average wait time = %d/%d = %.2fs.\n", elevator->wait_time, elevator->total_passengers, avg_wait_time);
    fprintf(elevator->fp, "Average turnaround time = %d/%d = %.2fs.\n", elevator->turn_around_time, elevator->total_passengers, avg_turnaround_time);
    fclose(elevator->fp);
    pqueue_clear(elevator->first);
    pqueue_clear(elevator->second);
    pqueue_clear(elevator->third);
    free(elevator);
    return 0;
}