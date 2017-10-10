#include "list.h"
#include "tree.h"
#include "common.h"
#include <stdlib.h>
#include <stdio.h>
///int version
/*
elem_t copy_func(elem_t elem)
{
  return elem;
}

void free_func(elem_t elem)
{
  return;
}

int comp_func(elem_t elem, elem_t elem2)
{
  if(elem.i > elem2.i)
    {
      return 2;
    }
  if(elem.i < elem2.i)
    {
      return 1;
    }
  else return 0;
}

bool print_func(elem_t elem, void *data)
{
  printf("element = %d\n", elem.i);
  return true;
}

bool fail_func(elem_t elem, void *data)
{
  return false;
}

int main(int argc, char **argv)
{
  elem_t elem1 = { .i=1 };
  elem_t elem2 = { .i=2 };
  elem_t elem3 = { .i=3 };
  elem_t elem4 = { .i=4 };
  elem_t elem5 = { .i=5 };
  elem_t elem6 = { .i=6 };
  elem_t elem7 = { .i=7 };
  elem_t elem8 = { .i=8 };
  elem_t elem9 = { .i=9 };

  list_t *the_list = list_new(copy_func, free_func, comp_func);

  list_insert(the_list, 0, elem1);

  list_insert(the_list, 1, elem2);
  list_insert(the_list, 2, elem3);
  list_insert(the_list, 3, elem4);

  list_prepend(the_list, elem5);

  list_append(the_list, elem6);

  list_insert(the_list, 1000, elem7);

  puts("running list get\n");
  elem_t *retrieved;
  list_get(the_list, 6, retrieved);

  list_apply(the_list, print_func, NULL);

  puts("running remove\n");
  list_remove(the_list, 60, true);

  list_apply(the_list, print_func, NULL);

  return 0;
}
*/

/// Struct list version

struct shelf{
  char *name;
  int amount;
};

typedef struct shelf shelf_t;

shelf_t *new_shelf(char* name, int amount)
{
  shelf_t *new_shelf = calloc(1, sizeof(shelf_t));
  new_shelf->name = name;
  new_shelf->amount = amount;
  return new_shelf;
}
/*
elem_t copy_func(elem_t elem)
{
  shelf_t *from = elem.p;
  shelf_t *to = calloc(1, sizeof(shelf_t));

  *to = *from;

  elem_t result;
  result.p = to;
  return result;
}

void free_func(elem_t elem)
{
  free(elem.p);
}

int comp_func(elem_t elem, elem_t elem2)
{
  if(((shelf_t*)elem.p)->amount > ((shelf_t*)elem2.p)->amount)
    {
      return 2;
    }
  if(((shelf_t*)elem.p)->amount < ((shelf_t*)elem2.p)->amount)
    {
      return 1;
    }
  else return 0;
}

bool print_func(elem_t elem, void *data)
{
  printf("element = %s\n", ((shelf_t*)elem.p)->name);
  return true;
}

void print_tmp(elem_t elem)
{
  printf("element found = %s\n", ((shelf_t *)elem.p)->name);
}
*/

/// TREE Int version

elem_t copy_func(elem_t elem)
{
  return elem;
}

void free_func(elem_t elem)
{
  return;
}

int comp_func(elem_t elem, elem_t elem2)
{
  if(elem.i > elem2.i)
    {
      return 2;
    }
  if(elem.i < elem2.i)
    {
      return 1;
    }
  else
    {
      puts("comp_func has reached equality\n");
      return 0;
    }
}

bool tree_print_func(tree_key_t key, elem_t elem, void *data)
{
  printf("key = %d\n", key.i);
  return true;
}

int main(int argc, char **argv)
{
  bool list = false;
  if(list)
    {
      /*
      elem_t elem1 = { .p = new_shelf("ett", 0) };
      elem_t elem2 = { .p = new_shelf("två", 1) };
      elem_t elem3 = { .p = new_shelf("tre", 2) };
      elem_t elem4 = { .p = new_shelf("fyra", 3) };
      elem_t elem5 = { .p = new_shelf("fem", 4) };
      elem_t elem6 = { .p = new_shelf("sex", 5) };
      elem_t elem7 = { .p = new_shelf("sju", 6) };
      elem_t elem8 = { .p = new_shelf("åtta", 7) };
      elem_t elem9 = { .p = new_shelf("ni0", 8) };

      list_t *the_list = list_new(copy_func, free_func, comp_func);

      list_insert(the_list, 0, elem1);

      list_insert(the_list, 1, elem2);
      list_insert(the_list, 2, elem3);
      list_insert(the_list, 3, elem4);

      list_prepend(the_list, elem5);
      list_append(the_list, elem6);
      list_insert(the_list, 1000, elem7);

      list_apply(the_list, print_func, NULL);

      puts("running list get\n");
      elem_t retrieved;
      list_get(the_list, 6, &retrieved);

      puts("printing retrieved elem\n");
      print_tmp(retrieved);

      list_apply(the_list, print_func, NULL);

      puts("running remove\n");
      list_remove(the_list, 6, true);

      puts("printing list:\n");
      list_apply(the_list, print_func, NULL);

      puts("running delete\n");
      list_delete(the_list, true);

      puts("running delete again\n");
      //list_delete(the_list, true);
      
      puts("list_contains:\n");
      int i = list_contains(the_list, elem1);
      printf("%d\n", i);
      */
    }
  else
    {
      tree_t *tree = tree_new(copy_func, free_func, free_func, comp_func);

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

      puts("inserting elements\n");

      tree_insert(tree, elem5, elem5);
      tree_insert(tree, elem1, elem1);
      tree_insert(tree, elem6, elem6);
      tree_insert(tree, elem2, elem2);
      tree_insert(tree, elem7, elem7);
      tree_insert(tree, elem3, elem3);
      tree_insert(tree, elem4, elem4);
      tree_insert(tree, elem8, elem8);
      tree_insert(tree, elem9, elem9);

      puts("printing in-order tree:\n");

      tree_apply(tree, inorder, tree_print_func, NULL);

      int i = tree_depth(tree);
      printf("depth of tree = %d\n", i);

      puts("printing pre-order tree:\n");

      tree_apply(tree, preorder, tree_print_func, NULL);

      puts("tree has key elem 10");
      if(tree_has_key(tree, elem10))
        {
          puts("true!\n");
        }
      else puts("false!\n");
    }
  return 0;
}

