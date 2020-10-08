//
// Created by FF on 2020-10-07.
//

#include "priorityqueue.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

Request *request_init(int start, int f, int t, int p, int s) {
    Request *request = (Request*)malloc(sizeof(Request));
    request->start_time = start;
    request->floor = f;
    request->to = t;
    request->passengers = p;
    request->status = s;
    return request;
}

PriorityQueue *pqueue_init(int length){
    if(length < 1){
        perror("length for queue is too small.");
        return NULL;
    }
    PriorityQueue *queue = (PriorityQueue*)malloc(sizeof(PriorityQueue));
    if(!queue){
        perror("alloc mem for queue error.");
        return NULL;
    }
    queue->size = 0;
    queue->length = length;
    queue->list = (QNode**)malloc(length * sizeof(QNode*));
    if(!queue->list){
        perror("alloc mem for list error.");
        free(queue);
        return NULL;
    }
    return queue;
}

int pqueue_is_empty(PriorityQueue *queue){
    if(queue == NULL)
        return 1;
    return queue->size == 0;
}

static int parent(int i){
    return (i - 1) / 2;
}

static int left(int i){
    return 2 * i + 1;
}

static int right(int i){
    return 2 * i + 2;
}

int pqueue_push(PriorityQueue *queue, int key, Request *request){
    if(queue == NULL || request == NULL)
        return 0;
    QNode *node = (QNode*)malloc(sizeof(QNode));
    if(!node){
        perror("alloc mem for node error.");
        return 0;
    }
    node->key = key;
    node->request = *request_init(request->start_time, request->floor, request->to, request->passengers, request->status);
    int i;
    for (i = queue->size;i > 0 && key < queue->list[parent(i)]->key; i = parent(i))
        queue->list[i] = queue->list[parent(i)];
    queue->list[i] = node;
    queue->size++;
    return 1;
}

QNode *pqueue_top(PriorityQueue *queue){
    if(queue == NULL || queue->size == 0)
        return NULL;
    return queue->list[0];
}

int pqueue_pop(PriorityQueue *queue){
    if(queue == NULL || queue->size == 0)
        return 0;
    pqueue_delete(queue, 0);
    return 1;
}

int pqueue_delete(PriorityQueue *queue, int index){
    if(queue == NULL || queue->size == 0 || index < 0 || index >= queue->size)
        return 0;
    free(queue->list[index]);
    int key = queue->list[queue->size - 1]->key;
    int i = index, child;
    while(left(i) < queue->size){
        child = left(i);
        if(right(i) < queue->size && queue->list[left(i)]->key > queue->list[right(i)]->key)
            child = right(i);
        if(key > queue->list[child]->key)
            queue->list[i] = queue->list[child];
        else
            break;
        i = child;
    }
    queue->list[i] = queue->list[queue->size - 1];
    queue->size--;
    return 1;
}

int pqueue_clear(PriorityQueue *queue){
    if(queue == NULL)
        return 0;
    for (int i = 0; i < queue->size; ++i) {
        free(queue->list[i]);
    }
    free(queue);
    return 1;
}