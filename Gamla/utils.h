#ifndef __UTILS_H__
#define __UTILS_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

extern char *strdup(const char *);

typedef union {
  int   i;
  float f;
  char *s;
  char c;
} answer_t;

typedef bool(*check_func)(char *);
typedef answer_t(*convert_func)(char *);

answer_t ask_question(char *question, check_func check, convert_func convert);
bool is_number(char *str);
bool is_shelf(char* str);
bool not_empty(char *str);
bool is_menu_char(char *str);
bool is_menu_edit_char(char *str);
bool is_list_db_char(char *str);
int ask_question_int(char *question);
int read_string(char *buf, int buf_siz);
char *ask_question_string(char *question);
char *ask_question_shelf(char *question);
int string_length(char *str);
void print(char *str);
void println(char *str);

#endif
