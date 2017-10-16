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
	tree_key_t  key;
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
void deleter(tree_t *tree, node_t *node, bool delete_keys, bool delete_elements)
{
	if(delete_keys)
	{
		tree->free_key(node->key);
	}
	if(delete_elements)
	{
		tree->free_elem(node->elem);
	}
	free(node);
}

void tree_delete_aux(tree_t *tree, node_t *cursor, bool delete_keys, bool delete_elements)
{
	if(cursor == NULL)
	{
		return;
	}
	else
	{
		tree_delete_aux(tree, cursor->left, delete_keys, delete_elements);
		tree_delete_aux(tree, cursor->right, delete_keys, delete_elements);
		deleter(tree, cursor, delete_keys, delete_elements);
	}
}

void tree_delete(tree_t *tree, bool delete_keys, bool delete_elements)
{
	if(tree)
	{
		tree_delete_aux(tree, tree->root, delete_keys, delete_elements);
		free(tree);
	}
	else return;
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
	int left_tree_depth;
	int right_tree_depth;
	if (node != NULL)
	{
		left_tree_depth = node_depth(node->left);
		right_tree_depth = node_depth(node->right);
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

/// ---------- BALANCING ----------
/// ---------- LEFT LEFT ----------
node_t *rotate_ll(node_t *node)
{
	node_t *z = node;
	node_t *y = z->left;

	z->left = y->right;
	y->right = z;

	return y;
}

/// ---------- LEFT RIGHT ----------
node_t *rotate_lr(node_t *node)
{
	node_t *z = node;
	node_t *y = z->left;
	node_t *x = y->right;

	z->left = x->right;
	y->right = x->left;
	x->left = y;
	x->right = z;

	return x;
}

/// ---------- RIGHT RIGHT ----------
node_t *rotate_rr(node_t *node)
{
	node_t *z = node;
	node_t *y = z->right;

	z->right = y->left;
	y->left = z;

	return y;
}

/// ---------- RIGHT LEFT ----------
node_t *rotate_rl(node_t *node)
{
	node_t *z = node;
	node_t *y = z->right;
	node_t *x = y->left;

	z->right = x->left;
	y->left = x->right;
	x->right = y;
	x->left = z;

	return x;
}

/// ---------- L/R BALANCE ----------
int tree_balance(node_t *node)
{
	int balance = 0;

	if(node->left)
	{
		balance += node_depth(node->left);
	}
	if(node->right)
	{
		balance -= node_depth(node->right);
	}

	return balance;
}

/// ---------- NODE BALANCE ----------
node_t *balance_node(node_t *node)
{
	node_t *new_local_root = NULL;
	// Gå ner i trädet och balancera ifrån botten först
	if(node->left)
	{
		node->left = balance_node(node->left);
	}
	if(node->right)
	{
		node->right = balance_node(node->right);
	}

	int balance = tree_balance(node);

	// Balanceringen
	if(balance >= 2)                           // Vänster för djup
	{
		if(tree_balance(node->left) <= -1)
		{
			new_local_root = rotate_ll(node);
		}
		else
		{
			new_local_root = rotate_lr(node);
		}
	}

	else if(balance <= -2)                     // Höger för djup
	{
		if(tree_balance(node->right) <= -1)
		{
			new_local_root = rotate_rr(node);
		}
		else
		{
			new_local_root = rotate_rl(node);
		}
	}
	else                                       // Trädet redan balancerat
	{
		new_local_root = node;
	}
	return new_local_root;
}

/// ---------- TREE BALANCE ----------
void balance_tree(tree_t *tree)
{
	node_t *new_root = NULL;
	new_root = balance_node(tree->root);

	if(new_root != tree->root)                 // Om rotnoden ändrats, peka om den till nya.
	{
		tree->root = new_root;
	}
	return;
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
		balance_tree(tree);
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
	if (*key_locator(tree, cursor, key) != NULL)
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
	node_t **cursor = &(tree->root);
	node_t **found_node = key_locator(tree, cursor, key);
	if (*found_node != NULL)
	{
		*result = ((*found_node)->elem);
		return true;
	}
	return false;
}

void tree_insert_node(tree_t *tree, node_t *node, tree_key_t key)
{
	node_t **cursor = key_locator(tree, &(tree->root), key);
	if(*cursor)              //Om cursor != NULL, så finns redan key
	{
		return;
	}
	else    // Annars har key_locator returnerat platsen där key hör hemma
	{
		*cursor = node;
		balance_tree(tree);
	}
}


/// Removes the element for a given key in tree.
///
/// \param tree pointer to the tree
/// \param key the key of elem to be removed
/// \param result a pointer to where result can be stored (only defined when result is true)
/// \returns: true if key is a key in the tree
/// NOTE: Ska ta bort noden, key, elem. Result ska va ny adress, med kopia av borttagna elem
bool tree_remove(tree_t *tree, tree_key_t key, elem_t *result)
{
  node_t **to_delete = key_locator(tree, &(tree->root), key);
  if(to_delete)
    {
      if(*to_delete == tree->root)            // Om edge case root
        {
          node_t **cursor = to_delete;
          if((*cursor)->right)                // Om höger träd finns
            {
              node_t *tmpleft = tree->root->left;
              node_t *tmpright = tree->root->right;
              cursor = &(*cursor)->right;
              while((*cursor)->left != NULL)
                {
                  cursor = &(*cursor)->left;
                }
              //(*cursor)->left = tmpleft;
              //(*cursor)->right = tmpright;

              (*cursor)->left = tree->root->left;
              (*cursor)->right = tree->root->right;
              tree->root = *cursor;

              (*cursor) = NULL;
            }

          else if((*cursor)->left)            // Om inte höger finns, men vänster. Ny root.
            {
              tree->root = (*cursor)->left;
            }
          /*
          *result = (*to_delete)->elem;
          tree->free_elem((*to_delete)->elem);
          tree->free_key((*to_delete)->key);
          free(*to_delete);
          */
          return true;
        }

      else
        {
          node_t *tmp_left = NULL;
          node_t *tmp_right = NULL;

          if((*to_delete)->left)
            {
              tmp_left = (*to_delete)->left;
            }
          if((*to_delete)->right)
            {
              tmp_right = (*to_delete)->right;
            }

          *result = (*to_delete)->elem;
          tree->free_elem((*to_delete)->elem);
          tree->free_key((*to_delete)->key);
          free(*to_delete);

          return true;
        }
    }
  else return false;
  /*
	node_t **to_delete = key_locator(tree, &(tree->root), key);
	if(to_delete)
	{
		node_t *tmp_left = NULL;
		node_t *tmp_right = NULL;
    bool root_reroute = false;

		if ((*to_delete)->left)
		{
			tmp_left = (*to_delete)->left;
		}

		if((*to_delete)->right)
		{
			tmp_right = (*to_delete)->right;
		}

    if(tree->root == *to_delete)
      {
        puts("deleting tree->root!\n");
        root_reroute = true;
        node_t **cursor = &(*to_delete)->right;
        while((*cursor)->left != NULL)
          {
            puts("iterating...\n");
            cursor = &((*cursor)->left);
          }
        (*cursor)->left = tree->root->left;
        (*cursor)->right = tree->root->right;
        tree->root = (*cursor);
      }

		*result = (*to_delete)->elem;                         //Skapa kopian
		tree->free_elem((*to_delete)->elem);
		tree->free_key((*to_delete)->key);
		free(*to_delete);
		--tree->size;

    if (tmp_left && !root_reroute)                         //Om dotter vänster finns, reinsert.
		{
      puts("insert right\n");
			tree_insert_node(tree, tmp_left, tmp_left->key);
		}

		if (tmp_right && !root_reroute)                        //Om dotter höger finns, reinsert.
		{
      puts("insert right\n");
			tree_insert_node(tree, tmp_right, tmp_right->key);
		}

		return true;

	}
	return false;
  */

}

/// Returns an array holding all the keys in the tree
/// in ascending order.
///
/// \param tree pointer to the tree
/// \returns: array of tree_size() keys
tree_key_t *tree_keys_aux(node_t *cursor, tree_key_t *key_array, int *i)
{
	if (cursor == NULL)
	{
		return key_array;
	}
	if (cursor != NULL)
	{
		tree_keys_aux(cursor->left, key_array, i);
		*i = *i + 1;
		key_array[*i] = cursor->key;
		tree_keys_aux(cursor->right, key_array, i);
	}
	return key_array;
}

tree_key_t *tree_keys(tree_t *tree)
{
	int i = -1;
	tree_key_t *key_array = calloc(tree->size, sizeof(tree_key_t));
	//NOTE: Glöm inte free!!!
	return tree_keys_aux(tree->root, key_array, &i);
}

/// Returns an array holding all the elements in the tree
/// in ascending order of their keys (which are not part
/// of the value).
///
/// \param tree pointer to the tree
/// \returns: array of tree_size() elements
tree_key_t *tree_elem_aux(node_t *cursor, elem_t *elem_array, int *i)
{
	if (cursor == NULL)
	{
		return elem_array;
	}
	if (cursor != NULL)
	{
		tree_elem_aux(cursor->left, elem_array, i);
		*i = *i + 1;
		elem_array[*i] = cursor->key;
		tree_elem_aux(cursor->right, elem_array, i);
	}
	return elem_array;
}

elem_t *tree_elements(tree_t *tree)
{
	int i = -1;
	elem_t *elem_array = calloc(tree->size, sizeof(elem_t));
	//NOTE: Glöm inte free!!!
	return tree_elem_aux(tree->root, elem_array, &i);
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
bool tapply_inorder(node_t *cursor, bool *success, key_elem_apply_fun fun, void *data)
{
	bool fun_result = false;
	if (cursor == NULL)
	{
		return true;
	}
	if (cursor != NULL)
	{
		tapply_inorder(cursor->left, success, fun, data);
		fun_result = fun(cursor->key, cursor->elem, data);
		if (fun_result) *success = true;
		tapply_inorder(cursor->right, success, fun, data);
	}
	return true;
}

/// ---------- PRE ORDER ----------
bool tapply_preorder(node_t *cursor, bool *success, key_elem_apply_fun fun, void *data)
{
	bool fun_result = false;
	if (cursor == NULL)
	{
		return true;
	}
	if (cursor != NULL)
	{
		fun_result = fun(cursor->key, cursor->elem, data);
		if (fun_result) *success = true;
		tapply_preorder(cursor->left, success, fun, data);
		tapply_preorder(cursor->right, success, fun, data);
	}
	return true;
}

/// ---------- POST ORDER ----------
bool tapply_postorder(node_t *cursor, bool *success, key_elem_apply_fun fun, void *data)
{
	bool fun_result = false;
	if (cursor == NULL)
	{
		return true;
	}
	if (cursor != NULL)
	{
		tapply_postorder(cursor->left, success, fun, data);
		tapply_postorder(cursor->right, success, fun, data);
		fun_result = fun(cursor->key, cursor->elem, data);
		if (fun_result) *success = true;
	}
	return true;
}

/// ---------- ACTUALL APPLY ----------
bool tree_apply(tree_t *tree, enum tree_order order, key_elem_apply_fun fun, void *data)
{
	bool success = false;
	if(order == inorder)
	{
		tapply_inorder(tree->root, &success, fun, data);
		return success;
	}
	if(order == preorder)
	{
		tapply_preorder(tree->root, &success, fun, data);
		return success;
	}
	if(order == postorder)
	{
		tapply_postorder(tree->root, &success, fun, data);
		return success;
	}
	return false;
}
