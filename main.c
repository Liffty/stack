//
//  main.c
//  StackLøve.c
//
//  Created by jonas jakobsen on 10/02/2025.
//


//  Skal comp med:

/*gcc -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Werror main.c -o new_stack.out
*/

/* gcc -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Werror \
-Wstrict-prototypes -Wold-style-definition -Wmissing-prototypes \
-Wmissing-declarations -Wfloat-equal -Wformat=2 \
-fanalyzer main.c -o new_stack.out
*/


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define INITIAL_CAPACITY 2 //Relg 7

#ifndef STACK_H
#define STACK_H

struct stack; // Forward declaration af struct

struct stack *stack_alloc(void);
int stack_height(struct stack *s);
int stack_capacity(struct stack *s);
int stack_print(struct stack *s);
int stack_push(struct stack *s, int x);
int stack_pop(struct stack *s, int *dst);

#endif

/*
 laver en struct stack
 */
struct stack {
    int *data;
    int height;
    int capacity;
};

/*
 Funktion til at allocere og initializere en stack
 */
struct stack *stack_alloc(void) {
    struct stack *s = (struct stack *)malloc(sizeof(struct stack));
    
    //Check retur typen af malloc for data array eller stack
    if (s == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for stack structure. \n");
        return NULL;
    }
    
    s->data = (int *)malloc(INITIAL_CAPACITY * sizeof(int));
    
    //Check retur typen af malloc for data array eller stack
    if (s->data == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for stack structure. \n");
        free(s);
        return NULL;
    }
    
    s->height = 0;
    s->capacity = INITIAL_CAPACITY;
    
    return s;
}

/*
 Retunere hvor mange elementer som er i vores stack. Altså hvor mange elementer som er blevet pushet og ikke poppet endnu.
 */
int stack_height(struct stack *s) {
    // valider stack pointer
    assert(s != NULL);
    
    return s->height;
}

/*
 Retunere størreslen af vores stack, altså hvor mange elemnter som kan være i vores stack.
 */
int stack_capacity(struct stack *s) {
    assert(s != NULL);
    return s->capacity;
}
 

/*
 Skal printe som: "37 42 34 . \n"
 */
int stack_print(struct stack *s) {
    
    //Valider input
    assert(s != NULL);
    
    if (s->height == 0) {
        printf(".\n");
        return 0;  //Retunere 0 som succes
    }
    
    //Print alle elemnter i stacken
    for (int i = 0; i < s->height; i++) {
        printf("%d ", s->data[i]);
        if (i < s->height - 1) {
            printf(" ");
        }
    }
    
    printf(" .\n");
    return 0;
}

/*
 Skal ligge et elemnet ned i vores stack
 */
int stack_push(struct stack *s, int x) {
    assert(s != NULL);
    
    //Hvis stacken er fuld, fordobl dens kapacitet
    if (s->height == s->capacity) {
        int new_capacity = s->capacity * 2;
        int *new_data = realloc(s->data, (size_t)new_capacity * sizeof(int));
        
        if (new_data == NULL) {
            return -1;
        }
        
        s->data = new_data;
        s->capacity = new_capacity;
    }
    
    
    //tilføj et element
    s->data[s->height] = x;
    s->height++;
    
    return 0; //Succes
}



/*
 Skal tage det øverste element ud af vores stack
 */
int stack_pop(struct stack *s, int *dst) {
    assert(s != NULL);
    
    
    if (s->height == 0) {
        printf("Stack is empty! Cannot pop.\n");
        return -1; //Fejl, for stacken er tom
    }
    
    *dst = s->data[s->height - 1]; //Gem værdien
    s->height--; //Reducer højden
    
    //shring stack hvis den bliver for tom (under 1/4 af kapaciteten)
    int new_capacity = s->capacity;
    
    if (s->height > 0 && s->height < s->capacity / 4) {
        new_capacity = s->capacity / 2;
        if (new_capacity < 2) { // Behold en minimumskapacitet
            new_capacity = 2;
        }
    }
    
    
    if(new_capacity < s->capacity) {
        int *new_data = realloc(s->data, (size_t)new_capacity * sizeof(int));
        if (new_data != NULL) {
            s->data = new_data;
            s->capacity = new_capacity;
            printf("Stack shrunk to capacity: %d\n", s->capacity);
        }
    }
    
    return 0; //succes
}







//TEST FUNKTION
int main(void) {
    struct stack *s = stack_alloc();
    
    if (s == NULL) {
        return EXIT_FAILURE;
    }
    
    int popped_value;

    stack_push(s, 10);
    stack_push(s, 20);
    stack_push(s, 30);
    stack_push(s, 40);  // Burde trigger en resize

    stack_print(s);

    stack_pop(s, &popped_value);
    stack_pop(s, &popped_value);
    stack_pop(s, &popped_value);
    stack_pop(s, &popped_value);  // Burde trigger en shrink

    stack_print(s);
    
    //Clean up
    free(s->data);
    free(s);
    
    return 0;
}

