#include "int_stack.h"
#include <stdlib.h>
#include <stdio.h>

void int_stack_init(int_stack_t *stk, int capacity) {
    SLIST_INIT(&stk->head);
    stk->size = 0;
    stk->capacity = capacity;
}

int int_stack_push(int_stack_t *stk, int value) {
    if (stk->size >= stk->capacity) {
        printf("Stack is at full capacity.\n");
        return 0; // fail
    }

    int_entry_t *newEntry = malloc(sizeof(int_entry_t));
    if (newEntry) {
        newEntry->value = value;
        SLIST_INSERT_HEAD(&stk->head, newEntry, entries);
        stk->size++;
        return 1; //success
    }
    return 0; // fail
}

int int_stack_pop(int_stack_t *stk, int *top_value) {
    int_entry_t *entry = SLIST_FIRST(&stk->head);
    if (entry) {
        int value = entry->value;
        SLIST_REMOVE_HEAD(&stk->head, entries);
        free(entry);
        stk->size--;
        *top_value = value;
        return 1; // success
    }
    return 0; // fail
}

int int_stack_top(int_stack_t *stk, int *top_value) {
    int_entry_t *entry = SLIST_FIRST(&stk->head);
    if (entry) {
        *top_value = entry->value;
        return 1; // success
    }
    return 0; // fail
}

/* Functions for FORTH langauge stack operators */

int int_stack_dup(int_stack_t *stk) {
    if (stk->size < 1)
        return 0;
    int top_value;
    int_stack_top(stk, &top_value);
    return int_stack_push(stk, top_value); // success only if last operation succeeds
}

int int_stack_swap(int_stack_t *stk) {
    if (stk->size < 2)
        return 0;
    int top_value, next_to_top_value;
    int_stack_pop(stk, &top_value);
    int_stack_pop(stk, &next_to_top_value);
    int_stack_push(stk, top_value);
    return int_stack_push(stk, next_to_top_value); // success only if last operation succeeds
}

/* New stack operations */
int int_stack_over(int_stack_t *stk) {
    if (stk->size < 2)
        return 0;
    
    int_entry_t *entry = SLIST_FIRST(&stk->head);
    if (!entry || !(entry->entries.sle_next))
        return 0; // Fail if there's no second item.
    
    int second_value = entry->entries.sle_next->value;
    return int_stack_push(stk, second_value);
}


int int_stack_rot(int_stack_t *stk) {
    if (stk->size < 3) return 0;

    int temp[3];
    for (int i = 0; i < 3; ++i) {
        if (!int_stack_pop(stk, &temp[i])) return 0; // Fail if pop fails
    }

    // Re-add them in rotated order: second, first, third from the top
    int_stack_push(stk, temp[0]); // Originally third
    int_stack_push(stk, temp[1]); // Originally first
    int_stack_push(stk, temp[2]); // Originally second

    return 1;
}



int int_stack_drop(int_stack_t *stk) {
    if (stk->size < 1)
        return 0;
    int top_value;
    return int_stack_pop(stk, &top_value);
}

int int_stack_2swap(int_stack_t *stk) {
    if (stk->size < 4)
        return 0;
    int d1, d2, d3, d4;
    int_stack_pop(stk, &d1);
    int_stack_pop(stk, &d2);
    int_stack_pop(stk, &d3);
    int_stack_pop(stk, &d4);
    int_stack_push(stk, d2);
    int_stack_push(stk, d1);
    int_stack_push(stk, d4);
    return int_stack_push(stk, d3);
}

int int_stack_2dup(int_stack_t *stk) {
    if (stk->size < 2)
        return 0;
    
    int top_value, second_value;
    int_stack_pop(stk, &top_value);
    int_stack_pop(stk, &second_value);
    
    // Push them back in reverse order to maintain the original stack order
    int_stack_push(stk, second_value);
    int_stack_push(stk, top_value);
    int_stack_push(stk, second_value);
    int_stack_push(stk, top_value);
    
    return 1;
}


int int_stack_2over(int_stack_t *stk) {
    if (stk->size < 4) return 0;

    int temp[4];
    // Temporarily remove the top four elements to access the third and fourth
    for (int i = 0; i < 4; ++i) {
        if (!int_stack_pop(stk, &temp[i])) return 0; // Fail if pop fails
    }

    // Re-add the third and fourth elements first to copy them to the top
    int_stack_push(stk, temp[1]); // Originally fourth
    int_stack_push(stk, temp[2]); // Originally third

    // Re-add the original top four elements
    for (int i = 3; i >= 0; --i) {
        int_stack_push(stk, temp[i]);
    }

    return 1;
}



int int_stack_2drop(int_stack_t *stk) {
    if (stk->size < 2)
        return 0;
    int d1, d2;
    int_stack_pop(stk, &d1);
    return int_stack_pop(stk, &d2);
}

/* Example of how to create a binary operator that works o top two elements (if present) */

int int_stack_add(int_stack_t *stk) {
    if (stk->size < 2)
        return 0;
    int top_value, next_to_top_value;
    int_stack_pop(stk, &top_value);
    int_stack_pop(stk, &next_to_top_value);
    return int_stack_push(stk, top_value + next_to_top_value);
}

void int_stack_print(int_stack_t *stk, FILE *file) {
    int_entry_t *entry;
    int pos = 0;
    if (stk->size == 0) {
        fprintf(file, "empty stack\n");
    }

    SLIST_FOREACH(entry, &stk->head, entries) {
        fprintf(file, "%d: %d\n", pos, entry->value);
        pos++;
    }
    printf("\n");
}

int int_stack_size(int_stack_t* stk) {
    return stk->size;
}

int int_stack_capacity(int_stack_t* stk) {
    return stk->capacity;
}