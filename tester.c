#include "list.h"
#include "tree.h"
#include "common.h"
#include <stdlib.h>
#include <stdio.h>

elem_t copy_func(elem_t elem)
{
  //elem_t ret;
  //elem_t *copy = calloc(1, sizeof(elem_t));
  //*copy = elem;
  return elem;
}

void free_func();

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

int main(int argc, char *argv[])
{
  elem_t elem1 = { .i=1 };
  elem_t elem2 = { .i=2 };
  elem_t elem3 = { .i=3 };

  list_t *the_list = list_new(copy_func, NULL, comp_func);

  list_insert(the_list, 0, elem1);
  list_insert(the_list, 1, elem2);
  list_insert(the_list, 2, elem3);

  list_apply(the_list, print_func, NULL);

  return 0;
}
