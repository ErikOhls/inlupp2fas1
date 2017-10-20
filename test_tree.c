#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <CUnit/Automated.h>
#include <stdlib.h>
#include "tree.h"

void test_tree_creation()
{
  tree_t *tree = tree_new(NULL, NULL, NULL, NULL);
  CU_ASSERT_TRUE(tree_size(tree) == 0);
  tree_delete(tree, true, true);
}

void test_tree_insert()
{
  elem_t elem1 = { .i = 1};
  elem_t elem2 = { .i = 2};
  elem_t elem3 = { .i = 3};
  elem_t elem4 = { .i = 4};
  elem_t elem5 = { .i = 5};
  elem_t elem6 = { .i = 6};
  elem_t elem7 = { .i = 7};

  tree_key_t key1 = { .i = 1};
  tree_key_t key2 = { .i = 2};
  tree_key_t key3 = { .i = 3};
  tree_key_t key4 = { .i = 4};
  tree_key_t key5 = { .i = 5};
  tree_key_t key6 = { .i = 6};
  tree_key_t key7 = { .i = 7};

  tree_t *tree = tree_new(NULL, NULL, NULL, NULL);

  tree_insert(tree, key1, elem1);
  CU_ASSERT_TRUE(tree_size(tree) == 1);

  tree_insert(tree, key2, elem2);
  tree_insert(tree, key3, elem3);
  tree_insert(tree, key4, elem4);
  tree_insert(tree, key5, elem5);
  tree_insert(tree, key6, elem6);
  tree_insert(tree, key7, elem7);
  CU_ASSERT_TRUE(tree_size(tree) == 7);

  tree_delete(tree, true, true);
}

void test_tree_has_key()
{
  elem_t elem1 = { .i = 1};
  elem_t elem2 = { .i = 2};
  elem_t elem3 = { .i = 3};
  elem_t elem4 = { .i = 4};

  tree_key_t key1 = { .i = 1};
  tree_key_t key2 = { .i = 2};
  tree_key_t key3 = { .i = 3};
  tree_key_t key4 = { .i = 4};
  tree_key_t key5 = { .i = 5};
  tree_t *tree = tree_new(NULL, NULL, NULL, NULL);

  tree_insert(tree, key1, elem1);
  tree_insert(tree, key2, elem2);
  tree_insert(tree, key3, elem3);
  tree_insert(tree, key4, elem4);

  bool test = tree_has_key(tree, key1);
  CU_ASSERT_TRUE(test);

  test = tree_has_key(tree, key4);
  CU_ASSERT_TRUE(test);

  test = tree_has_key(tree, key5);
  CU_ASSERT_TRUE(!test);

  tree_delete(tree, true, true);
}

void test_tree_get()
{
  elem_t elem1 = { .i = 1};
  elem_t elem2 = { .i = 2};
  elem_t elem3 = { .i = 3};
  elem_t elem4 = { .i = 4};
  elem_t elemget;

  tree_key_t key1 = { .i = 1};
  tree_key_t key2 = { .i = 2};
  tree_key_t key3 = { .i = 3};
  tree_key_t key4 = { .i = 4};
  tree_key_t key5 = { .i = 5};

  tree_t *tree = tree_new(NULL, NULL, NULL, NULL);

  tree_insert(tree, key1, elem1);
  tree_insert(tree, key2, elem2);
  tree_insert(tree, key3, elem3);
  tree_insert(tree, key4, elem4);

  tree_get(tree, key1, &elemget);
  CU_ASSERT_TRUE(elemget.i = elem1.i);

  tree_get(tree, key4, &elemget);
  CU_ASSERT_TRUE(elemget.i = elem4.i);

  bool test = tree_get(tree, key5, &elemget);
  CU_ASSERT_TRUE(!test);

  tree_delete(tree, true, true);
}

void test_tree_remove()
{
  elem_t elem1 = { .i = 1};
  elem_t elem2 = { .i = 2};
  elem_t elem3 = { .i = 3};
  elem_t elem4 = { .i = 4};
  elem_t elemget;

  tree_key_t key1 = { .i = 1};
  tree_key_t key2 = { .i = 2};
  tree_key_t key3 = { .i = 3};
  tree_key_t key4 = { .i = 4};

  tree_t *tree = tree_new(NULL, NULL, NULL, NULL);

  tree_insert(tree, key1, elem1);
  tree_insert(tree, key2, elem2);
  tree_insert(tree, key3, elem3);
  tree_insert(tree, key4, elem4);

  bool test = tree_remove(tree, key1, &elemget);
  CU_ASSERT_TRUE(test);
  CU_ASSERT_TRUE(elemget.i = elem1.i);
  CU_ASSERT_TRUE(tree_size(tree) == 3);

  test = tree_remove(tree, key4, &elemget);
  CU_ASSERT_TRUE(test);
  CU_ASSERT_TRUE(elemget.i = elem4.i);
  CU_ASSERT_TRUE(tree_size(tree) == 2);

  tree_delete(tree, true, true);
}

void test_tree_keys()
{
  elem_t elem1 = { .i = 1};
  elem_t elem2 = { .i = 2};
  elem_t elem3 = { .i = 3};
  elem_t elem4 = { .i = 4};
  elem_t elemget;

  tree_key_t key1 = { .i = 1};
  tree_key_t key2 = { .i = 2};
  tree_key_t key3 = { .i = 3};
  tree_key_t key4 = { .i = 4};

  tree_t *tree = tree_new(NULL, NULL, NULL, NULL);

  tree_insert(tree, key1, elem1);
  tree_insert(tree, key2, elem2);
  tree_insert(tree, key3, elem3);
  tree_insert(tree, key4, elem4);

  tree_key_t *key_list = tree_keys(tree);

  elemget = key_list[0];
  CU_ASSERT_TRUE(elemget.i == key1.i);

  elemget = key_list[3];
  CU_ASSERT_TRUE(elemget.i == key4.i);

  free(key_list);
  tree_delete(tree, true, true);
}

void test_tree_elements()
{
  elem_t elem1 = { .i = 1};
  elem_t elem2 = { .i = 2};
  elem_t elem3 = { .i = 3};
  elem_t elem4 = { .i = 4};
  elem_t elemget;

  tree_key_t key1 = { .i = 1};
  tree_key_t key2 = { .i = 2};
  tree_key_t key3 = { .i = 3};
  tree_key_t key4 = { .i = 4};

  tree_t *tree = tree_new(NULL, NULL, NULL, NULL);

  tree_insert(tree, key1, elem1);
  tree_insert(tree, key2, elem2);
  tree_insert(tree, key3, elem3);
  tree_insert(tree, key4, elem4);

  tree_key_t *elem_list = tree_elements(tree);

  elemget = elem_list[0];
  CU_ASSERT_TRUE(elemget.i == elem1.i);

  elemget = elem_list[3];
  CU_ASSERT_TRUE(elemget.i == elem4.i);

  free(elem_list);
  tree_delete(tree, true, true);
}

void test_tree_balance()
{
  elem_t elem1 = { .i = 1};
  elem_t elem2 = { .i = 2};
  elem_t elem3 = { .i = 3};
  elem_t elem4 = { .i = 4};
  elem_t elem5 = { .i = 5};
  elem_t elem6 = { .i = 6};
  elem_t elem7 = { .i = 7};

  tree_key_t key1 = { .i = 1};
  tree_key_t key2 = { .i = 2};
  tree_key_t key3 = { .i = 3};
  tree_key_t key4 = { .i = 4};
  tree_key_t key5 = { .i = 5};
  tree_key_t key6 = { .i = 6};
  tree_key_t key7 = { .i = 7};
  tree_key_t key8 = { .i = 8};

  tree_t *tree = tree_new(NULL, NULL, NULL, NULL);

  int i = 0;

  tree_insert(tree, key1, elem1);
  i = tree_depth(tree);
  CU_ASSERT_TRUE(i == 1);

  tree_insert(tree, key2, elem2);
  i = tree_depth(tree);
  CU_ASSERT_TRUE(i == 2);

  tree_insert(tree, key3, elem3);
  tree_insert(tree, key4, elem4);
  i = tree_depth(tree);
  CU_ASSERT_TRUE(i == 3);

  tree_insert(tree, key5, elem5);
  tree_insert(tree, key6, elem6);
  tree_insert(tree, key7, elem7);
  tree_insert(tree, key8, elem5);
  i = tree_depth(tree);
  CU_ASSERT_TRUE(i == 4);

  tree_delete(tree, true, true);

}

int main(int argc, char *argv[argc])
{
  // Initialise
  CU_initialize_registry();

  // Set up suites and tests
  CU_pSuite creation = CU_add_suite("Test creation and height", NULL, NULL);
  CU_add_test(creation, "Creation", test_tree_creation);
  CU_add_test(creation, "Insertion", test_tree_insert);

  CU_pSuite useability = CU_add_suite("Test useability", NULL, NULL);
  CU_add_test(useability, "Tree has key", test_tree_has_key);
  CU_add_test(useability, "Tree get", test_tree_get);
  CU_add_test(useability, "Tree remove", test_tree_remove);
  CU_add_test(useability, "Tree keys", test_tree_keys);
  CU_add_test(useability, "Tree elements", test_tree_elements);

  CU_pSuite structure = CU_add_suite("Test structure", NULL, NULL);
  CU_add_test(structure, "Balance", test_tree_insert);

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

  tree_key_t key1 = { .i = 1};
  tree_key_t key2 = { .i = 2};
  tree_key_t key3 = { .i = 3};
  tree_key_t key4 = { .i = 4};
  tree_key_t key5 = { .i = 5};
  tree_key_t key6 = { .i = 6};
  tree_key_t key7 = { .i = 7};
  tree_key_t key8 = { .i = 8};
  tree_key_t key9 = { .i = 9};
  tree_key_t key10 = { .i = 10};
  tree_key_t key11 = { .i = 11};

*/
