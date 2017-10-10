#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "common.h"

typedef elem_t tree_key_t;
typedef element_free_fun key_free_fun;

typedef struct node node_t;

struct node{
  node_t *    left;
  node_t *    right;
  tree_key_t  key; // Eller?
  elem_t      elem;
};

typedef struct tree tree_t;

struct tree{
  node_t *root;
  int     size;

  element_copy_fun copy;
  key_free_fun     free_key;
  element_free_fun free_elem;
  element_comp_fun compare;

};

/// Creates a new tree
///
/// \param copy (may be NULL) a function applied to all elements when stored in the tree
/// \param key_free (may be NULL) used to free keys in tree_delete
/// \param elem_free (may be NULL) used to free elements in tree_delete
/// \param compare (may be NULL) used to compare keys
/// \returns: empty tree
tree_t *tree_new(element_copy_fun element_copy, key_free_fun key_free, element_free_fun elem_free, element_comp_fun compare)
{
  tree_t *result = calloc(1, sizeof(tree_t));

  result->size = 0;

  result->copy      = element_copy;
  result->free_key  = key_free;
  result->free_elem = elem_free;
  result->compare   = compare;

  return result;
}

node_t *tree_node_new(tree_t *tree, tree_key_t key, elem_t elem)
{
  node_t *result = calloc(1, sizeof(node_t));
  result->key = tree->copy(key);
  result->elem = tree->copy(elem);
  return result;
}

node_t **key_locator(tree_t *tree, node_t **cursor, tree_key_t key)
{
  while(*cursor != NULL)                            //Medans tom nod ej är nådd
    {
      if(tree->compare((*cursor)->key, key) == 0)   //Key är hittad
        {
          return cursor;
        }
      if(tree->compare((*cursor)->key, key) == 1)   //Flytta cursor höger
        {
          cursor = &((*cursor)->right);
        }
      else                                          //Vänster
        {
          cursor = &((*cursor)->left);
        }
    }
  return cursor;                                    //Om key ej hittas, returna där cursor landade
}



/// Remove a tree along with all elem_t elements.
///
/// \param tree the tree
/// \param delete_keys if true, run tree's key_free function on all keys
/// \param delete_elements if true, run tree's elem_free function on all elements
void tree_delete(tree_t *tree, bool delete_keys, bool delete_elements)
{
  return;
}

/// Get the size of the tree 
///
/// \returns: the number of nodes in the tree
int tree_size(tree_t *tree)
{
  return tree->size;
}

// If first node not NULL
// 
int node_depth(node_t *node)
{
	int right_tree_depth;
	int left_tree_depth;
	if (node != NULL)
	{
		right_tree_depth = node_depth(node->right);
		left_tree_depth = node_depth(node->left);
		if (right_tree_depth > left_tree_depth)
		{
			return (right_tree_depth + 1);
		}
		else
		{
			return (left_tree_depth + 1);
		}
	}
	else
	{
		return 0;
	}
}


/// Get the depth of the tree 
///
/// \returns: the depth of the deepest subtree
int tree_depth(tree_t *tree)
{
  return (node_depth(tree->root));
}

/// Insert element into the tree. Returns false if the key is already used.
///
/// Uses the tree's compare function to compare keys.
///
/// If tree's copy function is non-NULL, it will be applied to
/// elem and its result stored in the tree. Otherwise, elem will
/// be stored in the tree. Note that keys are never copied and are
/// assumed to be immutable. (They may however be freed by the
/// tree.)
///
/// \param tree pointer to the tree
/// \param key the key of element to be appended -- this is assumed to be an immutable value
/// \param elem the element 
/// \returns: true if successful, else false
bool tree_insert(tree_t *tree, tree_key_t key, elem_t elem)
{
  node_t **cursor = key_locator(tree, &(tree->root), key);
  if(*cursor)                                                 //Om cursor != NULL, så finns redan key
    {
      return false;
    }
  else                                // Annars har key_locator returnerat platsen där key hör hemma
    {
      *cursor = tree_node_new(tree, key, elem);
      ++tree->size;
      return true;
    }
}

/// Checks whether a key is used in a tree
///
/// Uses the tree's compare function to compare keys.
/// 
/// \param tree pointer to the tree
/// \param key the key to check for inclusion in the tree
/// \returns: true if key is a key in the tree
bool tree_has_key(tree_t *tree, tree_key_t key)
{
	node_t **cursor = &(tree->root);
	if (key_locator(tree, cursor, key) != NULL)
		{
			return true;
		}
	return false;
}

/// Finds the element for a given key in tree.
/// 
/// \param tree pointer to the tree
/// \param key the key of elem to be removed
/// \param result a pointer to where result can be stored (only defined when result is true)
/// \returns: true if key is a key in the tree
bool tree_get(tree_t *tree, tree_key_t key, elem_t *result)
{
  return false;
}

/// Removes the element for a given key in tree.
///
/// \param tree pointer to the tree
/// \param key the key of elem to be removed
/// \param result a pointer to where result can be stored (only defined when result is true)
/// \returns: true if key is a key in the tree
bool tree_remove(tree_t *tree, tree_key_t key, elem_t *result)
{
  return false;
}

/// Returns an array holding all the keys in the tree
/// in ascending order.
///
/// \param tree pointer to the tree
/// \returns: array of tree_size() keys
tree_key_t *tree_keys(tree_t *tree)
{
 return 0; 
}

/// Returns an array holding all the elements in the tree
/// in ascending order of their keys (which are not part
/// of the value).
///
/// \param tree pointer to the tree
/// \returns: array of tree_size() elements
elem_t *tree_elements(tree_t *tree)
{
	
	return 0;
}

/// This function is used in tree_apply() to allow applying a function
/// to all elements in a tree. 
typedef bool(*key_elem_apply_fun)(tree_key_t key, elem_t elem, void *data);

enum tree_order { inorder = 0, preorder = -1, postorder = 1 };

/// Applies a function to all elements in the tree in a specified order.
/// Example (using shelf as key):
///
///     tree_t *t = tree_new();
///     tree_insert(t, "A25", some_item);
///     int number = 0;
///     tree_apply(t, inorder, print_item, &number);
///
/// where print_item is a function that prints the number and increments it,
/// and prints the item passed to it. 
///
/// \param tree the tree
/// \param order the order in which the elements will be visited
/// \param fun the function to apply to all elements
/// \param data an extra argument passed to each call to fun (may be NULL)
/// \returns the result of all fun calls, combined with OR (||)

/// ---------- IN ORDER ----------
bool tapply_inorder(node_t *cursor, key_elem_apply_fun fun, void *data)
{	
	bool fun_result = false;
  if (cursor == NULL)
    {
      return true;
    }
  if (cursor != NULL)
    {
      tapply_inorder(cursor->left, fun, data);
      fun_result = fun(cursor->key, cursor->elem, data);
	  if (fun_result) *success = true;
      tapply_inorder(cursor->right, fun, data);
    }
  return true;
}
/// ---------- IN ORDER ----------

/// ---------- IN ORDER ----------


bool tree_apply(tree_t *tree, enum tree_order order, key_elem_apply_fun fun, void *data)
{   //TODO: Fixa så return <bool> funkar korrekt
  if(order == inorder)
    {
      tapply_inorder(tree->root, fun, data);
      return true;
    }
  if(order == preorder)
    {
      puts("not implemented");
      return true;
    }
  if(order == postorder)
    {
      puts("not implemented");
      return true;
    }
  return false;
}
