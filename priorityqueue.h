//
// Created by FF on 2020-10-07.
//

#ifndef ELEVATOR_PRIORITYQUEUE_H
#define ELEVATOR_PRIORITYQUEUE_H

typedef struct Request{
    int start_time;   // time when call issue
    int floor;    // the current floor going to
    int to;     // the floor to drop off (if the request is pick-up request)
    int passengers;
    int status;      // 0: pick-up, 1: drop-off, 2: internal(no passenger)
} Request;

typedef struct qnode{
    int key;    // priority
    Request request;
} QNode;

typedef struct priorityqueue{
    int length;
    int size;
    QNode **list;
} PriorityQueue;

extern Request *request_init(int start, int f, int t, int p, int s);
// Methods for PriorityQueue
extern PriorityQueue *pqueue_init(int length);
extern int pqueue_is_empty(PriorityQueue *queue);
extern int pqueue_push(PriorityQueue *queue, int key, Request *request);
extern QNode *pqueue_top(PriorityQueue *queue);
extern int pqueue_pop(PriorityQueue *queue);
extern int pqueue_clear(PriorityQueue *queue);
extern int pqueue_delete(PriorityQueue *queue, int index);
#endif //ELEVATOR_PRIORITYQUEUE_H
