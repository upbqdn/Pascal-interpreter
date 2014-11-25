/*
 *Hlavickovy subor pre ial.c
 *Autor: Filip Gulan xgulan00@stud.fit.vutbr.cz
 */

/*
 *Inkludovanie funkcii, ktore budeme pouzivat
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <stdarg.h>

/*
 *Define na eliminovanie magickych konstant
 */
#define HASH_ARRAY_SIZE (10) //velkost hashovacej tabulky
#define ORIGIN (0) //zaciatok hashovacej tabulky/ alebo proste 0
#define CONTAINS (0) //obsahuje prvok/retazec...
#define NOCONTAINS (1) //neobsahuje prvok/retazec
#define NOSUBSTRING (-1) //neobsahuje podretazec

/*
 *Deklaracia struktur a premennych
 */
typedef struct elementS //struktura, ktora je prvok zoznamu
{
	struct elementS *ptr; //ukazatel na dalsi prvok
	int token_state; //stav tokenu
	char *token_data; //data tokenu
	int type; //typ premennej int/real/bool/string
	int tid; //premennej, funkcie,  ostatne
} *list_element;

typedef struct listS //struktura, ktora je zoznam
{
	list_element Act; //ukazatel na aktualny prvok s ktorym pracujeme, na akom mieste sa nachadzame 
	list_element First; //ukazatel na prvy prvok zoznamu
} list;

extern list list_array[HASH_ARRAY_SIZE]; //globalna tabulka hash klucov

/*
 *Deklaracia funkcii
 */
 int hash(char *);
 void hash_init();
 void hash_insert(char *, int, int, int);
 int hash_search(char *);
 list_element hash_return(char *);
 void hash_destroy();
 void sort(char *, int, int);
 int find(char *, char *);
 void vector(char *, int *);
