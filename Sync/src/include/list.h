/**
	Declarations for a doubly linked list data structure
**/
#ifndef __LIST_H__
#define __LIST_H__

/**
	struct representing a node in a linked list.
**/
typedef struct list_node_struct {
	struct list_node_struct *next;
	struct list_node_struct *prev;
	void *data;
} Node;

/**
	struct for the list
**/
typedef struct list_struct {
	unsigned int size;
	Node *head;
	Node *tail;
} List;

// iterator which traverses the list, starting at the list head. 
#define LIST_FOR_EACH(ptr, list) for(ptr = list->head; ptr != NULL; ptr = ptr->next)

/**
	Creates and allocates space for a new Listed List.
**/
List *new_list();

/**
	Frees the list, via passed in pointer
**/
void free_list(List *list);

/**
	Adds an item to the end of the list
**/
void list_append(List *list, void *item);

/**
	Adds an item to the beginning of the list
**/
void list_insert(List *list, void *item);

/**
	Removes and returns a pointer to the first element in the list
**/
void *pop(List *list);

#endif // !__LIST_H__
