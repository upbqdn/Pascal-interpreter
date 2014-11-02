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
#define CONTAIN (0)
#define NOCONTAIN (1)

/*
 *Deklaracia struktur a premennych
 */
typedef struct elementS //struktura, ktora je prvok zoznamu
{
	struct elementS *ptr; //ukazatel na dalsi prvok
	int data; //obsah struktury !!!!!!!!!!!!!!!!!!!!!!!zmenime este
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
 void hash_init();
 void hash_insert(char *, int);
 void hash_destroy();
 int hash(char *);
 int hash_search(char *, int);