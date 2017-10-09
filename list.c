#include <stdbool.h>
#include "common.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct node node_t;

struct node{
  node_t *next;
  elem_t elem;
};

typedef struct list list_t;

struct list{
  node_t *first;
  node_t *last;

  element_copy_fun copy;
  element_free_fun free;
  element_comp_fun compare;

  int size;
};
/// Converts any number to a valid index
int index_correction(list_t *list, int index)
{
  if(index < 0)                       // Om index är negativ, ommvandla till posetiv
    {
      index = index + 1 + list->size;
    }
  if(index > list->size)              // Om index > än lista, ändra index
    {
      index = -1;
    }
  return index;
}

/// Creates a new list
///
/// \param copy (may be NULL) a function applied to all elements when stored in the list
/// \param free (may be NULL) used to free elements in list_delete
/// \param compare (may be NULL) used to compare elements in list_contains
/// \returns: empty list
list_t *list_new(element_copy_fun copy, element_free_fun free, element_comp_fun compare)
{
  list_t *result = calloc(1, sizeof(list_t));

  result->copy = copy;
  result->free = free;
  result->compare = compare;

  result->size = 0;

  return result;
}

node_t *node_new(elem_t elem, node_t *next)
{
  node_t *result = calloc(1, sizeof(node_t));
  result->elem = elem;
  result->next = next;
  return result;
}

/// Inserts a new element at a given index.
///
/// If list's copy function is non-NULL, it will be applied to elem and its result
/// stored in the list. Otherwise, elem will be stored in the list. 
///
/// All indexes are valid. 0 means first element. Negative indexes
/// count backward and too large negative indexes equal 0. Too
/// large positive indexes are same as -1.
/// 
/// \param list  pointer to the list
/// \param index the index for elem to be inserted at
/// \param elem  the element to be inserted
void list_insert(list_t *list, int index, elem_t elem)
{
  index = index_correction(list, index);

  printf("index = %d, list->size = %d\n", index, list->size);

  if(list->first == NULL)                 // Om tom lista
    {
      list->first = node_new(list->copy(elem), NULL);
      list->last = list->first;
      ++list->size;
    }
  else
    {
      if(index == 0)                          // Om ny första position
        {
          node_t *new_first = node_new(list->copy(elem), list->first);
          list->first = new_first;
          ++list->size;
        }
      else
        {
          node_t **cursor = &(list->first);
          for(int i = 0; i < index; i++)
            {
                  cursor = &((*cursor)->next);
            }
          *cursor = node_new(list->copy(elem), *cursor);
          if(index == list->size-1)             // Om ny sista position
            {
              list->last = list->last->next;
            }
          ++list->size;
        }
    }
}

/// Inserts a new element at the end of the list.
///
/// If list's copy function is non-NULL, it will be applied to elem and its result
/// stored in the list. Otherwise, elem will be stored in the list.
///
/// \param list pointer to the list
/// \param elem the element to be appended
void list_append(list_t *list, elem_t elem)
{
  list_insert(list, -1, elem);
}

/// Inserts a new element at the beginning of the list
///
/// If list's copy function is non-NULL, it will be applied to elem and its result
/// stored in the list. Otherwise, elem will be stored in the list.
///
/// \param list pointer to the list
/// \param elem the element to be prepended
void list_prepend(list_t *list, elem_t elem)
{
  list_insert(list, 0, elem);
}

/// Finds a element from a given index
node_t **find_in_list(list_t *list, int index)
{
  node_t **cursor = &(list->first);
  index = index_correction(list, index);
  for(int i = 0; i < index; i ++)
    {
      cursor = &((*cursor)->next);
    }
  return cursor;
}

/// Removes an element from a list.
///
/// All indexes are valid. 0 means first element. Negative indexes
/// count backward and too large negative indexes equal 0. Too
/// large positive indexes are same as -1.
///
/// \param list  pointer to the list
/// \param index the index to be removed
/// \param delete if true, run list's free function on the removed element
void list_remove(list_t *list, int index, bool delete)
{
  index = index_correction(list, index);
  if(list->size == 0)                       // Tom lista
    {
      return;
    }
  if(delete)                                // Delete all
    {
      node_t **cursor = &(list->first);
      for(int i = 0; i < list->size; i++)
        {
          list->free((*cursor)->elem);
          cursor = &((*cursor)->next);
        }
      return;
    }
  else                                      // Delete @ index
    {
      if(index == list->size) --index;      // NOTE: Ingen aning om varför detta måste göras.
      // NOTE: Varför funkar detta utan att peka om list->first!?
      node_t **cursor = find_in_list(list, index);
      node_t *to_delete = *cursor;
      printf("index = %d, list->size = %d\n", index, list->size);
      if(index == list->size-1)             // Om sista positionen, peka om list->last
        {
          node_t **new_last = find_in_list(list, -2);
          list->last = *new_last;
        }
      else                                  // Annars peka "över" bortagna element
        {
          *cursor = (*cursor)->next;
        }
      list->free((to_delete)->elem);
      free(to_delete);
      --list->size;
      return;
    }
}

/// Returns the element at a given index
/// \param list  pointer to the list
/// \param index the index to be returned
/// \param result pointer to where the element at index index will be stored if true is returned
/// \returns true if index was a valid index
bool list_get(list_t *list, int index, elem_t *result)
{
  if(index < 0 || index > list->size)
    {
      return false;
    }
  else
    {
      node_t **cursor = &(list->first);
      for(int i = 0; i < index; i++)
        {
          cursor = &((*cursor)->next);
        }
      *result = ((*cursor)->elem);
      //int tmp = (*cursor)->elem.i;
      //printf("elem @ index %d is %d\n", index, tmp); // Sketchy, funkar bara på int
      return true;
    }
}

/// A convenience for list_get(list, 0, result)
bool list_first(list_t *list, elem_t *result)
{
  return list_get(list, 0, result);
}

/// A convenience for list_get(list, -1, result)
bool list_last(list_t *list, elem_t *result)
{
  return list_get(list, -1, result);
}

/// Returns the length of the list. It is undefined
/// whether the length is calculated in O(n) time or
/// whether there is a size counter in the list object
/// that is manipulated by insert, remove, etc. 
/// \param list the list
/// \returns the length of list
int list_length(list_t *list)
{
  return list->size;
}

/// Deletes a list. 
///
/// \param list pointer to the list
/// \param delete if true, use list's free function to free elements
void list_delete(list_t *list, bool delete)
{
  return;
}

/// Applies a function to all elements in a list in list order
///
/// \param list the list
/// \param fun the function to apply to all elements
/// \param data an extra argument passed to each call to fun (may be NULL)
/// \returns the result of all fun calls, combined with OR (||)
bool list_apply(list_t *list, elem_apply_fun fun, void *data)
{
  node_t *cursor = list->first;
  bool success = false;
  bool success_store = false;;
  for(int i = 0; i < list->size; i++)
    {
      success = fun(cursor->elem, data);
      cursor = cursor->next;
      if(success)
        {
          success_store = true;
        }
    }
  return success_store;
}

/// Searches for an element in a list
///
/// Uses list's compare if non-NULL, otherwise uses == for comparison. 
///
/// \param list the list
/// \param elem the element to search for
/// \returns the index of elem in list, or -1 if not found
int list_contains(list_t *list, elem_t elem)
{
  return 0;
}
