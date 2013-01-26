
#include <stdlib.h>
#include "list.h"
#include "util.h"
#include "mem.h"

_ME

static void free_node(Node *node);
static Node *new_node(void *data);

List *new_list() {
	List *list = malloc(sizeof(List));
	

	list->head = NULL;
	list->tail = NULL;
	list->size = 0;

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

void add_list_item(List *list, void *item) {
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

static Node *new_node(void *data) {
	Node *node = malloc(sizeof(Node));

	if(node == NULL) {
		die("Could not allocate memory.");
	}

	node->data = data;
	node->prev = NULL;
	node->next = NULL;

	return node;
}

static void free_node(Node *node) {
	free(node->data);
	free(node);
}