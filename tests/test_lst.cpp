#include <stdio.h>
#include <greatest.h>

#include "helper/list.h"
/*
static void before_each(void *data)
{

}

static void after_each(void *data)
{

}*/

TEST list_should_initialize()
{
	linked_list_t * list = linked_list_new();

	ASSERT_EQ(NULL, list->head);
	ASSERT_EQ(NULL, list->tail);

	linked_list_dispose(list);
	PASS();
}

TEST list_append_to_new_list_should_work()
{
	linked_list_t * list = linked_list_new();
	int dummy_data = 1337;
	node_t *dummy_node = linked_list_create_node(&dummy_data);
	linked_list_append(list, dummy_node);

	ASSERT_EQ(&dummy_data, list->head->data);
	ASSERT_EQ(dummy_node, list->head);
	ASSERT_EQ(true, (list->head == list->tail));
	ASSERT_EQ(NULL, list->head->next);
	ASSERT_EQ(NULL, list->tail->next);

	linked_list_dispose(list);
	PASS();
}

TEST list_append_second_element_should_work()
{
	linked_list_t *list = linked_list_new();
	int one = 1;
	int two = 2;

	node_t *node_one = linked_list_create_node(&one);
	node_t *node_two = linked_list_create_node(&two);

	linked_list_append(list, node_one);
	linked_list_append(list, node_two);

	ASSERT_EQm("Node 1 should be at the list head", node_one, list->head);
	ASSERT_EQm("Node 2 should be at the list tail", node_two, list->tail);
	ASSERT_EQm("list.head->next should point to node 2", node_two, list->head->next);
	ASSERT_EQm("list.tail->next shuold be NULL", NULL, list->tail->next);

	linked_list_dispose(list);

	PASS();
}

TEST list_append_third_element_should_work()
{
	linked_list_t *list = linked_list_new();
	int one = 1;
	int two = 2;
	int three = 3;

	node_t *node_one = linked_list_create_node(&one);
	node_t *node_two = linked_list_create_node(&two);
	node_t *node_three = linked_list_create_node(&three);

	linked_list_append(list, node_one);
	linked_list_append(list, node_two);
	linked_list_append(list, node_three);

	ASSERT_EQm("Node 1 should be at the list head", node_one, list->head);
	ASSERT_EQm("Node 3 should be at the list tail", node_three, list->tail);
	ASSERT_EQm("Node 2->next should point to node 3", node_three, node_two->next);
	ASSERT_EQm("list->head->next should point to node 2", node_two, list->head->next);
	ASSERT_EQm("list->tail->next should be NULL", NULL, list->tail->next);

	linked_list_dispose(list);
	PASS();
}



SUITE(linked_list_tests)
{
	// SET_SETUP(before_each, nullptr);
	// SET_TEARDOWN(after_each, nullptr);

	RUN_TEST(list_should_initialize);
	RUN_TEST(list_append_to_new_list_should_work);
	RUN_TEST(list_append_third_element_should_work);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();      /* init & parse command-line args */
    RUN_SUITE(linked_list_tests);
    GREATEST_MAIN_END();        /* display results */
}
