#ifndef __LIST_H__
#define __LIST_H__

#include <stdint.h>

#define LIST_FOR_EACH(ptr, list) for(ptr = list->head; ptr != NULL; ptr = ptr->next)

typedef struct node_struct {
	struct node_struct *next;
	struct node_struct *prev;
	void *data;
} Node;

typedef struct list_struct {
	uint32_t size;
	Node *head;
	Node *tail;
} List;

List *new_list();
void free_list(List *list);
void add_item(List *list, void *item);

#endif // !__LIST_H__
