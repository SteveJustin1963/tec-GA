;;;;
;
;
#include <stdio.h> 
#include <stdlib.h> 
  
// A structure to represent an individual in a population 
struct individual 
{ 
    int fitness; 
    int *chromosome; 
}; 
  
// A utility function to create a random individual 
struct individual* create_rand_individual(int num) 
{ 
    struct individual *ind = 
              (struct individual*)malloc(sizeof(struct individual)); 
    ind->chromosome = (int*)malloc(sizeof(int) * num); 
    ind->fitness = 0; 
    for (int i = 0; i < num; i++) 
    { 
        if (rand()%2 == 0) 
            ind->chromosome[i] = 0; 
        else
            ind->chromosome[i] = 1; 
    } 
    return ind; 
} 
  
// A utility function to calculate fitness of an individual 
int calculate_fitness(struct individual *ind) 
{ 
    int num = 0, i; 
    for (i = 0; i < 16; i++) 
    { 
        num += ind->chromosome[i]; 
    } 
    ind->fitness = num; 
    return num; 
} 
  
// A utility function to print an individual 
void print_individual(struct individual *ind) 
{ 
    for (int i = 0; i < 16; i++) 
        printf("%d", ind->chromosome[i]); 
    printf("\n"); 
} 
  
// A utility function to crossover two individuals at a point 
void crossover(struct individual *parent1, 
                struct individual *parent2, 
                struct individual *child1, 
                struct individual *child2) 
{ 
    int i; 
    for (i = 0; i < 8; i++) 
    { 
        child1->chromosome[i] = parent1->chromosome[i]; 
        child2->chromosome[i] = parent2->chromosome[i]; 
    } 
    for (; i < 16; i++) 
    { 
        child1->chromosome[i] = parent2->chromosome[i]; 
        child2->chromosome[i] = parent1->chromosome[i]; 
    } 
} 
  
// A function to mutate an individual 
void mutate(struct individual *ind) 
{ 
    int i = rand() % 16; 
    if (ind->chromosome[i] == 0) 
        ind->chromosome[i] = 1; 
    else
        ind->chromosome[i] = 0; 
} 
  
// A function to print entire population 
void print_pop(struct individual **pop, int pop_size) 
{ 
    for (int i = 0; i < pop_size; i++) 
    { 
        print_individual(pop[i]); 
    } 
} 
  
// A function to select two parents using roulette wheel selection 
void selection(struct individual **pop, int pop_size, 
                                          struct individual *parent1, 
                                          struct individual *parent2) 
{ 
    // Calculate cumulative fitness of entire population 
    int cum_fit = 0; 
    for (int i = 0; i < pop_size; i++) 
        cum_fit += pop[i]->fitness; 
  
    // Generate a random number between 0 and cumulative fitness 
    int rand_num = (rand() % cum_fit) + 1; 
  
    // Find the individual having cum_fitness >= rand_num 
    // using binary search 
    // Refer: https://www.geeksforgeeks.org/find-individual-having-cumulative-fitness-cumfit/ 
    int l = 0, h = pop_size-1; 
    while (h - l > 1) 
    { 
        int m = (l + h)/2; 
        if (cum_fit >= rand_num) 
            h = m; 
        else
            l = m; 
    } 
  
    int i; 
    for (i = l; i <= h; i++) 
    { 
        if (cum_fit >= rand_num) 
            break; 
        cum_fit -= pop[i]->fitness; 
    } 
  
    parent1 = pop[i]; 
  
    // Generate a random number between 0 and cumulative fitness 
    rand_num = (rand() % cum_fit) + 1; 
  
    // Find the individual having cum_fitness >= rand_num 
    // using binary search 
    l = 0, h = pop_size-1; 
    while (h - l > 1) 
    { 
        int m = (l + h)/2; 
        if (cum_fit >= rand_num) 
            h = m; 
        else
            l = m; 
    } 
  
    for (i = l; i <= h; i++) 
    { 
        if (cum_fit >= rand_num) 
            break; 
        cum_fit -= pop[i]->fitness; 
    } 
  
    parent2 = pop[i]; 
} 
  
// A function to perform crossover and mutation on entire population 
// and generate new population 
void crossover_and_mutation(struct individual **pop, int pop_size) 
{ 
    struct individual **new_pop = 
                          (struct individual**)malloc(sizeof(struct individual*) * pop_size); 
    for (int i = 0; i < pop_size; i++) 
        new_pop[i] = (struct individual*)malloc(sizeof(struct individual)); 
  
    struct individual *parent1, *parent2; 
    int i = 0; 
    while (i < pop_size) 
    { 
        // Select two parents 
        selection(pop, pop_size, &parent1, &parent2); 
  
        // Perform crossover
        crossover(parent1, parent2, new_pop[i], new_pop[i+1]); 
  
        // Perform mutation on new population 
        mutate(new_pop[i]); 
        mutate(new_pop[i+1]); 
  
        // Calculate fitness of new population 
        calculate_fitness(new_pop[i]); 
        calculate_fitness(new_pop[i+1]); 
        i += 2; 
    } 
  
    for (i = 0; i < pop_size; i++) 
    { 
        free(pop[i]->chromosome); 
        free(pop[i]); 
    } 
    free(pop); 
  
    pop = new_pop; 
} 
  
// A function to find the best individual in a population 
struct individual* find_best(struct individual **pop, int pop_size) 
{ 
    int max_fit = 0, max_index = 0; 
    for (int i = 0; i < pop_size; i++) 
    { 
        if (pop[i]->fitness > max_fit) 
        { 
              max_fit = pop[i]->fitness; 
              max_index = i; 
        } 
    } 
    return pop[max_index]; 
} 
  
// Driver code 
int main() 
{ 
    srand(time(NULL)); 
  
    int pop_size = 50; 
    struct individual **pop = 
              (struct individual**)malloc(sizeof(struct individual*) * pop_size); 
    for (int i = 0; i < pop_size; i++) 
        pop[i] = create_rand_individual(16); 
  
    for (int i = 0; i < pop_size; i++) 
        calculate_fitness(pop[i]); 
  
    for (int gen = 0; gen < 100; gen++) 
    { 
        printf("Generation %d:\n", gen); 
        crossover_and_mutation(pop, pop_size); 
    } 
  
    struct individual *best = find_best(pop, pop_size); 
    printf("\nBest chromosome in final generation: "); 
    print_individual(best); 
    printf("Fitness = %d\n", best->fitness); 
  
    for (int i = 0; i < pop_size; i++) 
    { 
        free(pop[i]->chromosome); 
        free(pop[i]); 
    } 
    free(pop); 
  
    return 0; 
}


\\\\\\\\\\\\\\\\\\
sudo

struct individual {
    int fitness; 
    int *chromosome; 
}

function create_rand_individual(num) {
    ind = new individual
    ind->chromosome = new int[num]
    ind->fitness = 0
    for i = 0 to num-1 {
        if (random()%2 == 0) {
            ind->chromosome[i] = 0
        } else {
            ind->chromosome[i] = 1
        }
    } 
    return ind
} 

function calculate_fitness(ind) {
    num = 0
    for i = 0 to 15 {
        num += ind->chromosome[i]
    } 
    ind->fitness = num
    return num
} 

function print_individual(ind) {
    for i = 0 to 15 {
        print ind->chromosome[i]
    }
    print("\n")
} 

function crossover(parent1, parent2, child1, child2) {
    for i = 0 to 7 {
        child1->chromosome[i] = parent1->chromosome[i]
        child2->chromosome[i] = parent2->chromosome[i]
    } 
    for i = 8 to 15 {
        child1->chromosome[i] = parent2->chromosome[i]
        child2->chromosome[i] = parent1->chromosome[i]
    } 
} 

function mutate(ind) {
    i = random() % 16
    if (ind->chromosome[i] == 0) {
        ind->chromosome[i] = 1
    } else {
        ind->chromosome[i] = 0
    }
} 

function print_pop(pop, pop_size) {
    for i = 0 to pop_size-1 {
        print_individual(pop[i])
    }
} 

function selection(pop, pop_size, parent1, parent2) {
    cum_fit = 0
    for i = 0 to pop_size-1 {
        cum_fit += pop[i]->fitness
    }
    rand_num = (random() % cum_fit) + 1
    l = 0
    h = pop_size-1
    while (h - l > 1) {
        m = (l + h)/2
        if (cum_fit >= rand_num) {
            h = m
        } else {
            l = m
        }
    }
    parent1 = pop[h]
    // repeat process to select second parent
}
\\\\\\\\\\\\\\\\\\\
forth 83


: create_rand_individual ( num -- ind )
    create ind ,
    num cells allot
     \\  1
     0 >r   
    : loop ( num -- )
        dup 0 mod 2 = 
        if
            0 swap !
        else
            1 swap !
        then
        1 - dup 0 >
        if
            loop
        else
            drop
        then
    ;
    r> 0 loop
    0 ind !
;

: calculate_fitness ( ind -- num )
 \\2   
 
 0 >r
    : loop ( ind -- )
        dup @ 0 =
        if
            r> drop
        else
            1 + r> swap
            1 - dup 0 >
            if
                loop
            else
                drop
            then
        then
    ;
    0 ind @ loop
    r> swap !
;

\\3
: print_individual ( ind -- )
    0 >r
    : loop ( ind -- )
        dup @ 0 =
        if
            r> drop
        else
            dup @ .
            1 + r> swap
            1 - dup 0 >
            if
                loop
            else
                drop
            then
        then
    ;
    0 ind @ loop
    cr
;

\\4
: crossover ( parent1 parent2 child1 child2 -- )
    8 0 >r
    : loop ( parent1 parent2 child1 child2 -- )
        dup @ swap @ swap !
        1 + r> swap
        8 + dup @ swap @ swap !
        1 + r> swap
        1 - dup 0 >
        if
            loop
        else
            drop
        then
    ;
    0 loop
;
\\5
: mutate ( ind -- )
    16 random mod swap dup @ xor swap !
;
\\6
: print_pop ( pop pop_size -- )
    0 >r
    : loop ( pop pop_size -- )
        dup @ 0 =
        if
            r> drop
        else
            dup @ print_individual
            1 + r> swap
            1 - dup 0 >
            if
                loop
            else
                drop
            then
        then
    ;
    0 pop @ loop
;
\\7
: selection ( pop pop_size parent1 parent2 -- )
    0 >r
    0 >r
    : loop ( pop pop_size -- )
        dup @ 0 =
        if
            r> drop
            r> swap !
        else
            dup @ swap @ swap + >r
            1 + r> swap
            1 - dup 0 >
            if
                loop
            else
                drop
            then
        then
    ;
    0 pop @ loop
    r> random swap mod + swap !
    r> swap !
;
\\\\\\\\\\\\\\\\\\\\\

\\1
"0 >r"   take the value 0, and move it to the return stack.
"dup" duplicates the value of "num" on top of the stack.
"0 mod 2 =" performs a modulus operation with 2 and compares the result to 0. 
 If the result is 0, it leaves the value "true" on the stack, otherwise it leaves "false".
"if" starts a conditional block. If the value on top of the stack is "true", the code inside the block is executed, otherwise it is skipped.
"0 swap !" is executed if the value on top of the stack was "true". It takes the value 0 from the stack and swaps it with the value at the address stored in the "num" variable.
"1 swap !" is executed if the value on top of the stack was "false". It takes the value 1 from the stack and swaps it with the value at the address stored in the "num" variable.
"1 - dup 0 >" decrements the value of "num" by 1, duplicates the new value of "num" and compares it to 0. If the new value of "num" is greater than 0, it leaves "true" on the stack, otherwise it leaves "false".
"if" starts another conditional block. If the value on top of the stack is "true", the code inside the block is executed, otherwise it is skipped.
"loop" is executed if the value on top of the stack was "true", it calls the "loop" word again with the new value of "num".
"drop" is executed if the value on top of the stack was "false". It discards the top value on the stack.

 This code is implementing a loop that will repeat the number of times specified by the parameter "num". On each iteration, 
it performs a modulus operation with 2 on "num", if the remainder is 0 it will store 0 in the address of "num" 
 otherwise it will store 1. Then it decrement the value of "num" by 1 and check if the new value is greater than 0, 
if yes it calls the loop word again otherwise it drop the value of "num" and exit the loop.
 
 
 \\2 
 defines a word called "loop" that takes one parameter "ind" and performs the following operations:

"0 >r" moves the value of 0 to the return stack.
": loop ( ind -- )" starts the definition of a new word called "loop" that takes one parameter "ind".
"dup @" duplicates the value of "ind" on top of the stack and fetches the value stored at the address of "ind".
"0 =" compares the value on top of the stack to 0. If they are equal, it leaves the value "true" on the stack, otherwise it leaves "false".
"if" starts a conditional block. If the value on top of the stack is "true", the code inside the block is executed, otherwise it is skipped.
"r> drop" is executed if the value on top of the stack was "true". It takes the value from the return stack and discards it.
"else" starts the else block.
"1 + r> swap" is executed if the value on top of the stack was "false". It increments the value on top of the stack by 1, takes the value from the return stack and swaps it with the value on top of the stack.
"1 - dup 0 >" decrements the value of "ind" by 1, duplicates the new value of "ind" and compares it to 0. If the new value of "ind" is greater than 0, it leaves "true" on the stack, otherwise it leaves "false".
"if" starts another conditional block. If the value on top of the stack is "true", the code inside the block is executed, otherwise it is skipped.
"loop" is executed if the value on top of the stack was "true", it calls the "loop" word again with the new value of "ind".
"else" starts the else block.
"drop" is executed if the value on top of the stack was "false". It discards the top value on the stack.
"then" ends the conditional block.
";" ends the definition of the "loop" word.
"0 ind @ loop" calls the "loop" word with the parameter "ind" and the initial value 0.
"r> swap !" takes the value from the return stack and swaps it with the value stored at the address of "ind"

The code defines a new word called "loop" that takes a parameter "ind" and performs a specific operation based on the value 
stored at the address of "ind". The word starts by moving the value 0 to the return stack, then it enters a loop that continues 
until the value stored at the address of "ind" is equal to 0.
On each iteration, the word performs the following operations:
Check the value stored at the address of "ind"
If the value is 0, it takes the value from the return stack and discards it.
If the value is not 0, it increments the value stored at the address of "ind" by 1 and swaps it with the value from the return stack.
Decrement the value of "ind" by 1, duplicates the new value of "ind" and compares it to 0.
If the new value of "ind" is greater than 0, it calls the "loop" word again with the new value of "ind".
If the new value of "ind" is not greater than 0, it discards the top value on the stack
After the loop finishes, the code takes the value from the return stack and swaps it with the value stored at the address of "ind".
 
\\3
defines a new word called "print_individual" that takes one parameter "ind" and performs the following operations:
"0 >r" moves the value of 0 to the return stack.
": loop ( ind -- )" starts the definition of a new word called "loop" that takes one parameter "ind".
"dup @" duplicates the value of "ind" on top of the stack and fetches the value stored at the address of "ind".
"0 =" compares the value on top of the stack to 0. If they are equal, it leaves the value "true" on the stack, otherwise it leaves "false".
"if" starts a conditional block. If the value on top of the stack is "true", the code inside the block is executed, otherwise it is skipped.
"r> drop" is executed if the value on top of the stack was "true". It takes the value from the return stack and discards it.
"else" starts the else block.
"dup @ ." fetches the value stored at the address of "ind" and print it.
"1 + r> swap" increments the value stored at the address of "ind" by 1, 
              takes the value from the return stack and swaps it with the value on top of the stack.
"1 - dup 0 >" decrements the value of "ind" by 1, duplicates the new value of "ind" and compares it to 0. 
              If the new value of "ind" is greater than 0, it leaves "true" on the stack, otherwise it leaves "false".
"if" starts another conditional block. If the value on top of the stack is "true", the code inside the block is executed, otherwise it is skipped.
"loop" is executed if the value on top of the stack was "true", it calls the "loop" word again with the new value of "ind".
"else" starts the else block.
"drop" is executed if the value on top of the stack was "false". It discards the top value on the stack.
"then" ends the conditional block.
";" ends the definition of the "loop" word.
"0 ind @ loop" calls the "loop" word with the parameter "ind" and the initial value 0.
"cr" is used to print a new line.

This code is defining a new word called "print_individual" which takes one parameter "ind" and performs a specific operation. 
It starts by moving the value 0 to the return stack, then it enters a loop that continues until the value stored at 
the address of "ind" is equal to 0.

On each iteration, the word performs the following operations:

Check the value stored at the address of "ind"
If the value is 0, it takes the value from the return stack and discards it.
If the value is not 0, it fetches the value stored at the address of "ind" and prints it, increments the value stored at 
the address of "ind" by 1, takes the value from the return stack and swaps it with the value on top of the stack.
Decrement the value of "ind" by 1, duplicates the new value of "ind" and compares it to 0.
If the new value of "ind" is greater than 0, it calls the "loop" word again with the new value of "ind".
If the new value of "ind" is not greater than 0, it discards the top value on the stack
After the loop finishes, the code print a new line.

This code is likely iterating through a specific memory location and printing the values stored at that location, 
possibly as part of a larger program that is working with arrays or other data structures. 

 
\\4
defines a new word called "crossover" that takes four parameters "parent1", "parent2", "child1", and "child2" 
 and performs the following operations:
"8 0 >r" moves the value of 8 to the return stack and 0 on top of the stack.
": loop ( parent1 parent2 child1 child2 -- )" starts the definition of a new word called "loop" that takes four parameters 
      "parent1", "parent2", "child1", and "child2".
"dup @ swap @ swap !" duplicates the value of "parent1" on top of the stack, fetches the value stored at the 
      address of "parent1" and swap it with the value stored at the address of "child1" and stores the value of "parent1" into the "child1" address
"1 + r> swap" increments the value of "parent1" by 1, takes the value from the return stack and swaps it with the value on top of the stack.
"8 + dup @ swap @ swap !" increments the value of "parent1" by 8, duplicates the new value of "parent1" and fetches the 
      value stored at the address of "parent1" and swap it with the value stored at the address of "child2" 
      and stores the value of "parent1" into the "child2" address
"1 + r> swap" increments the value of "parent1" by 1, takes the value from the return stack and swaps it with the value on top of the stack.
"1 - dup 0 >" decrements the value of "parent1" by 1, duplicates the new value of "parent1" and compares it to 0. If the new value 
      of "parent1" is greater than 0, it leaves "true" on the stack, otherwise it leaves "false".
"if" starts a conditional block. If the value on top of the stack is "true", the code inside the block is executed, otherwise it is skipped.
"loop" is executed if the value on top of the stack was "true", it calls the "loop" word again with the new value of "parent1".
"else" starts the else block.
"drop" is executed if the value on top of the stack was "false". It discards the top value on the stack.
"then" ends the conditional block.
";" ends the definition of the "loop" word.
"0 loop" calls the "loop" word with the parameter "parent1" and the initial value 0.

This code is likely part of a genetic algorithm program, where the "crossover" word is used to combine the 
genetic information of two parents to create two children. It appears to be copying the first 8 elements 
of "parent1" to "child1" and the next 8 elements of "parent1" to "child2" .

 
 \\5
 defines a new word called "mutate" that takes one parameter "ind" and performs the following operations:
"16 random mod" generates a random number between 0 and 15 and calculates the remainder of the division of this number by 16.
"swap dup @" swap the top of the stack with the value of "ind" and duplicates the value of "ind" on top of the stack and fetches 
    the value stored at the address of "ind".
"xor" performs a bitwise exclusive or operation between the two values on top of the stack
"swap !" swap the top of the stack with the value of "ind" and stores the value on top of the stack into the address of "ind"

This code is likely mutating a bit in a specific memory location and it is likely a part of a larger program that is working 
with arrays or other data structures, it takes a random bit position and performs a bitwise xor operation with the value stored 
at that position, flipping the value of the bit.
 
\\6
defines a new word called "print_pop" that takes two parameters "pop" and "pop_size" and performs the following operations:
"0 >r" moves the value of 0 to the return stack.
": loop ( pop pop_size -- )" starts the definition of a new word called "loop" that takes two parameters "pop" and "pop_size".
"dup @ 0 =" duplicates the value of "pop" on top of the stack, fetches the value stored at the address of "pop" and compares 
    it to 0. If they are equal, it leaves the value "true" on the stack, otherwise it leaves "false".
"if" starts a conditional block. If the value on top of the stack is "true", the code inside the block is executed, otherwise it is skipped.
"r> drop" is executed if the value on top of the stack was "true". It takes the value from the return stack and discards it.
"else" starts the else block.
"dup @ print_individual" duplicates the value of "pop" on top of the stack, fetches the value stored at the address of "pop" 
     and calls the "print_individual" word with the value stored at the address of "pop" as a parameter.
"1 + r> swap" increments the value of "pop" by 1, takes the value from the return stack and swaps it with the value on top of the stack.
"1 - dup 0 >" decrements the value of "pop" by 1, duplicates the new value of "pop" and compares it to 0. If the new value of "pop" 
     is greater than 0, it leaves "true" on the stack, otherwise it leaves "false".
"if" starts another conditional block. If the value on top of the stack is "true", the code inside the block is executed, otherwise it is skipped.
"loop" is executed if the value on top of the stack was "true", it calls the "loop" word again with the new value of "pop".
"else" starts the else block.
"drop" is executed if the value on top of the stack was "false". It discards the top value on the stack.
"; " ends the definition of the "loop" word.
"0 pop @ loop" call the loop word with the parameters of 0 and the value stored at the address of "pop"
";" ends the definition of the "print_pop" word.

This code is likely used to print an entire population of individuals, each time the loop runs, 
it increments the value of "pop" by 1 and calls the "print_individual" function on the value stored at the address of "pop", 
this is done until the value of "pop" is greater than the value of "pop_size" and the loop ends.
 
\\7
defines a new word called "selection" that takes four parameters "pop", "pop_size", "parent1" and "parent2" and performs the following operations:
"0 >r" and "0 >r" move the value of 0 to the return stack twice.
": loop ( pop pop_size -- )" starts the definition of a new word called "loop" that takes two parameters "pop" and "pop_size".
"dup @ 0 =" duplicates the value of "pop" on top of the stack, fetches the value stored at the address of "pop" and compares it to 0. 
    If they are equal, it leaves the value "true" on the stack, otherwise it leaves "false".
"if" starts a conditional block. If the value on top of the stack is "true", the code inside the block is executed, otherwise it is skipped.
"r> drop" is executed if the value on top of the stack was "true". It takes the value from the return stack and discards it.
"r> swap !" takes the value from the return stack and swaps it with the value on top of the stack, and stores the value on top of the stack 
    into the address of "parent1"
"else" starts the else block.
"dup @ swap @ swap + >r" duplicate the value of "pop" on top of the stack, fetches the value stored at the address of "pop" 
     and swaps it with the value stored at the address of "pop", swap it with the value stored at the address of "pop", 
     and add them together, and move the result to the return stack.
"1 + r> swap" increments the value of "pop" by 1, takes the value from the return stack and swaps it with the value on top of the stack.
"1 - dup 0 >" decrements the value of "pop" by 1, duplicates the new value of "pop" and compares it to 0. If the new value of "pop" 
     is greater than 0, it leaves "true" on the stack, otherwise it leaves "false".
"if" starts another conditional block. If the value on top of the stack is "true", the code inside the block is executed, otherwise it is skipped.
"loop" is executed if the value on top of the stack was "true", it calls the "loop" word again with the new value of "pop".
"else" starts the else block.
"drop" is executed if the value on top of the stack was "false". It discards the top value on the stack.
";" ends the definition of the "loop" word.
"0 pop @ loop" call the loop word with the parameters of 0 and the value stored at the address of "pop"
"r> random swap mod + swap !" takes the value from the return stack, generate a random number, 
     swap it with the value on top of the stack, take the modulus of the value on top of the stack with the random number, 
     and add them together, and store the result into the address of "parent2"
"r> swap !" takes the value from the return stack and swaps it with the value on top of the stack, and stores the value on top of the 
     stack into the address of "parent1"
";" ends the definition of the "selection" word.

This code is likely used to select two parents from a population of individuals using a roulette wheel selection method. 
The loop sums the fitness of each individual in the population and store the result in the return stack. 
After the loop, it generates a random number and uses it to select a parent from the population based on their fitness value, 
and stores the selected parents in the "parent1" and "parent2" variables.

\\\\\\\\\\


 
