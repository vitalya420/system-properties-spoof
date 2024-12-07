#ifndef HASHMAP_H
#define HASHMAP_H


typedef struct Node {
    char *key;
    char *value;
    struct Node *next;
} Node;

typedef struct HashMap {
    Node **table;
    int size;
    int count;
} HashMap;


HashMap *create_hashmap(int initial_size);
void hashmap_insert(HashMap *map, const char *key, const char *value);
void resize_hashmap(HashMap *map);
char *hashmap_search(HashMap *map, const char *key);
void free_hashmap(HashMap *map);

#endif // HASHMAP_H
