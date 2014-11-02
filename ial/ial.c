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

void hash_insert(char *a)
{
 	//list_element help_var = malloc(sizeof(struct elementS));
}

/*
 *Funkcia na ziskanie zo stringu hashovaci kluc
 *Prevedie jednotlive chary stringu na intiger spocita a nasledne vydeli so zvyskom rozsahom hashovacej tabulky
 *Parameter: string/pointer na zaciatok stringu
 */
int hash(char *my_string)
{
	int hash_key = ORIGIN; //v tomto bude ulozen vysledok hashovacej funkcie
	int help_var = ORIGIN; //pomocna premenna na prevod z char na int
	for(int i = ORIGIN; i < strlen(my_string); i++) //cyklus na vsekty chary v stringu
 	{
 		help_var = my_string[i];
 		printf("%d\n", help_var);
 		hash_key = hash_key + help_var;
 	}
 	hash_key = hash_key % HASH_ARRAY_SIZE;
 	printf("%d\n", hash_key);
 	return hash_key;
}

int main()
{
 	hash_insert("aaccc\0");
 	return 0;
}