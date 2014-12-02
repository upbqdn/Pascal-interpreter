/*
 *Hlavickovy subor pre htable.c
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
#define LOCAL_HASH_ARRAY_SIZE (365) //velkost hashovacej tabulky
#define ORIGIN (0)
#define CONTAINS (0) //obsahuje prvok/retazec...
#define NOCONTAINS (1) //neobsahuje prvok/retazec

/*
 *Deklaracia struktur a premennych
 */
typedef struct elementSL //struktura, ktora je prvok zoznamu
{
	struct elementSL *ptr; //ukazatel na dalsi prvok
	int type;
	char *id;
	void *ref;
} *Llist_element;

typedef struct listSL //struktura, ktora je zoznam
{
	Llist_element Act; //ukazatel na aktualny prvok s ktorym pracujeme, na akom mieste sa nachadzame 
	Llist_element First; //ukazatel na prvy prvok zoznamu
} Llist;

typedef Llist Llist_array[LOCAL_HASH_ARRAY_SIZE]; //tabulka ktoru budeme vytvarat malocom

/*
 *Deklaracia funkcii
 */
 int Lhash(char *);
 void *Lhash_init();
 void Lhash_insert_i(Llist *, char *);
 void Lhash_insert_it(Llist *, char *, int);
 void Lhash_insert_func(Llist *, char *);
 void *Lhash_adress(Llist *, char *);
 int Lhash_search(Llist *, char *);
 void Lhash_destroy(Llist *);
