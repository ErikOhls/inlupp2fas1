#include "utils.h"
#include "tree.h"
#include "list.h"
#include "common.h"
#include "db_aux.h"

//////////// ================= ITEM INPUT
///
/// Functions for inputing items to db
///
void add_item_to_db(tree_t *db, char *name, char *desc, int price, char *shelf_name, int amount)
{
  puts("making item\n");
  // Make shelf
  shelf_t *new_shelf = calloc(1, sizeof(shelf_t));
  new_shelf->shelf_name = shelf_name;
  new_shelf->amount = amount;

  //Make item
  item_t *new_item = calloc(1, sizeof(item_t));
  new_item->name = name;
  new_item->desc = desc;
  new_item->price = price;

  //Make list
  elem_t list_elem = { .p = new_shelf };
  new_item->list = list_new(l_copy_func, l_free_func, l_comp_func);
  list_append(new_item->list, list_elem);

  //Make elems
  elem_t elem = { .p = new_item };
  tree_key_t key = { .p = new_item->name };

  //Checking item and shelf existance
  elem_t found_elem;
  bool item_exists = tree_get(db, key, &found_elem);
  bool shelf_exists = tree_apply(db, inorder, check_shelf_existance, &list_elem);

  if(item_exists && shelf_exists)
    {
      puts("Vara samt varuhylla finns redan! Försök igen\n");

      free(new_shelf);
      free(new_item);

      return;
    }

  else if(item_exists)
    {
      tree_key_t key = { .p = name };
      elem_t result = {};
      tree_get(db, key, &result);
      list_append(((item_t*)result.p)->list, list_elem);

      free(new_shelf);
      free(new_item);

      return;
    }

  else if(shelf_exists)
    {
      puts("Varahyllan är redan tagen! Försök igen\n");
      free(new_shelf);
      free(new_item);

      return;
    }

  else
    {
      puts("inserting to tree\n");
      tree_insert(db, key, elem);
      free(new_shelf);
      free(new_item);

      return;
    }
}

void input_item(tree_t *db)
{
	char *name  = ask_question_string("Varans namn: ");
	char *desc  = ask_question_string("Beskrivning av vara: ");
	int price   = ask_question_int("Varans pris: ");
	char *shelf = ask_question_shelf("Varans hylla: ");
	int amount  = ask_question_int("Varans antal: ");

	add_item_to_db(db, name, desc, price, shelf, amount);

	return;
}

//////////// ================= LIST DB
///
/// Functions for listing the database
///
void page_lister(tree_key_t *key_list, int position, int limit)
{
	int i = 0;
	while(position < limit)
	{
		printf("%d. %s\n", i, ((char *)key_list[position].p));
		++position;
		++i;
	}
}

bool list_menu(tree_t *db, tree_key_t *key_list, int position, bool edit, int *chosen_key)
{
	int local_item_choice = -1;
	print_list_db();
	char choice = ask_question_list_db();
	switch(choice)
	{
		case 'L':           // Visa vara
			while(local_item_choice < 0 || local_item_choice > tree_size(db))
			{
				local_item_choice = ask_question_int("Vilken vara vill du välja?\n") + position;
			}
			if(edit)
			{
				*chosen_key = local_item_choice;
				return true;
			}
			else
			{
				elem_t chosen;
				tree_get(db, key_list[local_item_choice], &chosen);
				print_item(chosen);
				return true;
			}

		case 'A':           // Avbryt
			return true;

		case'V':            // Visa nästa 20
			return false;;
	}
	return false;
}

int list_database(tree_t *db, bool edit)
{
	int page_size = 5;
	int position = 0;
	int limit = page_size;
	int chosen_key;
	tree_key_t *key_list = tree_keys(db);
	while(position < tree_size(db))
	{
		page_lister(key_list, position, limit);

		if(list_menu(db, key_list, position, edit, &chosen_key))
		{
			free(key_list);
			return chosen_key;
		}

		else
		{
			position += page_size;
			limit += page_size;

			if(limit > tree_size(db))
			{
				limit += tree_size(db)-limit;
			}
		}
	}
	free(key_list);
	return -1;
}

//////////// ================= ITEM EDIT
///
/// Functions for editing items in database
///
//------------------------Remove----------------------
void remove_item_from_db(tree_t *db)
{
	elem_t result = {};
	int nr_to_remove = list_database(db, true);
	printf("%d\n", nr_to_remove);
	tree_key_t *key_list = tree_keys(db);

	elem_t tmp = {};
	tree_get(db, key_list[nr_to_remove], &tmp);
	print_specific(tmp);
	tree_remove(db, key_list[nr_to_remove], &result);
	tree_apply(db, preorder, t_print_func, NULL);
	free(key_list);
}

//////////// ================= EVENT LOOPS
///
/// Handles menus
// -------------- Edit menu ---------------
void event_loop_edit(tree_t *db /*undo*/ )
{
  bool quit_v = true;
  int choise = list_database(db, true);
  tree_key_t *key_list = tree_keys(db);
  elem_t to_edit = {};
  tree_get(db, key_list[choise], &to_edit);
  free(key_list);

  //TODO: Undo
  while(quit_v)
    {
      char option = ask_question_menu_edit();
      switch(option)
        {
        case 'B' :
          puts("Inte implementerat!");
          break;

        case 'P' :
          puts("Inte implementerat!");
          break;

        case 'L' :
          puts("Inte implementerat!");
          break;

        case 'T' :
          puts("Inte implementerat!");
          break;

        case 'A' :
          quit_v = false;
          break;
        }
    }
}

// -------------- Main menu ---------------
void event_loop(tree_t *db)
{ // TODO: Ångra stuff
	bool quit_v = true;
	while(quit_v)
	{
		char option = ask_question_menu();
		switch(option)
		{
			case 'L' :                       // Add item
				input_item(db);
				break;

			case 'T' :                       // Delete
				remove_item_from_db(db);
				break;

			case 'R' :                       // Redigera
				puts("Inte implementerat!");
				break;

			case 'G' :                       // Undo
				puts("Inte implementerat!");
				break;

			case 'H' :                       // List DB
				list_database(db, false);
				break;

			case 'A' :                       // Quit
				quit_v = false;
				break;
		}
	}
}
int main(int argc, char *argv[])
{
	puts("Välkommen till database v2.0 av Erik och Mats\n\
=============================================\n");
	tree_t *db = tree_new(t_copy_func, t_free_key_func, t_free_elem_func, t_comp_func);

	puts("insert\n");

	direct_input(db);

	event_loop(db);

	puts("deleting tree\n");

	tree_delete(db, true, true);
	puts("exit");
	return 0;
}
