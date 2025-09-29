#ifndef __LIST_H__
#define __LIST_H__

typedef struct node_s node_t;
typedef struct node_s
{
	void *data;
	node_t *next;
} node_t;

typedef struct linked_list_s {
	node_t *head;
	node_t *tail;
} linked_list_t;

node_t *linked_list_create_node(void *data = NULL);
node_t *linked_list_find_node(linked_list_t *list, void *data);
void linked_list_dispose_node(node_t *node);

linked_list_t *linked_list_new();
void linked_list_dispose(linked_list_t *list);
void linked_list_append(linked_list_t *list, node_t *node);
void linked_list_insert_after(linked_list_t *list, node_t *new_node, node_t *ref_node);
bool linked_list_remove(linked_list_t *list, node_t *node);

#endif
