#include <stdlib.h>

typedef struct DisjointSets{
    int *parent;
    int *rank;
    int size;
} DisjointSets;

DisjointSets *createDisjointSets(int size) {
    DisjointSets *sets = (DisjointSets *)malloc(sizeof(DisjointSets));
    sets->size = size;
    sets->parent = (int *)malloc(size * sizeof(int));
    sets->rank = (int *)malloc(size * sizeof(int));

    for (int i = 0; i < size; ++i) {
        sets->parent[i] = i;
        sets->rank[i] = 0;
    }

    return sets;
}

int find(DisjointSets *sets, int element) {
    if (sets->parent[element] != element) {
        sets->parent[element] = find(sets, sets->parent[element]);
    }
    return sets->parent[element];
}

void unionSets(DisjointSets *sets, int element1, int element2) {
    int root1 = find(sets, element1);
    int root2 = find(sets, element2);

    if (root1 != root2) {
        if (sets->rank[root1] < sets->rank[root2]) {
            sets->parent[root1] = root2;
        } else if (sets->rank[root1] > sets->rank[root2]) {
            sets->parent[root2] = root1;
        } else {
            sets->parent[root2] = root1;
            sets->rank[root1]++;
        }
    }
}

void destroyDisjointSets(DisjointSets *sets) {
    free(sets->parent);
    free(sets->rank);
    free(sets);
}

