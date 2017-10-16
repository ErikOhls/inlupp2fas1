#include "list.h"
#include "tree.h"
#include "common.h"
#include <stdlib.h>
#include <stdio.h>

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

struct item{
	char *name;
	int amount;
};

typedef struct item item_t;

item_t *new_item(char* name, int amount)
{
	item_t *new_item = calloc(1, sizeof(item_t));
	new_item->name = name;
	new_item->amount = amount;
	return new_item;
}

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


/// TREE item version

elem_t t_copy_func(elem_t elem)
{
	item_t *from = elem.p;
	item_t *to = calloc(1, sizeof(item_t));

	*to = *from;

	elem_t result;
	result.p = to;
	return result;
}

void t_free_func(elem_t elem)
{

	free(elem.p);
}

int t_comp_func(elem_t elem, elem_t elem2)
{
	if(((item_t*)elem.p)->amount > ((item_t*)elem2.p)->amount)
	{
		return 2;
	}
	if(((item_t*)elem.p)->amount < ((item_t*)elem2.p)->amount)
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
	printf("key = %d, elem = %s\n", ((item_t*)key.p)->amount, ((item_t*)elem.p)->name);
	return true;
}

void print_specific(elem_t elem)
{
	//printf("elem retrieved = %d\n", elem);
}

int main(int argc, char **argv)
{
	/// List Shelf version
	bool list = false;;
	if(list)
	{
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

		puts("freeing elements\n");
		free(elem1.p);
		free(elem2.p);
		free(elem3.p);
		free(elem4.p);
		free(elem5.p);
		free(elem6.p);
		free(elem7.p);
		free(elem8.p);
		free(elem9.p);
	}
	/// Tree item version
	else
	{
		elem_t telem1 = { .p = new_item("ett" ,1) };
		elem_t telem2 = { .p = new_item("två", 2) };
		elem_t telem3 = { .p = new_item("tre", 3) };
		elem_t telem4 = { .p = new_item("fyra", 4) };
		elem_t telem5 = { .p = new_item("fem", 5) };
		elem_t telem6 = { .p = new_item("sex", 6) };
		elem_t telem7 = { .p = new_item("sju", 7) };
		elem_t telem8 = { .p = new_item("åtta", 8) };
		elem_t telem9 = { .p = new_item("nio", 9) };
		elem_t telem10 = { .p = new_item("tio", 10) };

		puts("making tree\n");
		tree_t *tree = tree_new(t_copy_func, t_free_func, t_free_func, t_comp_func);

		puts("inserting elements\n");
		tree_insert(tree, telem1, telem1);
		tree_insert(tree, telem2, telem2);
		tree_insert(tree, telem3, telem3);
		tree_insert(tree, telem4, telem4);
		tree_insert(tree, telem5, telem5);
		tree_insert(tree, telem6, telem6);
		tree_insert(tree, telem7, telem7);
		tree_insert(tree, telem8, telem8);
		tree_insert(tree, telem9, telem9);

		puts("printing in-order tree:\n");
		tree_apply(tree, inorder, tree_print_func, NULL);

		int i = tree_depth(tree);
		printf("depth of tree = %d\n", i);

		puts("printing pre-order tree:\n");
		tree_apply(tree, preorder, tree_print_func, NULL);
    /*
		puts("tree has key elem 10");
		if(tree_has_key(tree, telem10))
		{
			puts("true!\n");
		}
		else puts("false!\n");

		puts("tree_get\n");
		elem_t elemget;
		if(tree_get(tree, telem1, &elemget))
		{
			puts("true!\n");
			printf("elem = %s\n", ((item_t*)elemget.p)->name);
		}
		else puts("false!\n");

		puts("tree_keys\n");
		tree_key_t *key_list = tree_keys(tree);
		free(key_list);

		puts("tree_elements\n");
		elem_t *elem_list = tree_elements(tree);
		free(elem_list);
    */
		elem_t elemget2;
		puts("Tree_remove:\n");
		tree_remove(tree, (telem4), &elemget2);

		puts("printing pre-order tree:\n");
		tree_apply(tree, preorder, tree_print_func, NULL);

		puts("deleting tree\n");
		tree_delete(tree, true, true);

		puts("freeing elems");
		free(telem1.p);
		free(telem2.p);
		free(telem3.p);
		free(telem4.p);
		free(telem5.p);
		free(telem6.p);
		free(telem7.p);
		free(telem8.p);
		free(telem9.p);
		free(telem10.p);
	}
	return 0;
}

