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
elem_t t_copy_func(elem_t elem);
void t_free_key_func(elem_t elem);
void t_free_elem_func(elem_t elem);
int t_comp_func(elem_t elem, elem_t elem2);
bool t_print_func(tree_key_t key, elem_t elem, void *data);
bool check_shelf_existance(tree_key_t key, elem_t elem, void *data);

// -------------- List ---------------
elem_t l_copy_func(elem_t elem);
void l_free_func(elem_t elem);
int l_comp_func(elem_t elem, elem_t elem2);
bool print_shelfs(elem_t elem, void *data);

// -------------- Display items ---------------
void print_item(elem_t elem);

/// TMP FOR TESTING
void print_specific(elem_t elem);

//////////// ================= Menus
///
/// Functions for printing menus
///
void print_menu(void);
void print_edit(void);
void print_list_db(void);

//////////// ================= ASK QUESTIONS
///
/// Functions for converting input to useable variables
///
char ask_question_list_db(void);
char ask_question_menu(void);
char ask_question_menu_edit(void);

///// TEMPORARY FOR TESTING
void direct_input(tree_t *db);
