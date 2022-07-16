#include <stdlib.h>
#include "stack_point.h"

// Definition to struct node
typedef struct node Node;

// Struct of dynamic stack of point
struct stack{
    Node* begin;
    int size;
};

// Struct of node to a dynamic stack of point
struct node{
    Point data;
    Node* next;
};

Stack* stack_create(void){
    // alloc of st
    Stack* st = malloc(sizeof(Stack));

    // if st is allocated
    if(st){
        // initialize the variables of st
        st->begin = NULL;
        st->size = 0;
    }// if

    return st;
}// stack_create

void stack_free(Stack* st){
    Node* rm;

    // if st is allocated
    if(st){
        // while st isn't empty
        while(st->begin){
            // rm receive the fist point
            rm = st->begin;
            // second point becomes the first
            st->begin = rm->next;
            // free the old first point
            free(rm);
        }// if
        // free st
        free(st);
    }// if

    return;
}// stack_free

int stack_push(Stack* st, Point point){
    Node* nwnode;

    // if st isn't allocated
    if(!st)
        return STRUCT_NOT_ALLOC;

    // alloc of nwnode
    nwnode = malloc(sizeof(Node));
    // if nwnode can't be allocated
    if(!nwnode)
        return OUT_OF_MEMORY;

    // nwnode->data receive the variable point
    nwnode->data = point;
    // nwnode->next points to the first point
    nwnode->next = st->begin;
    // nwnode becomes the first point of st
    st->begin = nwnode;
    // update size of st
    st->size++;

    return SUCCESS;
}// stack_push

int stack_pop(Stack* st){
    Node* rm;

    // if st isn't allocated
    if(!st)
        return STRUCT_NOT_ALLOC;
    // if st is empty
    if(!st->size)
        return STRUCT_EMPTY;

    // rm receive the fist point
    rm = st->begin;
    // second point of st becomes the first
    st->begin = rm->next;
    // free the old first point
    free(rm);
    // update size of st
    --st->size;

    return SUCCESS;
}// stack_pop

int stack_top(Stack* st, Point* point){
    // if st isn't allocated
    if(!st)
        return STRUCT_NOT_ALLOC;
    // if st is empty
    if(!st->size)
        return STRUCT_EMPTY;

    // point receive the first point of st
    *point = st->begin->data;

    return SUCCESS;
}// stack_top

int stack_empty(Stack* st){
    // if st isn't allocated
    if(!st)
        return STRUCT_NOT_ALLOC;

    // if st is empty
    if(!st->size)
        // return true(1) to st empty
        return 1;
    else
        // return false(0) to st not empty
        return 0;
}// stack_empty

int stack_size(Stack* st){
    // if st isn't allocated
    if(!st)
        return STRUCT_NOT_ALLOC;

    // return the size of st
    return st->size;
}// stack_size