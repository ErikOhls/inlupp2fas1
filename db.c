#include "utils.h"
#include "tree.h"
#include "list.h"
#include "common.h"
#include "db_aux.h"
#include <stdlib.h>

///////////// ================= UNDO ACTION
//////////
///////
////
//

typedef struct action
{
	int type;
	item_t *merch;
	item_t *copy;
	shelf_t *shelf;

}action_t;

elem_t undo_edit_desc_aux(elem_t elem, action_t *latest_action)
{
	char *tmp = latest_action->copy->desc;
	((item_t*)elem.p)->desc = strdup(tmp);
	return elem;
}


void undo_edit_desc(elem_t *to_edit, action_t *latest_action)
{
	*to_edit = undo_edit_desc_aux(*to_edit, latest_action);
	return;
}

elem_t undo_edit_price_aux(elem_t elem, action_t *latest_action)
{
	((item_t*)elem.p)->price = latest_action->copy->price;
	return elem;
}

void undo_edit_price(elem_t *to_edit, action_t *latest_action)
{
	*to_edit = undo_edit_price_aux(*to_edit, latest_action);
}


void undo_action(tree_t *db, action_t *latest_action) // * till &
{
	switch(latest_action -> type)
	{
		case 0:
			{
				puts("Du har inget att Ângra\n");
			}
			break;

		case 1: //l‰gga till en vara sÂ vi tar bara bort shelfen. LA merch -> varan vi la till. bˆrja med att s‰tta type till 1 i l‰gga till.
			{
				elem_t elem;
				tree_key_t key = { .p = latest_action->copy->name };
				tree_remove(db, key, &elem); 
				//free(latest_action->copy->name);
				free(latest_action->copy);

			}
			break;


		case 2: //s‰tt gammla itemet i copy och ‰ndra latest_action type till 2. s‰tt LA tree-> tr‰det vi anv‰nder
			{

				elem_t elem;
				tree_key_t key = { .p = latest_action->copy->name };
				//	elem_t *to_reinsert = malloc(sizeof(elem_t));
				//	*to_reinsert = latest_action -> copy;
				//	tree_insert(latest_action->merch, to_reinsert->name, to_reinsert);
				tree_insert(db, key, elem);
				free(latest_action->copy);
			}
			break;

		case 3: //Redigera varans pris. s‰tt LA merch -> varan vi redigerade(nya redigerade varan) s‰tt latest_action -> amoun ‰r gammla priset sedan s‰tter vi LA till 3.
			{
				elem_t to_edit = {};
				tree_key_t key = { .p = latest_action->copy->name };
				tree_get(db, key, &to_edit);

				undo_edit_price(&to_edit, latest_action);
				//free(latest_action->copy->name);
				//free(latest_action->copy->price);
				free(latest_action->copy);
			}
			break;

		case 4: //Redigera varans beskrivning. s‰tt LA merch -> den nya redigerade varan. LA -> string ‰r gammla beskrivningen. sen LA type -> 4.
			{
				elem_t to_edit = {};
				tree_key_t key = { .p = latest_action->copy->name };
				tree_get(db, key, &to_edit);

				undo_edit_desc(&to_edit, latest_action);
				free(latest_action->copy->name);
				free(latest_action->copy->desc);
				free(latest_action->copy);
			}
			break;

	}
	latest_action->type = 0;
}
/*
   case 5: //ta bort en hylla. s‰tt LA merch -> nya redigerade varan. S‰tt LA location till den avrefrenserade sparar pointern i list remove. s‰tt LA type -> 5
   {
   list_append(get_item_locations(latest_action -> merch), latest_action -> location);
   }
   break;

   case 6: //l‰gga till en hylla, S‰tt LA merch -> den nya redigerade varan. vi l‰gger till de sist sÂ vi tar bara bort den sista. S‰tt LA type -> 6
   {
   location_t *toremove;
   list_remove(get_item_locations(latest_action->merch), -1,  &toremove);
   free(get_location_shelf(toremove));
   free(toremove);
   }
   break;

   case 7: //redigera en hyllas antal. s‰tt LA amount ska ha gammla antalet. LA location -> location vi modifierade- s‰tt LA 7
   {
   set_location_amount(latest_action->location, latest_action->amount);
   }
   break;

   case 8: //redigera en hyllas plats. LA location -> location vi redigera. LA string ska vara den gammla hyllplatsen. S‰tt LA type -> 8 
   {
   free(get_location_shelf(latest_action -> location));
   set_location_shelf(latest_action -> location, latest_action -> string);
   }
   break;
   }
   latest_action -> type = 0;
   }



*/

//////////// ================= ITEM INPUT
///
/// Functions for inputing items to db
///
//
//

void add_item_to_db(tree_t *db, char *name, char *desc, int price, char *shelf_name, int amount, action_t *latest_action)
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

	//Make list element
	elem_t list_elem = { .p = new_shelf };

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
		/*
		   free(name);
		   free(desc);
		   free(shelf_name);
		   */

		return;
	}

	else if(item_exists)
	{
		tree_key_t key = { .p = name };
		elem_t result = {};
		tree_get(db, key, &result);
		list_append(((item_t*)result.p)->list, list_elem);

		//TODO: undo funktioner
		free(new_shelf);
		free(new_item);
		/*
		   free(name);
		   free(desc);
		   free(shelf_name);
		   */

		return;
	}

	else if(shelf_exists)
	{
		puts("Varahyllan är redan tagen! Försök igen\n");
		free(new_shelf);
		free(new_item);
		/*
		   free(name);
		   free(desc);
		   free(shelf_name);
		   */

		return;
	}

	else
	{
		//Insert item
		new_item->list = list_new(l_copy_func, l_free_func, l_comp_func);
		list_append(new_item->list, list_elem);
		tree_insert(db, key, elem);

		//Make copy for undo
		latest_action->type = 1;
		item_t *tmp = calloc(1, sizeof(item_t)); // FREE!
		tmp->name = strdup(name);
		tmp->desc = desc;
		tmp->price = price;
		printf("tmp->name = %s\n", tmp->name);
		latest_action->copy = tmp;
		printf("latest_action->copy->name = %s\n", latest_action->copy->name);
		latest_action->shelf = new_shelf;
		//free(tmp);

		//Free callocs
		free(new_shelf);
		free(new_item);
		/*
		   free(name);
		   free(desc);
		   free(shelf_name);
		   */

		printf("latest_action->copy->name after free = %s\n", latest_action->copy->name);

		return;
	}
}

void input_item(tree_t *db, action_t *latest_action)
{
	char *name  = ask_question_string("Varans namn: ");
	char *desc  = ask_question_string("Beskrivning av vara: ");
	int price   = ask_question_int("Varans pris: ");
	char *shelf = ask_question_shelf("Varans hylla: ");
	int amount  = ask_question_int("Varans antal: ");

	add_item_to_db(db, name, desc, price, shelf, amount, latest_action);

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
// -------------- Remove ---------------
void remove_item_from_db(tree_t *db, action_t *latest_action)
{
	elem_t result = {};
	int nr_to_remove = list_database(db, true);
	printf("%d\n", nr_to_remove);
	tree_key_t *key_list = tree_keys(db);

	elem_t tmp = {};
	tree_get(db, key_list[nr_to_remove], &tmp);
	print_specific(tmp);

	latest_action->type = 2;
	item_t *save_item = calloc(1, sizeof(item_t)); // FREE!
	shelf_t *save_shelf = calloc(1, sizeof(shelf_t));

	save_item->name = strdup(((item_t*)tmp.p)->name);
	save_item->desc = strdup(((item_t*)tmp.p)->desc);
	save_item->price = ((item_t*)tmp.p)->price;
	/*
	char *s = ((shelf_t*)tmp.p)->shelf_name;
	printf("%s\n", s);

	save_shelf->shelf_name = strdup(((shelf_t*)tmp.p)->shelf_name);
	save_shelf->amount = ((shelf_t*)tmp.p)->amount;
	printf("save_item->name = %s\n", save_item->name);
	latest_action->copy = save_item;
	latest_action->shelf = save_shelf; 
	printf("latest_action->shelf->amount = %d\n", save_shelf->amount);
	//latest_action->shelf = new_shelf;
*/
	tree_remove(db, key_list[nr_to_remove], &result);
	tree_apply(db, preorder, t_print_func, NULL);


	free(key_list);
}

// -------------- edit desc ---------------
void print_desc(elem_t elem, action_t *latest_action)
{
	char *name = ((item_t*)elem.p)->name;
	char *desc = ((item_t*)elem.p)->desc;
	printf("Nuvarande beskrivning: %s\n", ((item_t*)elem.p)->name);
	latest_action->type = 4;
	item_t *tmp = calloc(1, sizeof(item_t));
	tmp->name = strdup(name);
	tmp->desc = strdup(desc);
	latest_action->copy = tmp;
}

elem_t edit_desc_aux(elem_t elem)
{
	char *tmp = ask_question_string("Ny beskrivning: ");
  free(((item_t*)elem.p)->desc);
	((item_t*)elem.p)->desc = tmp;
	return elem;
}

void edit_desc(elem_t *to_edit, action_t *latest_action)
{
	print_desc(*to_edit, latest_action);
	puts("--------------------------------\n");
	*to_edit = edit_desc_aux(*to_edit);
	puts("Uppdaterad vara:\n");
	print_item(*to_edit);
	return;
}

// -------------- edit price ---------------
void print_price(elem_t elem, action_t *latest_action)
{
	int price = ((item_t*)elem.p)->price;
	char *name = ((item_t*)elem.p)->name;
	printf("Nuvarande pris: %d\n", ((item_t*)elem.p)->price);
	latest_action->type = 3;
	item_t *tmp = calloc(1, sizeof(item_t));
	tmp->name = strdup(name);
	tmp->price = price;
	latest_action->copy = tmp;
	//TODO: free
}

elem_t edit_price_aux(elem_t elem)
{
	((item_t*)elem.p)->price = ask_question_int("Nytt pris: ");
	return elem;
}

void edit_price(elem_t *to_edit, action_t *latest_action)
{
	print_price(*to_edit, latest_action);
	puts("--------------------------------\n");
	*to_edit = edit_price_aux(*to_edit);
	puts("Uppdaterad vara:\n");
	print_item(*to_edit);
	return;
}

// -------------- edit shelf ---------------
void print_shelflist(elem_t elem)
{
	list_apply(((item_t*)elem.p)->list, print_shelfs, NULL);
}

bool change_shelf(elem_t original, void *new)
{
	elem_t tmp = *(elem_t *)new;
	if(strcmp(((shelf_t*)original.p)->shelf_name, ((shelf_t *)tmp.p)->shelf_name) == 0)
	{
		char *new_shelf_name = ask_question_shelf("Ny hylla: ");
		((shelf_t*)original.p)->shelf_name = new_shelf_name;
		free(new_shelf_name);
		return true;
	}
	return true;
}

void edit_shelf(tree_t *db, elem_t *to_edit)
{
	puts("Nuvarande hyllor: \n");
	puts("--------------------------------");
	print_shelflist(*to_edit);
	puts("--------------------------------\n");

	bool existance = false;
	while(!existance)
	{
		//Bestäm vilken hylla ändra
		char *new_shelf_name = ask_question_string("Vilken hylla vill du ändra?(case sensitive): ");

		//Kolla om hyllan finns
		shelf_t *shelf_tmp = calloc(1, sizeof(shelf_t));
		shelf_tmp->shelf_name = new_shelf_name;
		elem_t tmp_list_elem = { .p = shelf_tmp };
		existance = tree_apply(db, inorder, check_shelf_existance, &tmp_list_elem);

		//Ändra hyllan
		if(existance)
		{
			// Setup
			list_t *list = ((item_t*)to_edit->p)->list;
			int i = list_contains(list, tmp_list_elem);

			// Hitta och extrahera hylla
			shelf_t *old_shelf = calloc(1, sizeof(shelf_t));
			elem_t list_elem = { .p = old_shelf };
			list_get(list, i, &list_elem);
			int amount_copy = ((shelf_t*)list_elem.p)->amount;

			// Skapa ny hylla
			shelf_t *insert_shelf = calloc(1, sizeof(shelf_t));
			list_elem.p = insert_shelf;

			// Skapa värden på ny hylla
			char *new_shelf_name = ask_question_shelf("Ny hylla: "); //TODO: Läcker minne
			insert_shelf->shelf_name = new_shelf_name;
			insert_shelf->amount = amount_copy;

			// Fixa listan
			list_remove(list, i, true);
			list_append(list, list_elem);

			free(old_shelf);
			free(insert_shelf);
			//free(new_shelf_name);   //TODO: Men namnet försvinner om man kör free

		}
		else
		{
			puts("That shelf does not exist! Try again.\n");
		}
		free(new_shelf_name);
		free(shelf_tmp);
	}
	puts("Uppdaterad vara:\n");
	print_item(*to_edit);
	return;
}

// -------------- edit amount ---------------
bool change_amount(elem_t original, void *new)
{
	elem_t tmp = *(elem_t *)new;
	if(strcmp(((shelf_t*)original.p)->shelf_name, ((shelf_t *)tmp.p)->shelf_name) == 0)
	{
		printf("Nuvarande antal: %d", ((shelf_t*)original.p)->amount);
		((shelf_t*)original.p)->amount = ask_question_int("Nytt antal: \n");
		return true;
	}
	return true;
}


void edit_amount(tree_t* db, elem_t *to_edit)
{
	puts("Nuvarande hyllor: \n");
	puts("--------------------------------");
	print_shelflist(*to_edit);
	puts("--------------------------------\n");

	bool existance = false;
	while(!existance)
	{
		//Bestäm vilken hylla ändra
		char *new_shelf_name = ask_question_string("Vilken hylla vill du ändra?(case sensitive): ");

		//Kolla om hyllan finns
		shelf_t *shelf_tmp = calloc(1, sizeof(shelf_t));
		shelf_tmp->shelf_name = new_shelf_name;
		elem_t tmp_list_elem = { .p = shelf_tmp };
		existance = tree_apply(db, inorder, check_shelf_existance, &tmp_list_elem);

		//Ändra hyllan
		if(existance)
		{
			list_t *list = ((item_t*)to_edit->p)->list;
			list_apply(list, change_amount, &tmp_list_elem);
		}
		else
		{
			puts("That shelf does not exist! Try again.\n");
		}
		free(new_shelf_name);
		free(shelf_tmp);
	}
	puts("Uppdaterad vara:\n");
	print_item(*to_edit);
	return;
}
//////////// ================= PERSISTANCE
///
/// Functions for writing to and reading from text files
// -------------- Write ---------------
void save_shelfs(FILE *fptr, list_t *list)
{
	int nr_shelfs = list_length(list);        // Record nr of shelfs
	char tmp[255];
	snprintf(tmp, 255, "%d", nr_shelfs);
	fputs(tmp, fptr);
	fputs("\n", fptr);

	for(int i = 0; i < list_length(list); i++)
	{
		// Setup shelf to save
		elem_t shelf_to_save;
		list_get(list, i, &shelf_to_save);
		char *name = ((shelf_t*)shelf_to_save.p)->shelf_name;
		int iamount = ((shelf_t*)shelf_to_save.p)->amount;
		char amount[255];
		snprintf(amount, 255, "%d", iamount); // Convert int amount to char amount

		// Write to file
		fputs(name, fptr);
		fputs("\n", fptr);

		fputs(amount, fptr);
		fputs("\n", fptr);
	}
	//fputs("\n\n", fptr);
	return;
}

void save_item(elem_t *elem_list, FILE *fptr, int db_size)
{
	if(fptr)
	{
		//Convert and write db_size
		char db_size_tmp[255];
		snprintf(db_size_tmp, 255, "%d", db_size);
		fputs(db_size_tmp, fptr);
		fputs("\n", fptr);
		for(int i = 0; i < db_size; i++)
		{
			// Setup item to save
			elem_t item_to_save = elem_list[i];
			char *name = ((item_t*)item_to_save.p)->name;
			char *desc = ((item_t*)item_to_save.p)->desc;
			int iprice = ((item_t*)item_to_save.p)->price;
			char price[255];
			snprintf(price, 255, "%d", iprice); // Convert int price to char price

			// Write to file
			fputs(name, fptr);
			fputs("\n", fptr);

			fputs(desc, fptr);
			fputs("\n", fptr);

			fputs(price, fptr);
			fputs("\n", fptr);

			// Write shelfs to file
			save_shelfs(fptr, ((item_t*)item_to_save.p)->list);
		}
	}
	return;
}

void save_db(tree_t *db)
{
	elem_t *elem_list = tree_elements(db);

	FILE *fptr;
	fptr = fopen("db_persistance", "w");

	save_item(elem_list, fptr, tree_size(db));

	free(elem_list);
	fclose(fptr);
	return;
}
// -------------- Read ---------------
char *new_line_delete(char *line)
{
	int i = strlen(line) - 1;
	line[i] = '\0';
	return line;
}

void load_shelfs(FILE *fptr, elem_t *elem)
{
	size_t buf_siz = 255;
	char *line = calloc(1, sizeof(char *));
	getline(&line, &buf_siz, fptr);
	int list_size = atoi(line);

	for(int i = 0; i < list_size; i++)
	{
		shelf_t *new_shelf = calloc(1, sizeof(shelf_t));

		getline(&line, &buf_siz, fptr);               // Name
		char *new_name = new_line_delete(strdup(line));
		new_shelf->shelf_name = new_name;
		getline(&line, &buf_siz, fptr);               // Amount
		new_shelf->amount = atoi(line);

		elem_t list_elem = { .p = new_shelf};

		if(i == 0)                                    // Om första varvet, gör ny lista
		{
			((item_t*)elem->p)->list = list_new(l_copy_func, l_free_func, l_comp_func);
		}
		list_append(((item_t*)elem->p)->list, list_elem);

		//free(new_name);
		free(new_shelf);
	}
	free(line);
	return;
}

void load_item(tree_t *db, FILE *fptr)
{
	size_t buf_siz = 255;
	char *line = calloc(1, sizeof(char *));
	getline(&line, &buf_siz, fptr);
	int db_size = atoi(line);

	for(int i = 0; i < db_size; i++)
	{
		item_t *new_item = calloc(1, sizeof(item_t));

		getline(&line, &buf_siz, fptr);               // Name
		char *new_name = new_line_delete(strdup(line));
		new_item->name = new_name;
		printf("new_name = %s, line = %s", new_name, line);
		getline(&line, &buf_siz, fptr);               // Description
		char *new_desc = new_line_delete(strdup(line));
		new_item->desc = new_desc;
		getline(&line, &buf_siz, fptr);               // Price
		new_item->price = atoi(line);

		elem_t elem = { .p = new_item };
		tree_key_t key = { .p = new_item->name };

		load_shelfs(fptr, &elem);

		tree_insert(db, key, elem);

		//free(new_name);
		//free(new_desc);
		free(new_item);
	}
	free(line);
	return;
}

void load_db(tree_t *db)
{
	FILE *fptr;
	fptr = fopen("db_persistance", "r");

	if(fptr)
	{
		load_item(db, fptr);
		fclose(fptr);
	}
	else puts("No file to load database from!");
	return;
}

//////////// ================= EVENT LOOPS
///
/// Handles menus
// -------------- Edit menu ---------------
void event_loop_edit(tree_t *db, action_t *latest_action )
{
	bool quit_v = true;
	int choise = list_database(db, true);
	tree_key_t *key_list = tree_keys(db);
	elem_t to_edit = {};
	tree_get(db, key_list[choise], &to_edit);
	free(key_list);
	print_item(to_edit);
	//TODO: Undo
	while(quit_v)
	{
		char option = ask_question_menu_edit();
		switch(option)
		{
			case 'B' :                       // Beskrivning
				edit_desc(&to_edit, latest_action);
				break;

			case 'P' :                       // Pris
				edit_price(&to_edit, latest_action);
				break;

			case 'L' :                       // Lagerhylla
				edit_shelf(db, &to_edit);
				break;

			case 'T' :                       // Antal
				edit_amount(db, &to_edit);
				break;

			case 'A' :                       // Avbryt
				quit_v = false;
				break;
		}
	}
}

// -------------- Main menu ---------------
void event_loop(tree_t *db, action_t *latest_action)
{
	bool quit_v = true;

	while(quit_v)
	{
		char option = ask_question_menu();
		switch(option)
		{
			case 'L' :				// Add item

				input_item(db, latest_action);
				break;

			case 'T' :                       // Delete
				remove_item_from_db(db, latest_action);
				break;

			case 'R' :                       // Redigera
				event_loop_edit(db, latest_action);
				break;

			case 'G' :                       // Undo
				undo_action(db, latest_action);
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

	action_t latest_action;

	//direct_input(db);

	load_db(db);

	puts("printing tree preorder\n");

	tree_apply(db, preorder, t_print_func, NULL);

	event_loop(db, &latest_action);

	puts("saving db to .txt...");
	save_db(db);
	puts("deleting db...");
	tree_delete(db, true, true);
	puts("exit");
	return 0;
}
