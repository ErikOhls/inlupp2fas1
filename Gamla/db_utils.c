#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "utils.h"
#include "list.h"
#include "tree.h"
#include "db_utils.h"

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

//////////// ================= Print functions
///
/// Functions for printing items and item data
///
void print_item(item_t* item)
{
  printf("Namn: %s\n", item->name);
  printf("Beskrivning: %s\n", item->desc);
  printf("Pris: %d\n", item->price);
  list_apply(item->list, print_amounts, NULL);
}

void print_tree(K key, T elem, void *data)
{
  printf("%s\n", (char *)key);
}


void print_shelfs(void *elem, void *data)
{
  printf("%s\n", ((rack_t *)elem)->shelf); // ((rack_t *)elem) = typdefintion
  return;
}

void print_amounts(void *elem, void *data)
{
  printf("Lagerhylla: %s\nAntal: %d\n", ((rack_t *)elem)->shelf, ((rack_t *)elem)->amount);
  return;
}

// -------------- Change shelf/amount ---------------

void shelf_exist(void *elem, void *exist)
{
  if(strcmp(((rack_t *)elem)->shelf, exist) == 0)
    {
      strcpy(exist, "trueeeeeeeeeeeeeeeeee");
    }
}

void change_shelf(void *elem, void *exist)
{
  if(strcmp(((rack_t *)elem)->shelf, exist) == 0)
    {
      printf("Byter hylla: %s\n", ((rack_t *)elem)->shelf);
      puts("--------------------------------\n");
      ((rack_t *)elem)->shelf = ask_question_shelf("Ny hylla: ");
      strcpy(exist, "trueeeeeeeeeeeeeeeeee");// Detta för att vi ska veta att hyllan hittats.
    }
  return;
}

void change_amount(void *elem, void *exist)
{
  if(strcmp(((rack_t *)elem)->shelf, exist) == 0)
    {
      printf("Byter hylla: %s. Med antal: %d\n", ((rack_t *)elem)->shelf, ((rack_t *)elem)->amount);
      puts("--------------------------------\n");
      ((rack_t *)elem)->amount = ask_question_int("Nytt antal: ");
      strcpy(exist, "trueeeeeeeeeeeeeeeeee");// Detta för att vi ska veta att hyllan hittats.
    }
  return;
}

//////////// ================= AUX
///
/// Auxilary functions
//
bool shelf_existance(tree_t *db, char *shelf_name)
{
  T *elem_list = tree_elements(db);
  for(int i = 0; i < tree_size(db); i++)
    {
      item_t *current = elem_list[i];
      list_t *current_list = current->list;
      char *shelf_tmp = strdup(shelf_name);
      list_apply(current_list, shelf_exist, shelf_tmp);
      if(strlen(shelf_tmp) > 10)
        {
          return true;
        }
    }
  return false;
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
  return true_c;
}

char ask_question_menu(void)
{
  print_menu();
  char *c = ask_question("Input: ", is_menu_char, (convert_func) strdup).s;
  char true_c;
  true_c = c[0];
  true_c = toupper(true_c);
  return true_c;
}

char ask_question_menu_edit(void)
{
  print_edit();
  char *c = ask_question("Input: ", is_menu_edit_char, (convert_func) strdup).s;
  char true_c;
  true_c = c[0];
  true_c = toupper(true_c);
  return true_c;
}

//////////// ================= MAKE ITEMS
///
/// Functions for making and adding items to db
///
item_t *make_item(tree_t *db, char *nm, char *dsc, int prc, char *slf, int amnt)
{
  rack_t *shlf = calloc(1, sizeof(rack_t));
  shlf->shelf = slf;
  shlf->amount = amnt;

  item_t *itm = calloc(1, sizeof(item_t));
  itm->name = nm;
  itm->desc = dsc;
  itm->price = prc;

  list_t *list = list_new();
  itm->list = list;
  list_append(itm->list, shlf);


  if(shelf_existance(db, slf))
    {
      printf("Vara finns redan på hylla %s, pröva igen\n", slf);
      itm->name = "AVBRYT";
      return itm;
    }

  if(tree_has_key(db, itm->name))                 // Om item finns
    {
      item_t *existing = tree_get(db, itm->name); // Hämta item
      // if(shelf exist)
      //     existing amount += item amount
      // else
      list_append(existing->list, shlf);          // Och appenda shelf
    }

  return itm;
}

item_t *input_item(tree_t *db)
{
  char *name  = ask_question_string("Varans namn: ");
  char *desc  = ask_question_string("Beskrivning av vara: ");
  int price   = ask_question_int("Varans pris: ");
  char *shelf = ask_question_shelf("Varans hylla: ");
  int amount  = ask_question_int("Varans antal: ");

  return make_item(db, name, desc, price, shelf, amount);
}

void edit_desc(tree_t *db, item_t *item)
{
  printf("Nuvarande beskrivning: %s\n", item->desc);
  puts("--------------------------------\n");
  item->desc = ask_question_string("Ny beskrivning: ");
  puts("Uppdaterad vara:\n");
  print_item(item);
  return;
}

void edit_price(tree_t *db, item_t *item)
{
  printf("Nuvarande pris: %d\n", item->price);
  puts("--------------------------------\n");
  item->price = ask_question_int("Nytt pris: ");
  puts("Uppdaterad vara:\n");
  print_item(item);
  return;
}

void edit_shelf(tree_t *db, item_t *item)
{
  puts("Nuvarande hyllor: ");
  list_apply(item->list, print_shelfs, NULL); // Prints shelfs
  puts("--------------------------------\n");
  bool has_shelf = true;
  while(has_shelf)
    {
      char *shelf_edit = ask_question_string("Vilken hylla vill du ändra?(case sensitive): ");
      list_apply(item->list, change_shelf, shelf_edit); // Ändrar shelf, om den finns.
      if(strlen(shelf_edit) > 10) // change_shelf ändrar shelf edit till lång sträng.
        {
          has_shelf = false;
        }
    }
  puts("Uppdaterad vara:\n");
  print_item(item);
  return;
}

void edit_amount(tree_t *db, item_t *item)
{
  puts("Nuvarande hyllor och antal: ");
  list_apply(item->list, print_amounts, NULL);
  puts("--------------------------------\n");
  bool has_shelf = true;
  while(has_shelf)
    {
      char *shelf_edit = ask_question_string("Vilken hylla vill du ändra?(case sensitive): ");
      list_apply(item->list, change_amount, shelf_edit);
      if(strlen(shelf_edit) > 10) // change_shelf ändrar shelf edit till lång sträng.
        {
          has_shelf = false;
        }
    }
  puts("Uppdaterad vara:\n");
  print_item(item);
  return;
}
//////////// ================= REMOVE ITEM
///
/// TODO: Inlupp 2
///
void remove_item_from_db(tree_t *db)
{
  // TODO: Först i inlupp 2.
  return;
}
