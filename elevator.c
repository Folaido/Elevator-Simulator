//
// Created by FF on 2020-10-07.
//

#include "elevator.h"
#include "priorityqueue.h"
#include <stdio.h>

/*
 * Internal Use Only
 * The function determines whether the elevator can respond all calls if the elevator turn direction in current floor
 * Input: Elevator *
 * Output: 1 if true, 0 false
 */
int can_arrive(Elevator *elevator) {
    return elevator->direction ? elevator->current_floor <= pqueue_top(elevator->second)->request.floor : elevator->current_floor >= pqueue_top(elevator->second)->request.floor;
}

/*
 * Internal Use Only
 * The function writes a message to output file when the elevator finish a pick-up call
 * Input: Elevator *, Request *
 * Output: None
 */
void print_pickup(Elevator *elevator, Request *request) {
    fprintf(elevator->fp, "Time %d: The elevator picked up %d passengers at floor %d.\n", elevator->timestamp, request->passengers, elevator->current_floor);
}

/*
 * Internal Use Only
 * The function writes a message to output file when the elevator finish a drop-off call
 * Input: Elevator *, Request *
 * Output: None
 */
void print_arrive(Elevator *elevator, Request *request) {
    fprintf(elevator->fp, "Time %d: The elevator dropped off %d passengers at floor %d.\n", elevator->timestamp, request->passengers, elevator->current_floor);
}

/*
 * Internal Use Only
 * The function will turn the direction of the elevator
 * Input: Elevator *
 * Output: None
 */
void turn_direction(Elevator *elevator) {
    elevator->direction ^= 1;
    // move all elements from second to first
    while (!pqueue_is_empty(elevator->second)) {
        QNode *node = pqueue_top(elevator->second);
        pqueue_pop(elevator->second);
        pqueue_push(elevator->first, node->key * -1, &node->request);
    }
    // move all elements from third to second
    while (!pqueue_is_empty(elevator->third)) {
        QNode *node = pqueue_top(elevator->third);
        pqueue_pop(elevator->third);
        pqueue_push(elevator->second, node->key * -1, &node->request);
    }
}

/*
 * Internal Use Only
 * The function determines the elevator condition for next timestamp
 * Input: Elevator *
 * Output: None
 */
void set_condition(Elevator *elevator) {
    /*
     * Case 1: No remaining call left, rest the elevator
     * Case 2: Only remaining calls are in the same direction and already passed by, change direction and call the function again
     * Case 3: All calls in the same direction and can process in current round are finished and elevator can
     *         respond all calls if the elevator turn direction in current floor, just turn the direction
     * Case 4: All calls in the same direction and can process in current round are finished but elevator cannot
     *         respond all calls. Need to go to the floor that can respond all calls before turning direction.
     * Default case: set counter to 0
     */
    if (pqueue_is_empty(elevator->first) && pqueue_is_empty(elevator->second) && pqueue_is_empty(elevator->third)) {
        elevator->status = 2;
    } else if (pqueue_is_empty(elevator->first) && pqueue_is_empty(elevator->second)) {
        turn_direction(elevator);
        set_condition(elevator);
    } else if (pqueue_is_empty(elevator->first) && can_arrive(elevator)) {
        turn_direction(elevator);
        elevator->status = 0;
    } else if (pqueue_is_empty(elevator->first)) {
        Request *internal_request = request_init(elevator->timestamp, pqueue_top(elevator->second)->request.floor, 0, 0, 2);
        int dir = elevator->direction ? -1 : 1;
        pqueue_push(elevator->first, dir * internal_request->floor, internal_request);
        elevator->status = 1;
    }
    elevator->counter = 0;
}

/*
 * Internal Use Only
 * The functions process all calls happen in current floor in current directions
 * Input: Elevator *
 * Output: None;
 */
void process_current_floor(Elevator *elevator) {
    while (!pqueue_is_empty(elevator->first) && pqueue_top(elevator->first)->request.floor == elevator->current_floor) {
        QNode *cur_node = pqueue_top(elevator->first);
        pqueue_pop(elevator->first);
        if (cur_node->request.status == 0) {
            // set request status from pick-up to drop-down
            elevator->wait_time += (elevator->timestamp - cur_node->request.start_time) * cur_node->request.passengers;
            elevator->total_passengers += cur_node->request.passengers;
            cur_node->request.floor = cur_node->request.to;
            cur_node->key = cur_node->key > 0 ? cur_node->request.floor : -cur_node->request.floor;
            cur_node->request.status = 1;
            pqueue_push(elevator->first, cur_node->key, &cur_node->request);
            print_pickup(elevator, &cur_node->request);
        } else if (cur_node->request.status == 1) {
            // if the request is already a drop-down, the entire request is finished
            elevator->turn_around_time += (elevator->timestamp - cur_node->request.start_time) * cur_node->request.passengers;
            print_arrive(elevator, &cur_node->request);
        }
    }
}

/*
 * API
 * The function handle a call request
 * Input: Elevator *, Request *
 * Output: None
 */
void handle_call(Elevator *elevator, Request *request) {
    // If the request is going up
    if (request->floor < request->to) {
        if (!elevator->direction) {
            if (request->floor < elevator->current_floor || (request->floor == elevator->current_floor && elevator->status == 1)) {
                pqueue_push(elevator->third, request->floor, request);
            } else {
                pqueue_push(elevator->first, request->floor, request);
            }
        } else {
            pqueue_push(elevator->second, -request->floor, request);
        }

    } else {
        if (elevator->direction) {
            if (request->floor > elevator->current_floor || (request->floor == elevator->current_floor && elevator->status == 1)) {
                pqueue_push(elevator->third, -request->floor, request);
            } else {
                pqueue_push(elevator->first, -request->floor, request);
            }
        } else {
            pqueue_push(elevator->second, request->floor, request);
        }
    }
}

/*
 * API
 * The function simulates the elevator in one timestamp
 * Input: Elevator *
 * Output: None
 */
void tick(Elevator *elevator) {
    // When the elevator is resting
    if (elevator->status == 2 ) {
        // Handle special case when the pick up floor is the current floor
        if (!pqueue_is_empty(elevator->first) && pqueue_top(elevator->first)->request.floor == elevator->current_floor) {
            QNode *node = pqueue_top(elevator->first);
            pqueue_pop(elevator->first);
            print_pickup(elevator, &node->request);
            elevator->direction = node->request.floor > node->request.to;
            elevator->total_passengers += node->request.passengers;
            node->request.floor = node->request.to;
            node->key = node->key > 0 ? node->request.floor : -node->request.floor;
            node->request.status = 1;
            pqueue_push(elevator->first, node->key, &node->request);
            elevator->status = 0;
            elevator->counter = 0;
        } else {
            set_condition(elevator);
        }
    } else {
        elevator->counter++;   // increment counter
        // if completes a move
        if (elevator->counter == 5 && elevator->status == 1) {
            elevator->current_floor = elevator->direction ? elevator->current_floor - 1 : elevator->current_floor + 1;
            if (!pqueue_is_empty(elevator->first) && pqueue_top(elevator->first)->request.floor == elevator->current_floor) {
                process_current_floor(elevator);
                set_condition(elevator);
            }
            elevator->counter = 0;
        } else if (elevator->status == 0 && elevator->counter == 2) {
            // if finishes a stop-move transition
            process_current_floor(elevator);
            elevator->status = 1;
            elevator->counter = 0;
        }
        // otherwise, just waiting...
    }
    elevator->timestamp++;     // increment timestamp
}
