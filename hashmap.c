#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"


unsigned int hash(const char *key, int size) {
    unsigned int hash = 0;
    while (*key) {
        hash = (hash << 5) + *key++;
    }
    return hash % size;
}

HashMap *create_hashmap(int initial_size) {
    HashMap *map = malloc(sizeof(HashMap));
    map->size = initial_size;
    map->count = 0;
    map->table = malloc(sizeof(Node *) * initial_size);
    for (int i = 0; i < initial_size; i++) {
        map->table[i] = NULL;
    }
    return map;
}

void resize_hashmap(HashMap *map) {
    int new_size = map->size * 2;
    Node **new_table = malloc(sizeof(Node *) * new_size);
    for (int i = 0; i < new_size; i++) {
        new_table[i] = NULL;
    }

    for (int i = 0; i < map->size; i++) {
        Node *current = map->table[i];
        while (current != NULL) {
            Node *next = current->next;
            unsigned int new_index = hash(current->key, new_size);
            current->next = new_table[new_index];
            new_table[new_index] = current;
            current = next;
        }
    }

    free(map->table);
    map->table = new_table;
    map->size = new_size;
}

void hashmap_insert(HashMap *map, const char *key, const char *value) {
    if ((float)(map->count + 1) / map->size > 0.7) {
        resize_hashmap(map);
    }

    unsigned int index = hash(key, map->size);
    Node *new_node = malloc(sizeof(Node));
    new_node->key = strdup(key);
    new_node->value = strdup(value);
    new_node->next = map->table[index];
    map->table[index] = new_node;
    map->count++;
}

char *hashmap_search(HashMap *map, const char *key) {
    unsigned int index = hash(key, map->size);
    Node *current = map->table[index];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return current->value;
        }
        current = current->next;
    }
    return NULL;
}


void free_hashmap(HashMap *map) {
    for (int i = 0; i < map->size; i++) {
        Node *current = map->table[i];
        while (current != NULL) {
            Node *temp = current;
            current = current->next;
            free(temp->key);
            free(temp->value);
            free(temp);
        }
    }
    free(map->table);
    free(map);
}