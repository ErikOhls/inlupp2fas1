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

void t_free_key_func(elem_t elem)
{
  if(elem.p != NULL)
    {
      free(elem.p);
    }
  else return;
}

void t_free_elem_func(elem_t elem)
{
  item_t *item = elem.p;
  if(elem.p != NULL)
    {

      if(((item_t*)elem.p)->list != NULL)
        {
          list_delete(item->list, true);
        }
      free(item->name);
      free(item->desc);
      free(elem.p);
    }
  else return;
}

int t_comp_func(elem_t elem, elem_t elem2)
{
	if(strcmp(((char*)elem.p), ((char*)elem2.p)) > 0)
    {
      return 2;
    }
	if(strcmp(((char*)elem.p), ((char*)elem2.p)) < 0)
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
	printf("Key = %s. Elem->name = %s\n" , ((char*)key.p), ((item_t*)elem.p)->name);
	return true;
}

bool check_shelf_existance(tree_key_t key, elem_t elem, void *data)
{
  elem_t tmp = *(elem_t *) data;
  int i = list_contains(((item_t*)elem.p)->list, tmp);
  if(i == -1)
    {
      return false;
    }
  else
    {
      return true;
    }
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
      free(((shelf_t*)elem.p)->shelf_name);
      free(elem.p);
    }
  else return;
}

int l_comp_func(elem_t elem, elem_t elem2)
{
	if(strcmp((((shelf_t*)elem.p)->shelf_name), ((shelf_t*)elem2.p)->shelf_name) > 0)
    {
      return 2;
    }
	if(strcmp((((shelf_t*)elem.p)->shelf_name), ((shelf_t*)elem2.p)->shelf_name) < 0)
    {
      return 1;
    }
	else
    {
      return 0;
    }
}

bool print_shelfs(elem_t elem, void *data)
{
  printf("Lagerhylla: %s\nAntal: %d\n", ((shelf_t*)elem.p)->shelf_name, ((shelf_t*)elem.p)->amount);
  return true;
}
// -------------- Display items ---------------
void print_item(elem_t elem)
{
  printf("Namn på vara:        %s\n", ((item_t*)elem.p)->name);
  printf("Beskrivning av vara: %s\n", ((item_t*)elem.p)->desc);
  printf("Pris på vara:        %d\n", ((item_t*)elem.p)->price);
  list_apply(((item_t*)elem.p)->list, print_shelfs, NULL);
}

/// TMP FOR TESTING
void print_specific(elem_t elem)
{
  printf("key = %s\n", ((item_t*)elem.p)->name);
}

//////////// ================= Menus
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

///// TEMPORARY FOR TESTING
void direct_input(tree_t *db)
{
  shelf_t *new_shelf = calloc(1, sizeof(shelf_t));
  new_shelf->shelf_name = "A10";
  new_shelf->amount = 100;

  shelf_t *new_subshelf1 = calloc(1, sizeof(shelf_t));
  new_subshelf1->shelf_name = "B10";
  new_subshelf1->amount = 100;

  item_t *new_item = calloc(1, sizeof(item_t));
  new_item->name = "test1";
  new_item->desc = "testdesc1";
  new_item->price = 100;

  elem_t list_elem = { .p = new_shelf };
  elem_t list_subelem2 = { .p = new_subshelf1 };
  new_item->list = list_new(l_copy_func, l_free_func, l_comp_func);
  list_append(new_item->list, list_elem);
  list_append(new_item->list, list_subelem2);

  elem_t elem1 = { .p = new_item };
  tree_key_t key1 = { .p = new_item->name };

  // ----- ITEM 2 -----

  shelf_t *new_shelf2 = calloc(1, sizeof(shelf_t));
  new_shelf2->shelf_name = "C10";
  new_shelf2->amount = 100;

  item_t *new_item2 = calloc(1, sizeof(item_t));
  new_item2->name = "test2";
  new_item2->desc = "test2";
  new_item2->price = 100;

  elem_t list_elem2 = { .p = new_shelf2 };
  new_item2->list = list_new(l_copy_func, l_free_func, l_comp_func);
  list_append(new_item2->list, list_elem2);;

  elem_t elem2 = { .p = new_item2 };
  tree_key_t key2 = { .p = new_item2->name };

  // ----- ITEM 3 -----

  shelf_t *new_shelf3 = calloc(1, sizeof(shelf_t));
  new_shelf3->shelf_name = "D10";
  new_shelf3->amount = 100;

  item_t *new_item3 = calloc(1, sizeof(item_t));
  new_item3->name = "test3";
  new_item3->desc = "test3";
  new_item3->price = 100;

  elem_t list_elem3 = { .p = new_shelf3 };
  new_item3->list = list_new(l_copy_func, l_free_func, l_comp_func);
  list_append(new_item3->list, list_elem3);;

  elem_t elem3 = { .p = new_item3 };
  tree_key_t key3 = { .p = new_item3->name };

  // ----- ITEM 4 -----

  shelf_t *new_shelf4 = calloc(1, sizeof(shelf_t));
  new_shelf4->shelf_name = "E10";
  new_shelf4->amount = 100;

  item_t *new_item4 = calloc(1, sizeof(item_t));
  new_item4->name = "test4";
  new_item4->desc = "test4";
  new_item4->price = 100;

  elem_t list_elem4 = { .p = new_shelf4 };
  new_item4->list = list_new(l_copy_func, l_free_func, l_comp_func);
  list_append(new_item4->list, list_elem4);;

  elem_t elem4 = { .p = new_item4 };
  tree_key_t key4 = { .p = new_item4->name };

  // ----- ITEM 5 -----

  shelf_t *new_shelf5 = calloc(1, sizeof(shelf_t));
  new_shelf5->shelf_name = "F10";
  new_shelf5->amount = 100;

  item_t *new_item5 = calloc(1, sizeof(item_t));
  new_item5->name = "test5";
  new_item5->desc = "test5";
  new_item5->price = 100;

  elem_t list_elem5 = { .p = new_shelf5 };
  new_item5->list = list_new(l_copy_func, l_free_func, l_comp_func);
  list_append(new_item5->list, list_elem5);;

  elem_t elem5 = { .p = new_item5 };
  tree_key_t key5 = { .p = new_item5->name };

  // ----- ITEM 6 -----

  shelf_t *new_shelf6 = calloc(1, sizeof(shelf_t));
  new_shelf6->shelf_name = "G10";
  new_shelf6->amount = 100;

  item_t *new_item6 = calloc(1, sizeof(item_t));
  new_item6->name = "test6";
  new_item6->desc = "test6";
  new_item6->price = 100;

  elem_t list_elem6 = { .p = new_shelf6 };
  new_item6->list = list_new(l_copy_func, l_free_func, l_comp_func);
  list_append(new_item6->list, list_elem6);;

  elem_t elem6 = { .p = new_item6 };
  tree_key_t key6 = { .p = new_item6->name };

  // ----- ITEM 7 -----

  shelf_t *new_shelf7 = calloc(1, sizeof(shelf_t));
  new_shelf7->shelf_name = "P10";
  new_shelf7->amount = 100;

  item_t *new_item7 = calloc(1, sizeof(item_t));
  new_item7->name = "test7";
  new_item7->desc = "test7";
  new_item7->price = 100;

  elem_t list_elem7 = { .p = new_shelf7 };
  new_item7->list = list_new(l_copy_func, l_free_func, l_comp_func);
  list_append(new_item7->list, list_elem7);;

  elem_t elem7 = { .p = new_item7 };
  tree_key_t key7 = { .p = new_item7->name };

  // ----- ITEM 8 -----

  shelf_t *new_shelf8 = calloc(1, sizeof(shelf_t));
  new_shelf8->shelf_name = "W10";
  new_shelf8->amount = 100;

  item_t *new_item8 = calloc(1, sizeof(item_t));
  new_item8->name = "test8";
  new_item8->desc = "test8";
  new_item8->price = 100;

  elem_t list_elem8 = { .p = new_shelf8 };
  new_item8->list = list_new(l_copy_func, l_free_func, l_comp_func);
  list_append(new_item8->list, list_elem8);;

  elem_t elem8 = { .p = new_item8 };
  tree_key_t key8 = { .p = new_item8->name };

  puts("inserting to tree\n");

  tree_insert(db, key1, elem1);
  tree_insert(db, key2, elem2);
  tree_insert(db, key3, elem3);
  tree_insert(db, key4, elem4);
  tree_insert(db, key5, elem5);
  tree_insert(db, key6, elem6);
  tree_insert(db, key7, elem7);
  tree_insert(db, key8, elem8);

  puts("printing tree preorder\n");

  tree_apply(db, preorder, t_print_func, NULL);

  free(new_shelf);
  free(new_item);
  free(new_subshelf1);
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
