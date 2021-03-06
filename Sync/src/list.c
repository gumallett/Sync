
#include <stdlib.h>
#include "include/list.h"
#include "include/util.h"
#include "include/mem.h"

/** 
	Allocates/frees a node for use in Linked List 
**/
static void free_node(Node *node);
static Node *new_node(void *data);

List *new_list() {
	List *list = malloc(sizeof(List));
	MEM_CHECK(list);

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

void list_append(List *list, void *item) {
	Node *node = new_node(item);
	MEM_CHECK(node);

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

void list_insert(List *list, void *item) {
	Node *node = new_node(item);
	MEM_CHECK(node);

	if(list->head == NULL) {
		list->head = node;
		list->tail = node;
	}
	else {
		node->next = list->head;
		node->prev = NULL;
		list->head = node;
	}

	list->size++;
}

void *pop(List *list) {
	Node *ret = list->head;
	void *data = ret->data;

	free_node(ret);
	list->head = list->head->next;

	if(list->head == NULL) {
		list->tail = NULL;
	}

	list->size--;
	return data;
}

static Node *new_node(void *data) {
	Node *node = malloc(sizeof(Node));
	MEM_CHECK(node);

	node->data = data;
	node->prev = NULL;
	node->next = NULL;

	return node;
}

static void free_node(Node *node) {
	free(node->data);
	free(node);
}