#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <CUnit/Automated.h>
#include "list.h"

void test_list_creation()
{
  list_t *list = list_new(NULL, NULL, NULL);
  CU_ASSERT_TRUE(list_length(list) == 0);
  list_delete(list, true);
}

void test_list_append()
{
  elem_t elem1 = { .i = 1};
  elem_t elem2 = { .i = 2};
  elem_t elem3 = { .i = 3};
  elem_t elem4 = { .i = 4};


  list_t *list = list_new(NULL, NULL, NULL);

  list_append(list, elem1);
  list_append(list, elem2);
  list_append(list, elem3);
  list_append(list, elem4);


  CU_ASSERT_TRUE(list_length(list) == 4);

  list_delete(list, true);
}

void test_list_prepend()
{
  elem_t elem1 = { .i = 1};
  elem_t elem2 = { .i = 2};
  elem_t elem3 = { .i = 3};
  elem_t elem4 = { .i = 4};


  list_t *list = list_new(NULL, NULL, NULL);

  list_prepend(list, elem1);
  list_prepend(list, elem2);
  list_prepend(list, elem3);
  list_prepend(list, elem4);

  CU_ASSERT_TRUE(list_length(list) == 4);

  list_delete(list, true);
}

void test_list_insert()
{
  elem_t elem1 = { .i = 1};
  elem_t elem2 = { .i = 2};
  elem_t elem3 = { .i = 3};
  elem_t elem4 = { .i = 4};


  list_t *list = list_new(NULL, NULL, NULL);

  list_insert(list, 0, elem1);
  list_insert(list, 10, elem2);
  list_insert(list, -5, elem3);
  list_insert(list, 1, elem4);


  CU_ASSERT_TRUE(list_length(list) == 4);

  list_delete(list, true);
}

void test_list_get()
{
  elem_t elem1 = { .i = 1};
  elem_t elem2 = { .i = 2};
  elem_t elem3 = { .i = 3};
  elem_t elem4 = { .i = 4};
  elem_t elemget;

  list_t *list = list_new(NULL, NULL, NULL);

  list_append(list, elem1);
  list_append(list, elem2);
  list_append(list, elem3);
  list_append(list, elem4);

  list_get(list, 0, &elemget);
  CU_ASSERT_TRUE(elemget.i == elem1.i);

  list_get(list, 1, &elemget);
  CU_ASSERT_TRUE(elemget.i == elem2.i);

  list_first(list, &elemget);
  CU_ASSERT_TRUE(elemget.i == elem1.i);

  list_last(list, &elemget);
  CU_ASSERT_TRUE(elemget.i == elem4.i);

  bool test = list_get(list, 0, &elemget);
  CU_ASSERT_TRUE(test);

  test = list_get(list, 10, &elemget);
  CU_ASSERT_TRUE(!test);

  test = list_get(list, -1, &elemget);
  CU_ASSERT_TRUE(!test);

  list_delete(list, true);
}

void test_list_contains()
{
  elem_t elem1 = { .i = 1};
  elem_t elem2 = { .i = 2};
  elem_t elem3 = { .i = 3};
  elem_t elem4 = { .i = 4};
  elem_t elem5 = { .i = 5};

  list_t *list = list_new(NULL, NULL, NULL);

  list_append(list, elem1);
  list_append(list, elem2);
  list_append(list, elem3);
  list_append(list, elem4);

  int i = list_contains(list, elem4);
  CU_ASSERT_TRUE(i == 3);

  i = list_contains(list, elem1);
  CU_ASSERT_TRUE(i == 0);

  i = list_contains(list, elem2);
  CU_ASSERT_TRUE(i == 1);

  i = list_contains(list, elem5);
  CU_ASSERT_TRUE(i == -1);

  list_delete(list, true);
}

void test_list_remove()
{
  elem_t elem1 = { .i = 1};
  elem_t elem2 = { .i = 2};
  elem_t elem3 = { .i = 3};
  elem_t elem4 = { .i = 4};

  list_t *list = list_new(NULL, NULL, NULL);

  list_append(list, elem1);
  list_append(list, elem2);
  list_append(list, elem3);
  list_append(list, elem4);

  list_remove(list, 0, true);
  CU_ASSERT_TRUE(list_length(list) == 3);

  list_remove(list, -1, true);
  CU_ASSERT_TRUE(list_length(list) == 2);

  list_remove(list, 10, true);
  CU_ASSERT_TRUE(list_length(list) == 1);

  list_remove(list, -10, true);
  CU_ASSERT_TRUE(list_length(list) == 0);

  list_delete(list, true);
}

int main(int argc, char *argv[argc])
{
  // Initialise
  CU_initialize_registry();

  // Set up suites and tests
  CU_pSuite creation = CU_add_suite("Test creation and height", NULL, NULL);
  CU_add_test(creation, "Creation", test_list_creation);
  CU_add_test(creation, "Append", test_list_append);
  CU_add_test(creation, "Prepend", test_list_prepend);
  CU_add_test(creation, "Insert", test_list_insert);

  CU_pSuite useability = CU_add_suite("Test useability", NULL, NULL);
  CU_add_test(useability, "List get", test_list_get);
  CU_add_test(useability, "List contains", test_list_contains);
  CU_add_test(useability, "List remove", test_list_remove);

  // Actually run tests
  CU_basic_run_tests();

  // Tear down
  CU_cleanup_registry();
  return CU_get_error();
}
/*
  elem_t elem1 = { .i = 1};
  elem_t elem2 = { .i = 2};
  elem_t elem3 = { .i = 3};
  elem_t elem4 = { .i = 4};
  elem_t elem5 = { .i = 5};
  elem_t elem6 = { .i = 6};
  elem_t elem7 = { .i = 7};
  elem_t elem8 = { .i = 8};
  elem_t elem9 = { .i = 9};
  elem_t elem10 = { .i = 10};
  elem_t elem11 = { .i = 11};
*/
