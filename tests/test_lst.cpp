#include <stdio.h>

#include "greatest.h"

#include "list.h"
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

TEST list_append_to_new_list()
{
	linked_list_t * list = linked_list_new();
	int dummy_data = 1337;
	node_t *dummy_node = linked_list_create_node(&dummy_data);
	linked_list_append_node(list, dummy_node);

	ASSERT_EQ(&dummy_data, list->head->data);
	ASSERT_EQ(dummy_node, list->head);
	ASSERT_EQ(true, (list->head == list->tail));
	ASSERT_EQ(NULL, list->head->next);
	ASSERT_EQ(NULL, list->tail->next);

	linked_list_dispose(list);
	PASS();
}

TEST list_append_second_element()
{
	linked_list_t *list = linked_list_new();
	int one = 1;
	int two = 2;

	node_t *node_one = linked_list_create_node(&one);
	node_t *node_two = linked_list_create_node(&two);

	linked_list_append_node(list, node_one);
	linked_list_append_node(list, node_two);

	ASSERT_EQm("Node 1 should be at the list head", node_one, list->head);
	ASSERT_EQm("Node 2 should be at the list tail", node_two, list->tail);
	ASSERT_EQm("list->head->next should point to node 2", node_two, list->head->next);
	ASSERT_EQm("list->tail->next shuold be NULL", NULL, list->tail->next);

	linked_list_dispose(list);

	PASS();
}

TEST list_append_third_element()
{
	linked_list_t *list = linked_list_new();
	int one = 1;
	int two = 2;
	int three = 3;

	node_t *node_one = linked_list_create_node(&one);
	node_t *node_two = linked_list_create_node(&two);
	node_t *node_three = linked_list_create_node(&three);

	linked_list_append_node(list, node_one);
	linked_list_append_node(list, node_two);
	linked_list_append_node(list, node_three);

	ASSERT_EQm("Node 1 should be at the list head", node_one, list->head);
	ASSERT_EQm("Node 3 should be at the list tail", node_three, list->tail);
	ASSERT_EQm("Node 2->next should point to node 3", node_three, node_two->next);
	ASSERT_EQm("list->head->next should point to node 2", node_two, list->head->next);
	ASSERT_EQm("list->tail->next should be NULL", NULL, list->tail->next);

	linked_list_dispose(list);
	PASS();
}

TEST list_remove_node()
{
	linked_list_t *list = linked_list_new();
	int one = 1;
	int two = 2;
	int three = 3;

	node_t *node_one = linked_list_create_node(&one);
	node_t *node_two = linked_list_create_node(&two);
	node_t *node_three = linked_list_create_node(&three);

	linked_list_append_node(list, node_one);
	linked_list_append_node(list, node_two);
	linked_list_append_node(list, node_three);

	linked_list_remove_node(list, node_two);

	ASSERT_EQm("Node 1 should be at the list head", node_one, list->head);
	ASSERT_EQm("Node 3 should be at the list tail", node_three, list->tail);
	ASSERT_EQm("Node 1->next should point to node 3", node_three, node_one->next);
	ASSERT_EQm("list->head->next should point to node 3", node_three, list->head->next);

	linked_list_dispose(list);
	PASS();
}


TEST list_remove_node_at_head()
{
	linked_list_t *list = linked_list_new();
	int one = 1;
	int two = 2;
	int three = 3;

	node_t *node_one = linked_list_create_node(&one);
	node_t *node_two = linked_list_create_node(&two);
	node_t *node_three = linked_list_create_node(&three);

	linked_list_append_node(list, node_one);
	linked_list_append_node(list, node_two);
	linked_list_append_node(list, node_three);

	linked_list_remove_node(list, node_one);

	ASSERT_EQm("Node 2 should be at the list head", node_two, list->head);
	ASSERT_EQm("Node 3 should be at the list tail", node_three, list->tail);
	ASSERT_EQm("list->head->next should point to node 3", node_three, list->head->next);

	linked_list_dispose(list);
	PASS();
}

TEST list_remove_node_at_tail()
{
	linked_list_t *list = linked_list_new();
	int one = 1;
	int two = 2;
	int three = 3;

	node_t *node_one = linked_list_create_node(&one);
	node_t *node_two = linked_list_create_node(&two);
	node_t *node_three = linked_list_create_node(&three);

	linked_list_append_node(list, node_one);
	linked_list_append_node(list, node_two);
	linked_list_append_node(list, node_three);

	linked_list_remove_node(list, node_three);

	ASSERT_EQm("Node 1 should be at the list head", node_one, list->head);
	ASSERT_EQm("Node 2 should be at the list tail", node_two, list->tail);
	ASSERT_EQm("list->tail->next should point NULL", NULL, list->tail->next);

	linked_list_dispose(list);
	PASS();
}

TEST list_remove_last_node_single()
{
	linked_list_t *list = linked_list_new();
	int one = 1;

	node_t *node_one = linked_list_create_node(&one);
	linked_list_append_node(list, node_one);

	/* remove the only node */
	bool removed = linked_list_remove_node(list, node_one);

	ASSERT_EQm("remove should return true", true, removed);
	ASSERT_EQm("list head should be NULL", NULL, list->head);
	ASSERT_EQm("list tail should be NULL", NULL, list->tail);
	ASSERT_EQm("list count should be 0", 0, linked_list_get_count(list));

	linked_list_dispose(list);
	PASS();
}

TEST list_insert_node_after_refnode()
{
	linked_list_t *list = linked_list_new();
	int one = 1;
	int two = 2;
	int three = 3;

	node_t *node_one = linked_list_create_node(&one);
	node_t *node_two = linked_list_create_node(&two);

	linked_list_append_node(list, node_one);
	linked_list_insert_node_after(list, node_two, node_one);

	ASSERT_EQm("Node 1 should be at the list head", node_one, list->head);
	ASSERT_EQm("list->head->next should point to node 2", node_two, list->head->next);
	ASSERT_EQm("list->tail->next should be NULL", NULL, list->tail->next);

	linked_list_dispose(list);
	PASS();
}

TEST list_insert_node_before_refnode()
{
	linked_list_t *list = linked_list_new();
	int one = 1;
	int two = 2;
	int three = 3;

	node_t *node_one = linked_list_create_node(&one);
	node_t *node_two = linked_list_create_node(&two);

	linked_list_append_node(list, node_one);
	linked_list_insert_node_before(list, node_two, node_one);

	ASSERT_EQm("Node 2 should be at the list head", node_two, list->head);
	ASSERT_EQm("list->head->next should point to node 1", node_one, list->head->next);
	ASSERT_EQm("list->tail->next should be NULL", NULL, list->tail->next);

	linked_list_dispose(list);
	PASS();
}

TEST list_remove_from_empty_list()
{
	linked_list_t *list = linked_list_new();
	int val = 10;
	node_t *n = linked_list_create_node(&val);

	bool removed = linked_list_remove_node(list, n);
	ASSERT_EQm("remove on empty list should return false", false, removed);

	/* cleanup the node since it wasn't removed */
	linked_list_dispose_node(n);
	linked_list_dispose(list);
	PASS();
}

TEST list_insert_after_null_ref_appends()
{
	linked_list_t *list = linked_list_new();
	int one = 1;
	int two = 2;

	node_t *node_one = linked_list_create_node(&one);
	node_t *node_two = linked_list_create_node(&two);

	linked_list_append_node(list, node_one);
	/* insert after NULL should behave like append */
	linked_list_insert_node_after(list, node_two, NULL);

	ASSERT_EQm("head should be node_one", node_one, list->head);
	ASSERT_EQm("head->next should be node_two", node_two, list->head->next);
	ASSERT_EQm("tail should be node_two", node_two, list->tail);

	linked_list_dispose(list);
	PASS();
}

TEST list_append_null_node_noop()
{
	linked_list_t *list = linked_list_new();
	/* appending a NULL node should be a no-op and not crash */
	linked_list_append_node(list, NULL);

	ASSERT_EQm("empty list head should be NULL", NULL, list->head);
	ASSERT_EQm("empty list tail should be NULL", NULL, list->tail);

	linked_list_dispose(list);
	PASS();
}

TEST list_prev_node_on_head_returns_null()
{
	linked_list_t *list = linked_list_new();
	int one = 1;
	int two = 2;

	node_t *node_one = linked_list_create_node(&one);
	node_t *node_two = linked_list_create_node(&two);

	linked_list_append_node(list, node_one);
	linked_list_append_node(list, node_two);

	node_t *prev = linked_list_prev_node(list, list->head);
	ASSERT_EQm("prev of head should be NULL", NULL, prev);

	linked_list_dispose(list);
	PASS();
}

TEST list_node_at_out_of_bounds_returns_null()
{
	linked_list_t *list = linked_list_new();
	int one = 1;
	int two = 2;

	node_t *node_one = linked_list_create_node(&one);
	node_t *node_two = linked_list_create_node(&two);

	linked_list_append_node(list, node_one);
	linked_list_append_node(list, node_two);

	node_t *oob = linked_list_node_at(list, 5);
	ASSERT_EQm("node_at out of bounds should be NULL", NULL, oob);

	linked_list_dispose(list);
	PASS();
}

SUITE(linked_list_tests)
{
	// SET_SETUP(before_each, nullptr);
	// SET_TEARDOWN(after_each, nullptr);
	RUN_TEST(list_should_initialize);
	RUN_TEST(list_append_to_new_list);
	RUN_TEST(list_append_third_element);
	RUN_TEST(list_remove_node);
	RUN_TEST(list_remove_node_at_head);
	RUN_TEST(list_remove_node_at_tail);
	RUN_TEST(list_remove_last_node_single);
	RUN_TEST(list_insert_node_after_refnode);
	RUN_TEST(list_insert_node_before_refnode);
	/* Edge cases */
	RUN_TEST(list_remove_from_empty_list);
	RUN_TEST(list_insert_after_null_ref_appends);
	RUN_TEST(list_append_null_node_noop);
	RUN_TEST(list_prev_node_on_head_returns_null);
	RUN_TEST(list_node_at_out_of_bounds_returns_null);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();      /* init & parse command-line args */
    RUN_SUITE(linked_list_tests);
    GREATEST_MAIN_END();        /* display results */
}
