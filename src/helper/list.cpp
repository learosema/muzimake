#include <stdlib.h>
#include "list.h"

node_t * const linked_list_create_node(const void * const data)
{
	node_t *node = (node_t *)malloc(sizeof(node_t));
	if (!node) return NULL;
	node->data = (void *)data;
	node->next = NULL;
	return node;
}

void linked_list_dispose_node(node_t *node)
{
	if (!node) return;
	free(node);
}

linked_list_t * const linked_list_new()
{
	linked_list_t *list = (linked_list_t *)malloc(sizeof(linked_list_t));
	if (!list) return NULL;
	list->head = NULL;
	list->tail = NULL;
	return list;
}

void linked_list_dispose(linked_list_t *list)
{
	if (!list) return;
	while (list->head != NULL) {
		node_t *disposal = list->head;
		list->head = disposal->next;
		linked_list_dispose_node(disposal);
	}
	list->tail = NULL;
	free(list);
}

void linked_list_append_node(linked_list_t * const list, node_t * const node)
{
	if (!list || !node) return;
	if (list->tail == NULL) {
		list->head = (node_t *)node;
		list->tail = (node_t *)node;
		node->next = NULL;
		return;
	}

	list->tail->next = (node_t *)node;
	list->tail = (node_t *)node;
	node->next = NULL;
}

void linked_list_insert_node_after(linked_list_t * const list, node_t * const new_node, const node_t * const ref_node)
{
	if (!list || !new_node) return;
	if (ref_node == NULL) {
		/* no ref: append to the list */
		linked_list_append_node(list, new_node);
		return;
	}

	for (node_t *iter = list->head; iter != NULL; iter = iter->next)
	{
		if (iter != ref_node)
		{
			continue;
		}

		new_node->next = iter->next;
		iter->next = new_node;
		if (iter == list->tail) {
			list->tail = new_node;
		}
		return;
	}

	/* ref_node not found: append safely */
	linked_list_append_node(list, new_node);
}

bool linked_list_remove_node(linked_list_t * const list, node_t *node)
{
	if (!list || !node) return false;
	if (list->head == NULL) return false;

	if (node == list->head) {
		list->head = list->head->next;
		if (list->head == NULL) {
			/* list is now empty, ensure tail doesn't keep a dangling pointer */
			list->tail = NULL;
		}
		linked_list_dispose_node(node);
		return true;
	}

	for (node_t *iter = list->head; iter != NULL; iter = iter->next)
	{
		if (iter->next != node) {
			continue;
		}
		iter->next = node->next;
		if (node == list->tail) {
			list->tail = iter;
			iter->next = NULL;
		}
		linked_list_dispose_node(node);
		return true;
	}
	return false;
}

node_t * linked_list_find_node(const linked_list_t * const list, const void * const data)
{
	if (!list) return NULL;
	for (node_t *iter = list->head; iter != NULL; iter = iter->next)
	{
		if (iter->data == data) {
			return iter;
		}
	}
	return NULL;
}

node_t *linked_list_node_at(const linked_list_t * const list, const int node_index)
{
	if (!list || node_index < 0) return NULL;
	int current_index = 0;
	for (node_t *iter = list->head; iter != NULL; iter = iter->next)
	{
		if (current_index == node_index) {
			return iter;
		}
		current_index++;
	}
	return NULL;
}

node_t *linked_list_prev_node(const linked_list_t * const list, const node_t * const ref_node)
{
	if (!list || !ref_node) return NULL;
	if (ref_node == list->head)
	{
		return NULL;
	}
	for (node_t *iter = list->head; iter != NULL; iter = iter->next)
	{
		if (iter->next == ref_node) {
			return iter;
		}
	}
	return NULL;
}

void linked_list_unshift_node(linked_list_t * const list, node_t * const new_node)
{
	if (!list || !new_node) return;
	if (list->tail == NULL)
	{
		list->head = new_node;
		list->tail = new_node;
		new_node->next = NULL;
		return;
	}

	node_t *old_head = list->head;
	list->head = new_node;
	list->head->next = old_head;
}

void linked_list_insert_node_before(linked_list_t * const list, node_t *new_node, node_t *ref_node)
{
	if (!list || !new_node) return;
	if (ref_node == list->head) {
		linked_list_unshift_node(list, new_node);
		return;
	}
	node_t *prev = linked_list_prev_node(list, ref_node);
	linked_list_insert_node_after(list, new_node, prev);
}

void linked_list_append(linked_list_t * const list, const void * const data)
{
	if (!list) return;
	node_t *n = linked_list_create_node(data);
	if (!n) return;
	linked_list_append_node(list, n);
}

void linked_list_insert_after(linked_list_t * const list, const void * const data, const int node_index)
{
	if (!list) return;
	node_t *ref_node = linked_list_node_at(list, node_index);
	node_t *new_node = linked_list_create_node(data);
	if (!new_node) return;
	if ((ref_node == NULL) || (node_index < 0))
	{
		linked_list_append_node(list, new_node);
		return;
	}
	linked_list_insert_node_after(list, new_node, ref_node);
}

int linked_list_get_count(const linked_list_t * const list)
{
	if (!list) return 0;
	int count = 0;
	for (node_t *iter = list->head; iter != NULL; iter = iter->next)
	{
		count++;
	}
	return count;
}
