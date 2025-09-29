#include <stdlib.h>
#include <list.h>

node_t *linked_list_create_node(void *data)
{
	node_t *node = (node_t *)malloc(sizeof(node_t));
	node->data = data;
	node->next = nullptr;
	return node;
}

void linked_list_dispose_node(node_t *node)
{
	free(node);
}

linked_list_t *linked_list_new()
{
	linked_list_t *list = (linked_list_t *)malloc(sizeof(linked_list_t));
	list->head = NULL;
	list->tail = NULL;
	return list;
}

void linked_list_dispose(linked_list_t *list)
{
	while (list->head != NULL) {
		node_t *disposal = list->head;
		list->head = disposal->next;
		linked_list_dispose_node(disposal);
	}
	free(list);
}

void linked_list_append(linked_list_t *list, node_t *node)
{
	if (list->tail == nullptr) {
		list->head = node;
		list->tail = node;
		node->next = nullptr;
		return;
	}

	node_t *old_tail = list->tail;
	list->tail->next = node;
	list->tail = node;
	node->next = nullptr;
}

void linked_list_insert_after(linked_list_t *list, node_t *new_node, node_t *ref_node)
{
	for (node_t *iter = list->head; iter != NULL; iter = iter->next)
	{
		if (iter == ref_node)
		{
			new_node->next = iter->next;
			iter->next = new_node;
			return;
		}
	}
	list->tail->next = new_node;
	list->tail = new_node;
	new_node->next = NULL;
}

bool linked_list_remove(linked_list_t *list, node_t *node)
{
	for (node_t *iter = list->head; iter != NULL; iter = iter->next)
	{
		if (iter->next == node) {
			iter->next = node->next;
			linked_list_dispose_node(node);
			return true;
		}
	}
	return false;
}

node_t * linked_list_find_node(linked_list_t *list, void *data)
{
	for (node_t *iter = list->head; iter != NULL; iter = iter->next)
	{
		if (iter->data == data) {
			return iter;
		}
	}
	return NULL;
}
