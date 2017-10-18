#include "list.h"
#include "tree.h"


struct rack
{
  char *shelf;
  int amount;
};

typedef struct rack rack_t;

struct item
{
  char *name;
  char *desc;
  int price;
  list_t *list;
};

typedef struct item item_t;

struct action
{
  int type; // NOTHING = 0, ADD = 1, REMOVE = 2, EDIT = 3
  item_t *merch;
  item_t copy;
};

typedef struct action action_t;

//////////// ================= Print menus
///
/// Functions for printing menus
///

//// Prints main menu
//
void print_menu(void);

//// Prints edit menu
//
void print_edit(void);

//// Prints list menu
//
void print_list_db(void);

//////////// ================= Print functions
///
/// Functions for printing items and item data
///

//// Prints a item with all specifications
// \param item_t*. Item to be printed.
//
void print_item(item_t* item);

//// Prints a key in a tree
// \param key. Key to be printed
// \param elem. Unused
// \param data. Unused
//
void print_tree(K key, T elem, void *data);

//// Prints a shelf in a list of shelfs.
// \param elem. Elem to be printed
// \param data. Unused
//
void print_shelfs(void *elem, void *data);

//// Prints a shelf and associated amount
// \param elem. Elem to be printed
// \param data. Unused
void print_amounts(void *elem, void *data);

// -------- Change shelf/amount ----------

//// Checks whether a shelf exists in a list
//  \param elem. Existing shelf
//  \param exist. Name of shelf to check existing of
//
void shelf_exist(void *elem, void *exist);

//// Changes a shelf name
//   \param elem. Existing shelf
//   \param exisit. Shelf to change
//
void change_shelf(void *elem, void *exist);

//// Changes an amount on a shelf
//  \param elem. Existing shelf.
//  \param exists. Shelf to change
//
void change_amount(void *elem, void *exist);

//// Check whether a shelf exists within the whole tree
//  \param tree_t*. Three where shelf exists.
//  \param char*. Shelf to check existence of.
//
bool shelf_existance(tree_t *db, char *shelf_name);

//////////// ================= ASK QUESTIONS
///
/// Functions for converting input to useable variables
///

//// Asks user for menu choice in list menu
// returns valid menu choice
char ask_question_list_db();

//// Asks user for menu choice in main menu
// return valid menu choice
char ask_question_menu();

//// Asks user for menu choice in edit menu
//  returns valid menu choice
char ask_question_menu_edit();

//////////// ================= MAKE ITEMS
///
/// Functions for making and adding items to db
///

//// Makes item from input params
// \param tree_t*. Tree containing database
// \param char *. String of name, description, shelf.
// \param int. Integer of price and amount
item_t *make_item(tree_t *db, char *nm, char *dsc, int prc, char *slf, int amnt);

//// Creates item specifications from user input
//   \param tree_t*. The three where to item belongs.
//   returns a complete item.
//
item_t *input_item(tree_t *db);

//// Changes a items description
//  \param tree_t*. Tree item is stored in
//  \param item_t*. Item to be edited
void edit_desc(tree_t *db, item_t *item);

//// Changes a items price
/// \param tree_t*. Tree item is stored in
//  \param item_t*. Item to be edited
void edit_price(tree_t *db, item_t *item);

//// Changes a items shelf
//  \param tree_t*. Tree item is stored in
//  \param item_t*. Item to be edited
void edit_shelf(tree_t *db, item_t *item);

//// Changes a items shelf amount
//  \param tree_t*. Tree item is stored in
//  \param item_t*. Item to be edited
void edit_amount(tree_t *db, item_t *item);


//// Ej implementerat
//
void remove_item_from_db(tree_t *db);
