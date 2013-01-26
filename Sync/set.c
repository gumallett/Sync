#include "set.h"
#include "list.h"
#include <stdlib.h>
#include <stdio.h>

static void free_entry(Entry *e) {
	free_list(e->data);
	free(e);
}

static Entry *new_entry(uint64_t hash, void *item) {
	Entry *e = malloc(sizeof(Entry));

	if(e == NULL) {
		die("Could not allocate memory.");
	}

	e->hash = hash;
	e->data = new_list();
	list_append(e->data, item);
	return e;
}

Set *new_set(uint32_t capacity) {
	Set *set = malloc(sizeof(Set));

	if(set == NULL) {
		die("Could not allocate memory.");
	}

	set->size = 0;
	set->capacity = capacity;
	set->entries = calloc(capacity, sizeof(Entry*));
	set->iterator = new_list();

	return set;
}

void free_set(Set *set) {
	uint32_t i;

	for(i=0; i<set->capacity; i++) {
		if(set->entries[i] != NULL) {
			free_entry(set->entries[i]);
		}
	}

	free_list(set->iterator);
	free(set);
}

void set_add(Set *set, uint64_t hash, void *item) {
	uint32_t idx = hash % set->capacity;
	Entry *e;

	//wprintf(L"item: %s\tidx: %d\n", item, idx);

	if(idx >= set->capacity) {
		//resize?
		printf("Set full.");
		return;
	}

	e = set->entries[idx];

	if(e == NULL) {
		e = new_entry(hash, item);
		set->entries[idx] = e;
	}
	else {		
		list_append(e->data, item);
	}

	list_append(set->iterator, item);
	set->size++;
}

bool set_contains(Set *set, uint64_t hash, void *item, compare_func compare) {
	uint32_t idx = hash % set->capacity;
	Entry *e = set->entries[idx];
	Node *ptr;

	if(e != NULL) {
		for(ptr = e->data->head; ptr != NULL; ptr = ptr->next) {
			if(compare(ptr->data, item) == 0) {				
				return 1;
			}
		}
	}

	return 0;
}

Set *set_diff(Set *set1, Set *set2, uint64_t (*hash_func) (const void *item), compare_func compare) {
	Set *diff = new_set(DEFAULT_CAPACITY);
	Node *ptr;
	uint64_t hash;

	for(ptr = set1->iterator->head; ptr != NULL; ptr = ptr->next) {
		hash = hash_func(ptr->data);

		if(!set_contains(set2, hash, ptr->data, compare)) {
			set_add(diff, hash, ptr->data);
		}
	}

	return diff;
}