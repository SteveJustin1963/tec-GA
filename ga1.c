#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define CHROMOSOME_SIZE 16
#define POPULATION_SIZE 50
#define NUM_GENERATIONS 100

struct individual {
    int fitness;
    int *chromosome;
};

struct individual* create_individual(int num_genes);
int calculate_fitness(struct individual *ind, int num_genes);
void print_individual(struct individual *ind, int num_genes);
void crossover(struct individual *parent1, struct individual *parent2,
               struct individual *child1, struct individual *child2,
               int num_genes);
void mutate(struct individual *ind, int num_genes);
void print_pop(struct individual **pop, int pop_size, int num_genes);
void selection(struct individual **pop, int pop_size,
               struct individual **parent1, struct individual **parent2);
struct individual** crossover_and_mutation(struct individual **pop, int pop_size, int num_genes);
struct individual* find_best(struct individual **pop, int pop_size);

int main() {
    srand(time(NULL));  // Initialize random seed

    struct individual **pop = malloc(sizeof(struct individual*) * POPULATION_SIZE);
    for (int i = 0; i < POPULATION_SIZE; ++i) {
        pop[i] = create_individual(CHROMOSOME_SIZE);
        calculate_fitness(pop[i], CHROMOSOME_SIZE);
    }

    for (int gen = 0; gen < NUM_GENERATIONS; ++gen) {
        printf("Generation %d:\n", gen);
        pop = crossover_and_mutation(pop, POPULATION_SIZE, CHROMOSOME_SIZE);
    }

    struct individual *best = find_best(pop, POPULATION_SIZE);
    printf("\nBest chromosome in final generation: ");
    print_individual(best, CHROMOSOME_SIZE);
    printf("Fitness = %d\n", best->fitness);

    for (int i = 0; i < POPULATION_SIZE; ++i) {
        free(pop[i]->chromosome);
        free(pop[i]);
    }
    free(pop);

    return 0;
}

struct individual* create_individual(int num_genes) {
    struct individual *ind = malloc(sizeof(struct individual));
    ind->chromosome = malloc(sizeof(int) * num_genes);
    ind->fitness = 0;
    for (int i = 0; i < num_genes; ++i) {
        ind->chromosome[i] = rand() % 2;
    }
    return ind;
}

int calculate_fitness(struct individual *ind, int num_genes) {
    ind->fitness = 0;
    for (int i = 0; i < num_genes; ++i) {
        ind->fitness += ind->chromosome[i];
    }
    return ind->fitness;
}

void print_individual(struct individual *ind, int num_genes) {
    for (int i = 0; i < num_genes; ++i) {
        printf("%d", ind->chromosome[i]);
    }
    printf("\n");
}

void crossover(struct individual *parent1, struct individual *parent2,
               struct individual *child1, struct individual *child2,
               int num_genes) {
    int crossover_point = num_genes / 2;

    for (int i = 0; i < crossover_point; ++i) {
        child1->chromosome[i] = parent1->chromosome[i];
        child2->chromosome[i] = parent2->chromosome[i];
    }
    for (int i = crossover_point; i < num_genes; ++i) {
        child1->chromosome[i] = parent2->chromosome[i];
        child2->chromosome[i] = parent1->chromosome[i];
    }
}

void mutate(struct individual *ind, int num_genes) {
    int i = rand() % num_genes;
    ind->chromosome[i] = !ind->chromosome[i];
}

void print_pop(struct individual **pop, int pop_size, int num_genes) {
    for (int i = 0; i < pop_size; ++i) {
        print_individual(pop[i], num_genes);
    }
}

void selection(struct individual **pop, int pop_size,
               struct individual **parent1, struct individual **parent2) {
    // Same as your code
}

struct individual** crossover_and_mutation(struct individual **pop, int pop_size, int num_genes) {
    // Same as your code, just add num_genes as a parameter where needed
}
