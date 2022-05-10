// c program using GA to find shortest path between point x and y

#include <stdio.h>
#include <stdlib.h>

// Define a struct for a point
typedef struct {
    int x;
    int y;
} point;

// Calculate the distance between two points
int distance(point p1, point p2) {
    return abs(p1.x - p2.x) + abs(p1.y - p2.y);
}

// Evaluate a path by calculating the total distance between all points
int evaluate(point* path, int length) {
    int i, d = 0;
    for (i = 1; i < length; i++) {
        d += distance(path[i-1], path[i]);
    }
    return d;
}

// Generate a random path
void generate(point* path, int length) {
    int i;
    for (i = 0; i < length; i++) {
        path[i].x = rand() % 100;
        path[i].y = rand() % 100;
    }
}

// Mutate a path by randomly swapping two points
void mutate(point* path, int length) {
    int i1, i2;
    i1 = rand() % length;
    i2 = rand() % length;
    point tmp = path[i1];
    path[i1] = path[i2];
    path[i2] = tmp;
}

// Generate a new path by crossover two paths
void crossover(point* path1, point* path2, point* new_path, int length) {
    int i, r;
    for (i = 0; i < length; i++) {
        r = rand() % 2;
        if (r == 0) {
            new_path[i] = path1[i];
        } else {
            new_path[i] = path2[i];
        }
    }
}

// Find the shortest path between two points using a genetic algorithm
void find_path(point start, point end) {
    const int POPULATION_SIZE = 100;
    const int PATH_LENGTH = 100;
    const int NUM_GENERATIONS = 1000;
    
    int i, j, k;
    
    // Create an array of paths
    point** paths = malloc(POPULATION_SIZE * sizeof(point*));
    for (i = 0; i < POPULATION_SIZE; i++) {
        paths[i] = malloc(PATH_LENGTH * sizeof(point));
    }
    
    // Generate a random population of paths
    for (i = 0; i < POPULATION_SIZE; i++) {
        generate(paths[i], PATH_LENGTH);
    }
    
    // Evaluate each path
    for (i = 0; i < POPULATION_SIZE; i++) {
        paths[i][0] = start;
        paths[i][PATH_LENGTH-1] = end;
        int d = evaluate(paths[i], PATH_LENGTH);
        printf("%d\n", d);
    }
    
    // Perform genetic algorithm
    for (i = 0; i < NUM_GENERATIONS; i++) {
        // Selection
        point** new_paths = malloc(POPULATION_SIZE * sizeof(point*));
        for (j = 0; j < POPULATION_SIZE; j++) {
            new_paths[j] = malloc(PATH_LENGTH * sizeof(point));
        }
        for (j = 0; j < POPULATION_SIZE; j++) {
            int r1 = rand() % POPULATION_SIZE;
            int r2 = rand() % POPULATION_SIZE;
            if (evaluate(paths[r1], PATH_LENGTH) < evaluate(paths[r2], PATH_LENGTH)) {
                for (k = 0; k < PATH_LENGTH; k++) {
                    new_paths[j][k] = paths[r1][k];
                }
            } else {
                for (k = 0; k < PATH_LENGTH; k++) {
                    new_paths[j][k] = paths[r2][k];
                }
            }
        }
        
        // Crossover
        for (j = 0; j < POPULATION_SIZE; j += 2) {
            crossover(new_paths[j], new_paths[j+1], paths[j], PATH_LENGTH);
            crossover(new_paths[j+1], new_paths[j], paths[j+1], PATH_LENGTH);
        }
        
        // Mutation
        for (j = 0; j < POPULATION_SIZE; j++) {
            mutate(paths[j], PATH_LENGTH);
        }
        
        // Evaluation
        for (j = 0; j < POPULATION_SIZE; j++) {
            paths[j][0] = start;
            paths[j][PATH_LENGTH-1] = end;
            int d = evaluate(paths[j], PATH_LENGTH);
            printf("%d\n", d);
        }
    }
    
    // Find the shortest path
    int best_distance = -1;
    int best_path = -1;
    for (i = 0; i < POPULATION_SIZE; i++) {
        int d = evaluate(paths[i], PATH_LENGTH);
        if (best_distance == -1 || d < best_distance) {
            best_distance = d;
            best_path = i;
        }
    }
    
    // Print the shortest path
    printf("Shortest path: ");
    for (i = 0; i < PATH_LENGTH; i++) {
        printf("(%d, %d) ", paths[best_path][i].x, paths[best_path][i].y);
    }
    printf("\n");
}

int main() {
    point start = {0, 0};
    point end = {99, 99};
    find_path(start, end);
    return 0;
}
