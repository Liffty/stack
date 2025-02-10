//  main.c
//  StackLøve.c
//
//  Created by jonas jakobsen on 10/02/2025.
//
//  Basic Compilation:
//  gcc -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Werror main.c -o new_stack.out
//
//  Full Warnings and Analysis:
//  gcc -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Werror \
//  -Wstrict-prototypes -Wold-style-definition -Wmissing-prototypes \
//  -Wmissing-declarations -Wfloat-equal -Wformat=2 \
//  -fanalyzer main.c -o new_stack.out
//


#include <stdio.h> // Standard I/O functions (printf, etc.)
#include <stdlib.h> // Memory allocation (malloc, realloc, free)
#include <assert.h> // Assertion for debugging

#define INITIAL_CAPACITY 2 //Initial stack capacity

#ifndef STACK_H // Hvis stack_h ikke er defineret...
#define STACK_H // Definer stack_h, så vi ikke inkludere filen flere gange

//Forward declaration of struct stack
struct stack;

//Functions prototypes for stack operations
struct stack *stack_alloc(void);        //Allocate and initialize stack
int stack_height(struct stack *s);      // Return current number of elements in stack
int stack_capacity(struct stack *s);    // Return the current capacity of the stack
int stack_print(struct stack *s);       // Print stack contents
int stack_push(struct stack *s, int x); // Push an element onto the stack
int stack_pop(struct stack *s, int *dst); //Pop an element from the stack

#endif

/*
 * Struct defination: stack
 * Denne struct repræsentere en dynamisk stack med:
 *  -Data: Pointer til et array af integers (Stack elementet)
 *  -Height: Antallet af elementer i stacken på tidspunktet
 *  -Capacity: Hvor mange elementer som kan være i stacken
 */
struct stack {
    int *data;      //Dynamic array for sorting stack elements.
    int height;     //Number of elements in stack
    int capacity;   //Current capacity of stack
};

/*
 *Funktion: stack_alloc
 *--------------------
 *Allocats memory for a new stack and initializes it.
 *Returns:
 *  -Pointer to a new stack if succesful
 *  -NULL if memory allocation fails
 */
struct stack *stack_alloc(void) {
    struct stack *s = (struct stack *)malloc(sizeof(struct stack)); // Allocate memory for stack structure

    //Check om malloc fejled
    if (s == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for stack structure. \n");
        return NULL;
    }

    s->data = (int *)malloc(INITIAL_CAPACITY * sizeof(int)); // Allocate memory for stack elements

    //Check om malloc fejlede for stack data
    if (s->data == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for stack structure. \n");
        free(s); //Free tidligere brugt allokeret stack struktur
        return NULL;
    }

    // Initialize stack properties
    s->height = 0;
    s->capacity = INITIAL_CAPACITY;

    return s;
}

/*
 *Funktion: stack_height
 *-----------------------
 *Retunere antallet af elementer i stacken
 *Sikre at stack pointeren er valid før den bliver brugt
 */
int stack_height(struct stack *s) {
    // valider stack pointer ikke er NULL
    assert(s != NULL);

    return s->height;
}

/*
 *Function: Stack_capacity
 *------------------------
 *Returns the current capacity of the stack.
 */
int stack_capacity(struct stack *s) {
    assert(s != NULL); //Tjekker om stack pointeren ikke er NULL
    return s->capacity;
}


/*
 *Function: stack_print
 *-----------------------
 *Prints the elements in the stack in the format: "37 42 34 . \n"
 *If the stack is empty, prints ".\n"
 */
int stack_print(struct stack *s) {

    //Valider input
    assert(s != NULL);

    if (s->height == 0) {
        printf(".\n"); //For en empty stack
        return 0;  //Retunere 0 som succes
    }

    //Print alle elemnter i stacken
    for (int i = 0; i < s->height; i++) {
        printf("%d ", s->data[i]);
        if (i < s->height - 1) {
            printf(" "); //Mellemrum i mellem
        }
    }

    printf(" .\n"); //End marker
    return 0;
}

/*
 *Function: stack_push
 *-------------------
 *Pushes an element into the stack.
 *If the stack is full, it doubles its capacity before adding the new element
 *Returns:
 *  - 0 on succes
 *  - -1 if memory allocation fails'
 *  Det var det han sagde til forlæsningerne
 */
int stack_push(struct stack *s, int x) {
    assert(s != NULL); //Validere pointer som vi plejer

    //Hvis stacken er fuld, fordobl dens kapacitet
    if (s->height == s->capacity) {
        int new_capacity = s->capacity * 2; //Her fordubler vi den
        int *new_data = realloc(s->data, (size_t)new_capacity * sizeof(int));

        if (new_data == NULL) {
            return -1; //Hvis allokeringen af hukkomelse fejlede
        }

        s->data = new_data;
        s->capacity = new_capacity;
    }


    //tilføj et element
    s->data[s->height] = x; // Tilføjer et element til stacken
    s->height++; //Increase stack height

    return 0; //Succes
}



/*
 *Function: stack_pop
 *-------------------
 *Removes the top element from the stack and stores it in 'dst'
 *If the stack becomes too empty, it shrinks its capacity
 *Returns:
 *  - 0 on succes
 *  - -1 if the is empty
 */
int stack_pop(struct stack *s, int *dst) {
    assert(s != NULL); //Valider pointer ikke er NULL


    if (s->height == 0) {
        printf("Stack is empty! Cannot pop.\n");
        return -1; //Fejl, for stacken er tom
    }

    *dst = s->data[s->height - 1]; //Gem værdien
    s->height--; //Reducer højden


    //shring stack hvis den bliver for tom (under 1/4 af kapaciteten)
    int new_capacity = s->capacity;
    if (s->height > 0 && s->height <= s->capacity / 4) {
        new_capacity = s->capacity / 2;
        if (new_capacity < 2) { // Behold en minimumskapacitet på 2
            new_capacity = 2;
        }
    }

//"Shrinking skal kun ske hvis capaciteten ændre sig
    if(new_capacity < s->capacity) {
        printf("Shrinking triggered! Old capacity: %d, New capacity: %d\n", s->capacity, new_capacity); //DEBUG

        int *new_data = realloc(s->data, (size_t)new_capacity * sizeof(int));
        if (new_data != NULL) {
            s->data = new_data;
            s->capacity = new_capacity;
            printf("Stack shrunk to capacity: %d\n", s->capacity);
        }
    }

    return 0; //succes
}



// =============================================
//                TEST FUNCTION
// =============================================
int main(void) {
    struct stack *s = stack_alloc(); //Lav en stack

    if (s == NULL) {
        return EXIT_FAILURE; //stop hvis allokering fejlede
    }

    int popped_value; //Gem popped værdi

    //Test for push operations
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
