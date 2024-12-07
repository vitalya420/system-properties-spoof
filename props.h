#ifndef PROPS_H
#define PROPS_H

#include <stdio.h>
#include "hashmap.h"

extern const char *props[][2];
extern const size_t props_amount;
static HashMap *prop_map;

HashMap *get_properties_map();
char *getprop(const char *key);
void cleanup_props();

#endif // PROPS_H
