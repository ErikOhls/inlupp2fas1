#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "utils.h"
#include "list.h"
#include "db_utils.h"



//////////// ================= STRUCTS
///
/// Structs for items and racks
/// TODO: Ångra
typedef struct rack rack_t;
typedef struct item item_t;
typedef struct action action_t;


void add_item_to_db(tree_t *db)
{
  item_t *item = input_item(db);     // Gör nytt item
  if(strcmp("AVBRYT", item->name) == 0)
    {
      return;
    }
  if(tree_has_key(db, item->name))   // Om item finns, gör inget(för list_append sker i input)
    {
      printf("Vara %s finns redan", item->name);
      return;
    }
  else                               // Annars inserta item
    {
      tree_insert(db, item->name, item);
      return;
    }
}


//////////// ================= LIST DB
///
/// Funtion to display the database and specific items in the database
///
void list_db(tree_t *db)
{
  K *key_list = tree_keys(db);
  int position = 0;               // Riktiga positionen i arrayen
  int page = 20;                  // Limiter för antal visade varor
  int page_ind = 0;
  int ind = -1;
  while(position < tree_size(db))
    {
      int i = 1;
      while(position < page)
        {
          printf("%d. %s\n", i, key_list[position]);
          i++;
          position++;
        }
      i = 1;                      // Återställ visat index

      print_list_db();

      char answer_id = ask_question_list_db();
      switch(answer_id)
        {
        case 'L':
          do
            {
              while(ind < 0 || ind > tree_size(db))
                {
                  ind = ask_question_int("Vilken vara vill du välja?: ") + page_ind-1;
                }
            }
          while(ind >= tree_size(db));
          item_t *my_elem = tree_get(db, key_list[ind]);
          print_item(my_elem);
          return;
        case 'A':
          return;
        case 'V':
          page += 20;                 // Incrementera en page
          page_ind +=20;
          break;
        }
      if(page > tree_size(db))    // Minskar page för att inte överskrida array size.
        {
          page += tree_size(db)-page;
        }
    }
  puts("Listan är slut!");
  return;
}

//////////// ================= CHOOSE ITEM
///
/// Function to display the database, and choose an item to edit.
///
item_t *choose_list_db(tree_t *db)
{
  K *key_list = tree_keys(db);
  int position = 0;               // Riktiga positionen i arrayen
  int page = 20;                  // Limiter för antal visade varor
  int page_ind = 0;
  int ind;
  item_t *false_item = calloc(1, sizeof(item_t));
  false_item->name = "AVBRYT";     // Fulhax för att kunna returna i event_loop_edit
  while(position < tree_size(db))
    {
      int i = 1;
      while(position < page)
        {
          printf("%d. %s\n", i, key_list[position]);
          i++;
          position++;
        }
      i = 1;                      // Återställ visat index

      print_list_db();

      char answer_id = ask_question_list_db();
      switch(answer_id)
        {
        case 'L':
          do
            {
              ind = ask_question_int("Vilken vara vill du välja?: ") + page_ind-1;
              puts("printing number");
              printf("%d", ind);
            }
          while(ind >= tree_size(db));
          return tree_get(db, key_list[ind]);

        case 'A':
          return false_item;
        case 'V':
          page += 20;                 // Incrementera en page
          page_ind +=20;
          break;
        }
      if(page > tree_size(db))    // Minskar page för att inte överskrida array size.
        {
          page += tree_size(db)-page;
        }
    }
  return false_item;
}


//////////// ================= EVENT LOOPS
///
/// Handles menus
///
void event_loop_edit(tree_t *db, action_t *undo)
{
  bool quit_v = true;
  item_t *chosen_item = choose_list_db(db);
  undo->copy = *chosen_item;
  undo->merch = chosen_item;
  if(strcmp(chosen_item->name, "AVBRYT") == 0) // Avbryt valdes i choose_list_db
    {
      return;
    }
  while(quit_v)
        {
          char option = ask_question_menu_edit();
          switch(option)
            {
            case 'B' :
              edit_desc(db, chosen_item);
              break;

            case 'P' :
              edit_price(db, chosen_item);
              break;

            case 'L' :
              edit_shelf(db, chosen_item);
              break;

            case 'T' :
              edit_amount(db, chosen_item);
              break;

            case 'A' :
              quit_v = false;
              break;
            }
        }
}

void undo_func(action_t undo)
{
  if(undo.type == 0)
    {
      puts("Det finns inget att ångra");
    }
  if(undo.type == 1)
    {
      puts("Inte implementerat");
    }
  if(undo.type == 2)
    {
      puts("Inte implementerat");
    }
  if(undo.type == 3)
    {
      puts("Ångrade din senaste redigering");
      *undo.merch = undo.copy;
    }
}

void event_loop(tree_t *db)
{
  bool quit_v = true;
  action_t undo = {.type = 0};
  while(quit_v)
        {
          char option = ask_question_menu();
          switch(option)
            {
            case 'L' :
              undo.type = 1;
              add_item_to_db(db);
              break;
            case 'T' :
              undo.type = 2;
              puts("Inte implementerat!");
              break;

            case 'R' :
              undo.type = 3;
              event_loop_edit(db, &undo);
              break;

            case 'G' :
              undo_func(undo);
              break;

            case 'H' :
              list_db(db);
              break;

            case 'A' :
              quit_v = false;
              break;
            }
        }
}

//////////// ================= MAIN
///
/// Does the shit
///
int main(int argc, char *argv[])
{
  puts("Välkommen till database v1.0 av Erik/Grim/Jonathan\n\
==================================================\n");
  tree_t *db = tree_new();
  tree_insert(db, "test1", make_item(db, "test1", "dsc1", 1000, "A10", 100));
  tree_insert(db, "test 2", make_item(db, "test 2", "dsc2", 1000, "B10", 100));
  tree_insert(db, "test 3", make_item(db, "test 3", "dsc3", 1000, "C10", 100));

  tree_insert(db, "Erik", make_item(db, "Erik", "Bäst av alla", 1000, "P20", 100));
  tree_insert(db, "Grim", make_item(db, "Grim", "Också bra", 1000, "R15", 100));
  tree_insert(db, "Jonathan", make_item(db, "Jonathan", "Stilig karl", 1000, "T50", 100));
  tree_insert(db, "Stol", make_item(db, "Stol", "dsc1", 1000, "Q65", 100));
  tree_insert(db, "Stol", make_item(db, "Stol", "dsc1", 1000, "Q70", 100));
  tree_insert(db, "Bord", make_item(db, "Bord", "dsc1", 1000, "L70", 100));

  tree_insert(db, "test 4", make_item(db, "test 4", "dsc4", 1000, "C20", 100));
  tree_insert(db, "test 5", make_item(db, "test 5", "dsc5", 1000, "C30", 100));
  tree_insert(db, "test 6", make_item(db, "test 6", "dsc6", 1000, "C40", 100));
  tree_insert(db, "test 7", make_item(db, "test 7", "dsc7", 1000, "C50", 100));
  tree_insert(db, "test 8", make_item(db, "test 8", "dsc8", 1000, "C60", 100));
  tree_insert(db, "test 9", make_item(db, "test 9", "dsc9", 1000, "C70", 100));

  tree_insert(db, "test 10", make_item(db, "test 10", "dsc3", 1000, "P10", 100));
  tree_insert(db, "test 11", make_item(db, "test 11", "dsc3", 1000, "D10", 100));
  tree_insert(db, "test 12", make_item(db, "test 12", "dsc3", 1000, "F10", 100));
  tree_insert(db, "test 13", make_item(db, "test 13", "dsc3", 1000, "T10", 100));
  tree_insert(db, "test 14", make_item(db, "test 14", "dsc3", 1000, "E10", 100));
  tree_insert(db, "test 15", make_item(db, "test 15", "dsc3", 1000, "Q10", 100));
  tree_insert(db, "test 16", make_item(db, "test 16", "dsc3", 1000, "W10", 100));
  tree_insert(db, "test 17", make_item(db, "test 17", "dsc3", 1000, "V10", 100));
  tree_insert(db, "test 18", make_item(db, "test 18", "dsc3", 1000, "H10", 100));
  tree_insert(db, "test 19", make_item(db, "test 19", "dsc3", 1000, "J10", 100));
  tree_insert(db, "test 20", make_item(db, "test 20", "dsc3", 1000, "K10", 100));
;
  event_loop(db);
  return 0;
}

