#include "utils.h"
#include "tree.h"
#include "list.h"
#include "common.h"

//////////// ================= STRUCTS
///
/// Structs for items and shelfs
///
typedef struct shelf{
  char *shelf_name;
  int   amount;
} shelf_t;

typedef struct item{
  char *  name;
  char *  desc;
  int     price;
  list_t *list;
} item_t;

//////////// ================= DATASTRUCTURE FUNCTIONS
///
/// Functions for handling datastructures
///
// -------------- Tree ---------------
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
  if(elem.p != NULL)
    {
      /*  TODO: Segfualt
      if(((item_t*)elem.p)->list != NULL)
        {
          list_delete(((item_t*)elem.p)->list, true);
        }
      */
      free(elem.p);
    }
  else return;
}

int t_comp_func(elem_t elem, elem_t elem2)
{
	if(strcmp(((char*)elem.p), ((char*)elem2.p)) < 0)
    {
      return 2;
    }
	if(strcmp(((char*)elem.p), ((char*)elem2.p)) > 0)
    {
      return 1;
    }
	else
    {
      return 0;
    }
}

bool t_print_func(tree_key_t key, elem_t elem, void *data)
{
  puts("t_print");
	printf("key = %s", ((char*)key.p));
	return true;
}

// -------------- List ---------------
elem_t l_copy_func(elem_t elem)
{
	item_t *from = elem.p;
	item_t *to = calloc(1, sizeof(item_t));

	*to = *from;

	elem_t result;
	result.p = to;
	return result;
}

void l_free_func(elem_t elem)
{
  if(elem.p != NULL)
    {
      free(elem.p);
    }
  else return;
}

int l_comp_func(elem_t elem, elem_t elem2)
{
	if(strcmp((((shelf_t*)elem.p)->shelf_name), ((shelf_t*)elem2.p)->shelf_name) < 0)
    {
      return 2;
    }
	if(strcmp((((shelf_t*)elem.p)->shelf_name), ((shelf_t*)elem2.p)->shelf_name) > 0)
    {
      return 1;
    }
	else
    {
      return 0;
    }
}

//////////// ================= Print menus
///
/// Functions for printing menus
///
void print_menu(void)
{
  puts("\n[L]ägga till en vara\n\
[T]a bort en vara\n\
[R]edigera en vara\n\
Ån[g]ra senaste ändringen\n\
Lista [h]ela varukatalogen\n\
[A]vsluta\n");
}

void print_edit(void)
{
  puts("\n[B]eskrivning\n\
[P]ris\n\
[L]agerhylla\n\
An[t]al\n\
\nVälj rad eller [a]vbryt: \n");
}

void print_list_db(void)
{
  puts("\n[V]isa nästa 20 varor\n\
Vä[l]j vara\n\
[A]vbryt\n");
}

//////////// ================= ASK QUESTIONS
///
/// Functions for converting input to useable variables
///
char ask_question_list_db(void)
{
  char *c = ask_question("Input: ", is_list_db_char, (convert_func) strdup).s;
  char true_c;
  true_c = c[0];
  true_c = toupper(true_c);
  free(c);
  return true_c;
}

char ask_question_menu(void)
{
  print_menu();
  char *c = ask_question("Input: ", is_menu_char, (convert_func) strdup).s;
  char true_c;
  true_c = c[0];
  true_c = toupper(true_c);
  free(c);
  return true_c;
}

char ask_question_menu_edit(void)
{
  print_edit();
  char *c = ask_question("Input: ", is_menu_edit_char, (convert_func) strdup).s;
  char true_c;
  true_c = c[0];
  true_c = toupper(true_c);
  free(c);
  return true_c;
}

//////////// ================= ITEM INPUT
///
/// Functions for inputing items to db
///
void add_item_to_db(tree_t *db, char *name, char *desc, int price, char *shelf_name, int amount)
{
  //TODO:
  //If item exists, append list of item and do not insert

  //If shelf exists, avbryt

  //If shelf AND item exists, shelf->amount += amount

  return;
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

///// TEMPORARY FOR TESTING
void direct_input(tree_t *db)
{
  puts("making shelf\n");
  shelf_t *new_shelf = calloc(1, sizeof(shelf_t));
  new_shelf->shelf_name = "A10";
  new_shelf->amount = 100;

  shelf_t *new_shelf2 = calloc(1, sizeof(shelf_t));
  new_shelf->shelf_name = "B10";
  new_shelf->amount = 100;

  shelf_t *new_shelf3 = calloc(1, sizeof(shelf_t));
  new_shelf->shelf_name = "C10";
  new_shelf->amount = 100;

  shelf_t *new_shelf4 = calloc(1, sizeof(shelf_t));
  new_shelf->shelf_name = "D10";
  new_shelf->amount = 100;

  shelf_t *new_shelf5 = calloc(1, sizeof(shelf_t));
  new_shelf->shelf_name = "E10";
  new_shelf->amount = 100;

  shelf_t *new_shelf6 = calloc(1, sizeof(shelf_t));
  new_shelf->shelf_name = "F10";
  new_shelf->amount = 100;

  shelf_t *new_shelf7 = calloc(1, sizeof(shelf_t));
  new_shelf->shelf_name = "G10";
  new_shelf->amount = 100;

  shelf_t *new_shelf8 = calloc(1, sizeof(shelf_t));
  new_shelf->shelf_name = "H10";
  new_shelf->amount = 100;

  shelf_t *new_shelf9 = calloc(1, sizeof(shelf_t));
  new_shelf->shelf_name = "J10";
  new_shelf->amount = 100;

  puts("making item\n");
  item_t *new_item = calloc(1, sizeof(item_t));
  new_item->name = "test1";
  new_item->desc = "test1";
  new_item->price = 100;

  item_t *new_item2 = calloc(1, sizeof(item_t));
  new_item->name = "test2";
  new_item->desc = "test2";
  new_item->price = 100;

  item_t *new_item3 = calloc(1, sizeof(item_t));
  new_item->name = "test3";
  new_item->desc = "test3";
  new_item->price = 100;

  item_t *new_item4 = calloc(1, sizeof(item_t));
  new_item->name = "test4";
  new_item->desc = "test4";
  new_item->price = 100;

  item_t *new_item5 = calloc(1, sizeof(item_t));
  new_item->name = "test5";
  new_item->desc = "test5";
  new_item->price = 100;

  item_t *new_item6 = calloc(1, sizeof(item_t));
  new_item->name = "test6";
  new_item->desc = "test6";
  new_item->price = 100;

  item_t *new_item7 = calloc(1, sizeof(item_t));
  new_item->name = "test7";
  new_item->desc = "test7";
  new_item->price = 100;

  item_t *new_item8 = calloc(1, sizeof(item_t));
  new_item->name = "test8";
  new_item->desc = "test8";
  new_item->price = 100;


  puts("making list\n");
  elem_t list_elem = { .p = new_shelf };
  elem_t list_elem2 = { .p = new_shelf2 };
  new_item->list = list_new(l_copy_func, l_free_func, l_comp_func);
  list_append(new_item->list, list_elem);
  list_append(new_item->list, list_elem2);

  elem_t list_elem3 = { .p = new_shelf3 };
  new_item2->list = list_new(l_copy_func, l_free_func, l_comp_func);
  list_append(new_item2->list, list_elem3);

  elem_t list_elem4 = { .p = new_shelf4 };
  new_item3->list = list_new(l_copy_func, l_free_func, l_comp_func);
  list_append(new_item3->list, list_elem4);

  elem_t list_elem5 = { .p = new_shelf5 };
  new_item4->list = list_new(l_copy_func, l_free_func, l_comp_func);
  list_append(new_item4->list, list_elem5);

  elem_t list_elem6 = { .p = new_shelf6 };
  new_item5->list = list_new(l_copy_func, l_free_func, l_comp_func);
  list_append(new_item5->list, list_elem6);

  elem_t list_elem7 = { .p = new_shelf7 };
  new_item6->list = list_new(l_copy_func, l_free_func, l_comp_func);
  list_append(new_item6->list, list_elem7);

  elem_t list_elem8 = { .p = new_shelf8 };
  new_item7->list = list_new(l_copy_func, l_free_func, l_comp_func);
  list_append(new_item7->list, list_elem8);

  elem_t list_elem9 = { .p = new_shelf9 };
  new_item8->list = list_new(l_copy_func, l_free_func, l_comp_func);
  list_append(new_item8->list, list_elem9);

  elem_t elem1 = { .p = new_item };
  tree_key_t key1 = { .p = new_item->name };

  elem_t elem2 = { .p = new_item2 };
  tree_key_t key2 = { .p = new_item2->name };

  elem_t elem3 = { .p = new_item3 };
  tree_key_t key3 = { .p = new_item3->name };

  elem_t elem4 = { .p = new_item4 };
  tree_key_t key4 = { .p = new_item4->name };

  elem_t elem5 = { .p = new_item5 };
  tree_key_t key5 = { .p = new_item5->name };

  elem_t elem6 = { .p = new_item6 };
  tree_key_t key6 = { .p = new_item6->name };

  elem_t elem7 = { .p = new_item7 };
  tree_key_t key7 = { .p = new_item7->name };

  elem_t elem8 = { .p = new_item8 };
  tree_key_t key8 = { .p = new_item8->name };

  puts("inserting to tree\n");

  tree_insert(db, key1, elem1);
  puts("insert2\n");
  tree_insert(db, key2, elem2);
  puts("insert 3\n");
  tree_insert(db, key3, elem3);
  tree_insert(db, key4, elem4);
  tree_insert(db, key5, elem5);
  tree_insert(db, key6, elem6);
  tree_insert(db, key7, elem7);
  puts("insert8\n");
  tree_insert(db, key8, elem8);
 
  puts("printing tree inorder\n");

  tree_apply(db, inorder, t_print_func, NULL);

  free(new_shelf);
  free(new_item);
  free(new_shelf2);
  free(new_item2);
  free(new_shelf3);
  free(new_item3);
  free(new_shelf4);
  free(new_item4);
  free(new_shelf5);
  free(new_item5);
  free(new_shelf6);
  free(new_item6);
  free(new_shelf7);
  free(new_item7);
  free(new_shelf8);
  free(new_item8);

}

//////////// ================= LIST DB
///
/// Functions for listing the database
///
void list_db(tree_t *db)
{
  tree_key_t *key_list = tree_keys(db);
  //TODO: list that shit!
  free(key_list);
  return;
}

//////////// ================= EVENT LOOPS
///
/// Handles menus
// -------------- Edit menu ---------------
void event_loop_edit(tree_t *db /*undo*/ )
{
  bool quit_v = true;
  //item_t *chosen_item = någon funktion
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
          puts("Inte implementerat!");
          break;

        case 'R' :                       // Redigera
          puts("Inte implementerat!");
          break;

        case 'G' :                       // Undo
          puts("Inte implementerat!");
          break;

        case 'H' :                       // List DB
          puts("Inte implementerat!");
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
==================================================\n");
  tree_t *db = tree_new(t_copy_func, t_free_func, t_free_func, t_comp_func);

  puts("insert\n");

  direct_input(db);

  event_loop(db);

  puts("deleting tree\n");

  tree_delete(db, true, true);
  puts("exit");
  return 0;
}
