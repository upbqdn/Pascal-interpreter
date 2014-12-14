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
#include <math.h>
//#include <stdarg.h>

/*
 *Define na eliminovanie magickych konstant
 */
#define HASH_ARRAY_SIZE (365) //velkost hashovacej tabulky
#define ORIGIN (0) //zaciatok hashovacej tabulky/ alebo proste 0
#define CONTAINS (0) //obsahuje prvok/retazec...
#define NOCONTAINS (1) //neobsahuje prvok/retazec
#define NOSUBSTRING (-1) //neobsahuje podretazec
#define UNDECLARED (0) //priznak nedefinovanej funkcie
#define FORWARDED (1) //priznak deklarovanej funkcie
#define DEFINED (2)  //priznak definovanej funkcie

/*
 *Deklaracia struktur a premennych
 */
typedef struct elementS //struktura, ktora je prvok zoznamu
{
	struct elementS *ptr; //ukazatel na dalsi prvok
	int type; //typ tokenu int...
	char *id; //data tokenu, meno...
	void *ref; //adresa
	int def_sign;
	void *start;
	unsigned int arg_num;
} *list_element;

typedef struct listS //struktura, ktora je zoznam
{
	list_element Act; //ukazatel na aktualny prvok s ktorym pracujeme, na akom mieste sa nachadzame 
	list_element First; //ukazatel na prvy prvok zoznamu
} list;

typedef list list_array[HASH_ARRAY_SIZE]; //tabulka ktoru budeme vytvarat malocom
//extern list *GLOB;

extern list *GLOBFRAME; // globalna tabulka
extern list *MASTERTAB; // globalna tabulka

/*
 *Deklaracia funkcii
 */
 int hash(char *);
 void *hash_init();
 void hash_insert_i(list *, char *);
 void hash_insert_it(list *, char *, int);
 void hash_insert_func(list *, char *);
 int hash_search(list *, char *);
 list_element hash_return(list *, char *);
 void *hash_adress(list *, char *);
 void *copyhash(list *);
 void hash_destroy(list *);
 void kmpgraf(char *, int *, int);
 int hash_is_sign(list *, char *);
 unsigned hash_return_type(list *, char *);
 void hash_set_sign(list *, char *, int);
 void set_arg_num(list *, char *, int);
 int get_arg_num(list *, char *);
 int arg_numSearch(list *, unsigned int);

 void sort(char *, int, int);
 int find(char *, char *);
//char *copy(char *string, int i, int n);
