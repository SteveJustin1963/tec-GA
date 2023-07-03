#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct individual {
    int fitness;
    int *chromosome;
};

struct individual* create_rand_individual(int num) {
    struct individual *ind = (struct individual*)malloc(sizeof(struct individual));
    ind->chromosome = (int*)malloc(sizeof(int) * num);
    ind->fitness = 0;
    for (int i = 0; i < num; i++) {
        if (rand()%2 == 0)
            ind->chromosome[i] = 0;
        else
            ind->chromosome[i] = 1;
    }
    return ind;
}

int calculate_fitness(struct individual *ind) {
    int num = 0;
    for (int i = 0; i < 16; i++) {
        num += ind->chromosome[i];
    }
    ind->fitness = num;
    return num;
}

void print_individual(struct individual *ind) {
    for (int i = 0; i < 16; i++)
        printf("%d", ind->chromosome[i]);
    printf("\n");
}

void crossover(struct individual *parent1, struct individual *parent2,
               struct individual *child1, struct individual *child2) {
    int i;
    for (i = 0; i < 8; i++) {
        child1->chromosome[i] = parent1->chromosome[i];
        child2->chromosome[i] = parent2->chromosome[i];
    }
    for (; i < 16; i++) {
        child1->chromosome[i] = parent2->chromosome[i];
        child2->chromosome[i] = parent1->chromosome[i];
    }
}

void mutate(struct individual *ind) {
    int i = rand() % 16;
    if (ind->chromosome[i] == 0)
        ind->chromosome[i] = 1;
    else
        ind->chromosome[i] = 0;
}

void print_pop(struct individual **pop, int pop_size) {
    for (int i = 0; i < pop_size; i++) {
        print_individual(pop[i]);
    }
}

void selection(struct individual **pop, int pop_size,
               struct individual **parent1, struct individual **parent2) {
    int cum_fit = 0;
    for (int i = 0; i < pop_size; i++)
        cum_fit += pop[i]->fitness;

    int rand_num = rand() % cum_fit;

    int partial_sum = 0;
    for (int i = 0; i < pop_size; i++) {
        partial_sum += pop[i]->fitness;
        if (partial_sum >= rand_num) {
            *parent1 = pop[i];
            break;
        }
    }

    rand_num = rand() % cum_fit;

    partial_sum = 0;
    for (int i = 0; i < pop_size; i++) {
        partial_sum += pop[i]->fitness;
        if (partial_sum >= rand_num) {
            *parent2 = pop[i];
            break;
        }
    }
}

struct individual** crossover_and_mutation(struct individual **pop, int pop_size) {
    struct individual **new_pop =
        (struct individual**)malloc(sizeof(struct individual*) * pop_size);
    for (int i = 0; i < pop_size; i++)
        new_pop[i] = create_rand_individual(16);

    struct individual *parent1, *parent2;
    int i = 0;
    while (i < pop_size) {
        selection(pop, pop_size, &parent1, &parent2);
        crossover(parent1, parent2, new_pop[i], new_pop[i + 1]);
        mutate(new_pop[i]);
        mutate(new_pop[i + 1]);
        calculate_fitness(new_pop[i]);
        calculate_fitness(new_pop[i + 1]);
        i += 2;
    }

    for (i = 0; i < pop_size; i++) {
        free(pop[i]->chromosome);
        free(pop[i]);
    }
    free(pop);

    return new_pop;
}

struct individual* find_best(struct individual **pop, int pop_size) {
    int max_fit = 0, max_index = 0;
    for (int i = 0; i < pop_size; i++) {
        if (pop[i]->fitness > max_fit) {
            max_fit = pop[i]->fitness;
            max_index = i;
        }
    }
    return pop[max_index];
}

int main() {
    srand(time(NULL));

    int pop_size = 50;
    struct individual **pop =
        (struct individual**)malloc(sizeof(struct individual*) * pop_size);
    for (int i = 0; i < pop_size; i++)
        pop[i] = create_rand_individual(16);

    for (int i = 0; i < pop_size; i++)
        calculate_fitness(pop[i]);

    for (int gen = 0; gen < 100; gen++) {
        printf("Generation %d:\n", gen);
        pop = crossover_and_mutation(pop, pop_size);
    }

    struct individual *best = find_best(pop, pop_size);
    printf("\nBest chromosome in final generation: ");
    print_individual(best);
    printf("Fitness = %d\n", best->fitness);

    for (int i = 0; i < pop_size; i++) {
        free(pop[i]->chromosome);
        free(pop[i]);
    }
    free(pop);

    return 0;
}
