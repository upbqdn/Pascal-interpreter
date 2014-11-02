/*
 *Autor: Filip Gulan xgulan00@stud.fit.vutbr.cz
 *Kniznica obsahujuca funkcie na pracu s hashovaciou tabulkou a quick sortom.
 */

#include "ial.h"

 list list_array[HASH_ARRAY_SIZE];

/*
 *Funkcia na inicializaciu pola zoznamov
 */
 void hash_init()
 {
 	for(int i = ORIGIN; i < HASH_ARRAY_SIZE; i++)
 	{
 		list_array[i].Act = NULL;
 		list_array[i].First = NULL;
 	}
 }

 void hash_insert()
 {
 	//list_element help_var = malloc(sizeof(struct elementS));
 }

 void hash_remove()
 {

 }

 int hash(int a) //zmenime asi budeme dostavat string
 {
 	int hash_key = a % HASH_ARRAY_SIZE;
 	return hash_key;
 }

 int main()
 {
 	hash_init();
 	return 0;
 }