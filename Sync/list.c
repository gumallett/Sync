#include "list.h"
#include "util.h"
#include <stdlib.h>

static void free_node(Node *node) {
	free(node->data);
	free(node);
}

static Node *new_node(void *data) {
	Node *node = calloc(1, sizeof(Node));
	node->data = data;

	return node;
}

List *new_list() {
	List *list = calloc(1, sizeof(List));

	if(list == NULL) {
		die("Could not allocate memory.");
	}

	return list;
}

void free_list(List *list) {
	Node *cur = list->head;
	Node *next;

	while(cur != NULL) {
		next = cur->next;
		free_node(cur);
		cur = next;
	}
}

void add_item(List *list, void *item) {
	Node *node = new_node(item);

	if(list->head == NULL) {
		list->head = node;
		list->tail = node;
	}
	else {	
		list->tail->next = node;
		node->prev = list->tail;
		list->tail = node;
	}

	list->size++;
}