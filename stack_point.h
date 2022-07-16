#ifndef STACK_POINT_H_INCLUDED
#define STACK_POINT_H_INCLUDED

#include "return.h"

// Definition to struct stack
typedef struct stack Stack;

// Type struct for a point
typedef struct{
    int x, y;
}Point;

// Create a Stack of Point
Stack* stack_create(void);

// Free a Stack of Point
void stack_free(Stack*);

// Push a new point on the stack
int stack_push(Stack*, Point);

// Pop the first point of the stack
int stack_pop(Stack*);

// Put the top point of the stack in a single variable of point
int stack_top(Stack*, Point*);

// Returns 1 if the stack is empty and 0 if not
int stack_empty(Stack*);

// Returns the size of the stack
int stack_size(Stack*);

#endif // STACK_POINT_H_INCLUDED