#include "harness/unity.h"
#include "../src/lab.h"

static list_t *lst_ = NULL;

static int *alloc_data(int i)
{
  int *rval = (int *)malloc(sizeof(int));
  *rval = i;
  return rval;
}

static void destroy_data(void *data)
{
  free(data);
}

static int compare_to(const void *a, const void *b)
{
  int fst = *(int *)a;
  int snd = *(int *)b;
  return fst - snd;
}

static void populate_list(void)
{
  for (int i = 0; i < 5; i++)
  {
    list_add(lst_, alloc_data(i));
  }
}

void setUp(void)
{
  lst_ = list_init(destroy_data, compare_to);
}

void tearDown(void)
{
  list_destroy(&lst_);
}

void test_create_destroy(void)
{
  list_t *lst = NULL;
  lst = list_init(destroy_data, compare_to);
  TEST_ASSERT_FALSE(lst == NULL);
  TEST_ASSERT_FALSE(lst->head == NULL);
  TEST_ASSERT_TRUE(lst->size == 0);
  TEST_ASSERT_TRUE(lst->head->data == NULL);
  // Make sure the function pointers are pointing to the correct fuctions
  TEST_ASSERT_TRUE(lst->destroy_data == destroy_data);
  TEST_ASSERT_TRUE(lst->compare_to == compare_to);
  // Make sure we are a circular linked list
  TEST_ASSERT_FALSE(lst->head->next == NULL);
  TEST_ASSERT_FALSE(lst->head->prev == NULL);
  TEST_ASSERT_TRUE(lst->head->next == lst->head->prev);
  list_destroy(&lst);
  TEST_ASSERT_TRUE(lst == NULL);
}

void test_add1(void)
{
  list_add(lst_, alloc_data(1));
  TEST_ASSERT_TRUE(lst_->size == 1);
  // With one node both next and prev should be equal
  TEST_ASSERT_TRUE(lst_->head->next == lst_->head->prev);
  // Make sure we didn't clobber our sentinel node
  TEST_ASSERT_FALSE(lst_->head == lst_->head->next);
  TEST_ASSERT_FALSE(lst_->head == lst_->head->prev);
  TEST_ASSERT_TRUE(lst_->head->data == NULL);
  // Check to make sure our data actually made it into the node
  TEST_ASSERT_TRUE(*((int *)lst_->head->next->data) == 1);
  TEST_ASSERT_TRUE(*((int *)lst_->head->prev->data) == 1);
}

void test_add2(void)
{
  list_add(lst_, alloc_data(1));
  TEST_ASSERT_TRUE(lst_->size == 1);
  list_add(lst_, alloc_data(2));
  TEST_ASSERT_TRUE(lst_->size == 2);
  // With two nodes both next and prev should NOT be equal
  TEST_ASSERT_FALSE(lst_->head->next == lst_->head->prev);
  // Make sure we didn't clobber our sentinel node
  TEST_ASSERT_FALSE(lst_->head == lst_->head->next);
  TEST_ASSERT_FALSE(lst_->head == lst_->head->prev);
  TEST_ASSERT_TRUE(lst_->head->data == NULL);
  // Check to make sure our next and prev have the correct data
  TEST_ASSERT_TRUE(*((int *)lst_->head->next->data) == 2);
  TEST_ASSERT_TRUE(*((int *)lst_->head->prev->data) == 1);
}

void test_removeIndex0(void)
{
  populate_list();
  int *rval = (int *)list_remove_index(lst_, 0);
  TEST_ASSERT_TRUE(lst_->size == 4);
  TEST_ASSERT_TRUE(*rval == 4);
  free(rval);
  node_t *curr = lst_->head->next;
  // List should be 3->2->1->0
  for (int i = 3; i >= 0; i--)
  {
    TEST_ASSERT_TRUE(*((int *)curr->data) == i);
    curr = curr->next;
  }
  curr = lst_->head->prev;
  for (int i = 0; i <= 3; i++)
  {
    TEST_ASSERT_TRUE(*((int *)curr->data) == i);
    curr = curr->prev;
  }
}

void test_removeIndex3(void)
{
  populate_list();
  int *rval = (int *)list_remove_index(lst_, 3);
  TEST_ASSERT_TRUE(lst_->size == 4);
  TEST_ASSERT_TRUE(*rval == 1);
  free(rval);
  node_t *curr = lst_->head->next;
  // List should be 4->3->2->0
  for (int i = 3; i >= 1; i--)
  {
    TEST_ASSERT_TRUE(*((int *)curr->data) == i + 1);
    curr = curr->next;
  }
  // Check the last one
  TEST_ASSERT_TRUE(*((int *)curr->data) == 0);
  // Set the curr back one node so we can check prev links
  curr = curr->prev;
  for (int i = 1; i <= 3; i++)
  {
    TEST_ASSERT_TRUE(*((int *)curr->data) == i + 1);
    curr = curr->prev;
  }
}

void test_removeIndex4(void)
{
  populate_list();
  int *rval = (int *)list_remove_index(lst_, 4);
  TEST_ASSERT_TRUE(lst_->size == 4);
  TEST_ASSERT_TRUE(*rval == 0);
  free(rval);
  node_t *curr = lst_->head->next;
  // List should be 4->3->2->1
  for (int i = 3; i >= 0; i--)
  {
    TEST_ASSERT_TRUE(*((int *)curr->data) == i + 1);
    curr = curr->next;
  }
  curr = lst_->head->prev;
  for (int i = 0; i <= 3; i++)
  {
    TEST_ASSERT_TRUE(*((int *)curr->data) == i + 1);
    curr = curr->prev;
  }
}

void test_invaidIndex(void)
{
  populate_list();
  void *rval = list_remove_index(lst_, 666);
  TEST_ASSERT_TRUE(rval == NULL);
  TEST_ASSERT_TRUE(lst_->size == 5);
  node_t *curr = lst_->head->next;
  // List should be 4->3->2->1->0
  for (int i = 4; i >= 0; i--)
  {
    TEST_ASSERT_TRUE(*((int *)curr->data) == i);
    curr = curr->next;
  }
  for (int i = 0; i >= 4; i++)
  {
    TEST_ASSERT_TRUE(*((int *)curr->data) == i);
    curr = curr->prev;
  }
}

void test_removeAll(void)
{
  populate_list();
  // List should be 4->3->2->1->0
  for (int i = 4; i >= 0; i--)
  {
    int *rval = (int *)list_remove_index(lst_, 0);
    TEST_ASSERT_TRUE(*rval == i);
    free(rval);
  }
  // Make sure we back to default
  TEST_ASSERT_FALSE(lst_->head->next == NULL);
  TEST_ASSERT_FALSE(lst_->head->prev == NULL);
  TEST_ASSERT_TRUE(lst_->head->next == lst_->head->prev);
  TEST_ASSERT_TRUE(lst_->size == 0);
}

void test_indexOf0(void)
{
  populate_list();
  // List should be 4->3->2->1->0
  void *data = lst_->head->next->data;
  size_t idx = list_indexof(lst_, data);
  TEST_ASSERT_TRUE(idx == 0);
}

void test_indexOf3(void)
{
  populate_list();
  // List should be 4->3->2->1->0
  void *data = alloc_data(1);
  size_t idx = list_indexof(lst_, data);
  TEST_ASSERT_TRUE(idx == 3);
  free(data);
}

void test_notInList(void)
{
  populate_list();
  void *data = alloc_data(22);
  int idx = list_indexof(lst_, data);
  TEST_ASSERT_EQUAL_INT64(-1, idx);
  free(data);
}

void test_remove_last_element(void)
{
  list_t *lst = list_init(destroy_data, compare_to);
  for (int i = 0; i < 5; i++) {
    list_add(lst, alloc_data(i));
  }
  int *rval = (int *)list_remove_index(lst, 4);
  TEST_ASSERT_TRUE(lst->size == 4);
  TEST_ASSERT_TRUE(*rval == 0);
  free(rval);
  node_t *curr = lst->head->next;
  for (int i = 4; i > 0; i--) {
    TEST_ASSERT_TRUE(*((int *)curr->data) == i);
    curr = curr->next;
  }
  list_destroy(&lst);
}

void test_mixed_operations(void)
{
  list_t *lst = list_init(destroy_data, compare_to);
  
  // Add elements 0, 1, 2
  for (int i = 0; i < 3; i++) {
    list_add(lst, alloc_data(i));
  }
  TEST_ASSERT_TRUE(lst->size == 3);

  // Remove the first element (2)
  int *rval = (int *)list_remove_index(lst, 0);
  TEST_ASSERT_TRUE(*rval == 2);
  free(rval);
  TEST_ASSERT_TRUE(lst->size == 2);

  // Add elements 3, 4
  for (int i = 3; i < 5; i++) {
    list_add(lst, alloc_data(i));
  }
  TEST_ASSERT_TRUE(lst->size == 4);

  // Verify the final state of the list: 4->3->1->0
  int expected[] = {4, 3, 1, 0};
  node_t *curr = lst->head->next;
  for (int i = 0; i < 4; i++) {
    TEST_ASSERT_TRUE(*((int *)curr->data) == expected[i]);
    curr = curr->next;
  }

  list_destroy(&lst);
  TEST_ASSERT_TRUE(lst == NULL);
}

void test_add_remove_middle(void)
{
  populate_list();
  
  // Add element 99 at index 2
  node_t *new_node = (node_t *)malloc(sizeof(node_t));
  new_node->data = alloc_data(99);
  node_t *current = lst_->head->next;
  for (int i = 0; i < 2; i++) {
    current = current->next;
  }
  new_node->next = current;
  new_node->prev = current->prev;
  current->prev->next = new_node;
  current->prev = new_node;
  lst_->size++;

  // Verify the state of the list: 4->3->99->2->1->0
  int expected[] = {4, 3, 99, 2, 1, 0};
  node_t *curr = lst_->head->next;
  for (int i = 0; i < 6; i++) {
    TEST_ASSERT_TRUE(*((int *)curr->data) == expected[i]);
    curr = curr->next;
  }

  // Remove element at index 2 (99)
  int *rval = (int *)list_remove_index(lst_, 2);
  TEST_ASSERT_TRUE(lst_->size == 5);
  TEST_ASSERT_TRUE(*rval == 99);
  free(rval);

  // Verify the state of the list: 4->3->2->1->0
  int final_expected[] = {4, 3, 2, 1, 0};
  curr = lst_->head->next;
  for (int i = 0; i < 5; i++) {
    TEST_ASSERT_TRUE(*((int *)curr->data) == final_expected[i]);
    curr = curr->next;
  }
}

void test_operations_on_empty_list(void)
{
  list_t *lst = list_init(destroy_data, compare_to);

  // Attempt to remove an element from an empty list
  int *rval = (int *)list_remove_index(lst, 0);
  TEST_ASSERT_TRUE(rval == NULL);
  TEST_ASSERT_TRUE(lst->size == 0);

  // Attempt to find an element in an empty list
  int *data = alloc_data(1);
  int index = list_indexof(lst, data);
  TEST_ASSERT_TRUE(index == -1);
  free(data);

  list_destroy(&lst);
  TEST_ASSERT_TRUE(lst == NULL);
}

int main(void)
{
  UNITY_BEGIN();
  RUN_TEST(test_create_destroy);
  RUN_TEST(test_add1);
  RUN_TEST(test_add2);
  RUN_TEST(test_removeIndex0);
  RUN_TEST(test_removeIndex3);
  RUN_TEST(test_removeIndex4);
  RUN_TEST(test_invaidIndex);
  RUN_TEST(test_removeAll);
  RUN_TEST(test_indexOf0);
  RUN_TEST(test_indexOf3);
  RUN_TEST(test_notInList);

  RUN_TEST(test_remove_last_element);
  RUN_TEST(test_mixed_operations);
  RUN_TEST(test_add_remove_middle);
  RUN_TEST(test_operations_on_empty_list); 
  return UNITY_END();
}