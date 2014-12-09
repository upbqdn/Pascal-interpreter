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

/*
 *Define na eliminovanie magickych konstant
 */
 #define PREFIX (3)
 #define ROZP5 (93)

/*
 *Deklaracia struktur a premennych
 */
extern int GlobalI;

/*
 *Deklaracia funkcii
 */
 char *stringRandGen();