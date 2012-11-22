#define _XOPEN_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <form.h>
#include <ctype.h>

void printCurrentTime(FILE *file);
char *dateToDay(char *datestr, char *timestr);
void printIntro(FILE *file);
void trimTrailingWhitespace(char *string);
void get_input(FORM *my_form, FIELD **field);