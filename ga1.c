 

 

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
