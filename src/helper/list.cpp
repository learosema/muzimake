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

void linked_list_append_node(linked_list_t *list, node_t *node)
{
	if (list->tail == NULL) {
		list->head = node;
		list->tail = node;
		node->next = NULL;
		return;
	}

	node_t *old_tail = list->tail;
	list->tail->next = node;
	list->tail = node;
	node->next = NULL;
}

void linked_list_insert_node_after(linked_list_t *list, node_t *new_node, node_t *ref_node)
{
	for (node_t *iter = list->head; iter != NULL; iter = iter->next)
	{
		if (iter != ref_node)
		{
			continue;
		}

		if (iter == list->tail) {
			list->tail = new_node;
		}

		new_node->next = iter->next;
		iter->next = new_node;
		return;
	}
	list->tail->next = new_node;
	list->tail = new_node;
	new_node->next = NULL;
}

bool linked_list_remove_node(linked_list_t *list, node_t *node)
{
	if (node == list->head) {
		list->head = list->head->next;
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

node_t *linked_list_node_at(linked_list_t * list, int node_index)
{
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

node_t *linked_list_prev_node(linked_list_t *list, node_t *ref_node)
{
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

void linked_list_unshift_node(linked_list_t *list, node_t *new_node)
{
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

void linked_list_insert_node_before(linked_list_t *list, node_t *new_node, node_t *ref_node)
{
	if (ref_node == list->head) {
		linked_list_unshift_node(list, new_node);
		return;
	}
	node_t *prev = linked_list_prev_node(list, ref_node);
	linked_list_insert_node_after(list, new_node, prev);
}

void linked_list_append(linked_list_t *list, void *data)
{
	linked_list_append_node(list, linked_list_create_node(data));
}

void linked_list_insert_after(linked_list_t *list, void *data, int node_index)
{
	node_t *ref_node = linked_list_node_at(list, node_index);
	node_t *new_node = linked_list_create_node(data);
	if ((ref_node == NULL) || (node_index < 0))
	{
		linked_list_append_node(list, new_node);
		return;
	}
	linked_list_insert_node_after(list, new_node, ref_node);
}
