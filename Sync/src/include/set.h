#ifndef __SET_H__
#define __SET_H__

#include <tchar.h>
#include <stdint.h>
#include "list.h"
#include "util.h"

#define DEFAULT_CAPACITY 2048

typedef struct entry_struct {
	uint64_t hash;
	List *data;
} Entry;

typedef struct set_struct {
	uint32_t size;
	uint32_t capacity;
	Entry **entries;
	List *iterator;
} Set;

typedef int16_t (*compare_func) (const void *item1, const void *item2);

Set *new_set(uint32_t capacity);

void free_set(Set *set);

void set_add(Set *set, uint64_t hash, void *item);

Set *set_diff(Set *set1, Set *set2, 
			  uint64_t (*hash_func) (const void *item), 
			  compare_func);

bool set_contains(Set *set, 
					  uint64_t hash, 
					  void *item, 
					  compare_func);

#endif