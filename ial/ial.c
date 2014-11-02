/*
 *Autor: Filip Gulan xgulan00@stud.fit.vutbr.cz
 *Kniznica obsahujuca funkcie na pracu s hashovaciou tabulkou a quick sortom.
 */

#include "ial.h"

list list_array[HASH_ARRAY_SIZE];

/**********************************
 *Implementacia hashovacej tabulky*
 *********************************/

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
 		printf("%d\n", help_var); //ttttttttttttttteeeeeeeeeeeeeeeeeeessssssssssssssssssttttttttttttttttt
 		hash_key = hash_key + help_var;
 	}
 	hash_key = hash_key % HASH_ARRAY_SIZE;
 	printf("%d\n", hash_key); //ttttttttttttttteeeeeeeeeeeeeeeeeeessssssssssssssssssttttttttttttttttt
 	return hash_key;
}

/*
 *Funkcia na inicializaciu pola zoznamov
 */
void hash_init()
{
 	for(int i = ORIGIN; i < HASH_ARRAY_SIZE; i++) //pre kazdy zoznam v tabulke
 	{
 		list_array[i].Act = NULL;
 		list_array[i].First = NULL;
 	}
}
/*
 *Funkcia na vkladanie do hashovacej tabulky
 *Parametre: data a stav
 */
void hash_insert(char *my_string)
{
 	list_element help_var = malloc(sizeof(struct elementS));
 	if(help_var == NULL) //alokacia prebehla chybne
 	{
 		//doplnit error
 	}
 	else //alokacia prebehla spravne
 	{
 		int i = hash(my_string);
 		help_var->data = 5; //treeeeeeeeeeeeeeeeeeeeeeba opravit toto je test
 		help_var->ptr = list_array[i].First;
 		list_array[i].First = help_var;
 	}
}

/*
 *Funkcia na zrusenie vsetkych zoznamov v hashovacej tabulke
 */
void hash_destroy()
{
	for(int i = ORIGIN; i < HASH_ARRAY_SIZE; i++) //pre kazdy zoznam v tabulke
 	{
 		while(list_array[i].First != NULL) //prejde vsetky elementy zoznamu
 		{
 			if(list_array[i].First == list_array[i].Act) //zrusi aj aktivny prvok
 			{
 				list_array[i].Act = NULL;
 			}
 			list_element help_var = list_array[i].First;
 			list_array[i].First = list_array[i].First->ptr;
 			free(help_var);
 		}
 	}
}

int main()
{
 	hash_destroy();
 	return 0;
}