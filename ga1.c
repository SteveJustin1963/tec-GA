 

 

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

: mutate ( ind -- )
    16 random mod swap dup @ xor swap !
;

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

