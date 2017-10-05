#include "list.h"
#include "tree.h"
#include "common.h"
#include <stdlib.h>
#include <stdio.h>

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
  list_insert(the_list, 1, elem4);

  list_prepend(the_list, elem5);

  list_append(the_list, elem6);

  list_insert(the_list, 1000, elem8);

  puts("running list get\n");
  elem_t *retrieved;
  list_get(the_list, 7, retrieved);

  list_apply(the_list, print_func, NULL);

  return 0;
}
