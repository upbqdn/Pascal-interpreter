/*
 *Autor: Filip Gulan xgulan00@stud.fit.vutbr.cz
 *Kniznica obsahujuca funkcie na pracu s hashovaciou tabulkou, pre lokalne funkcie.
 */

#include "htable.h"

/*
 *Funkcia na ziskanie zo stringu hashovaci kluc
 *Prevedie jednotlive chary stringu na intiger spocita a nasledne vydeli so zvyskom rozsahom hashovacej tabulky
 *Parameter: string/pointer na zaciatok stringu, ktory mame previes na hash kluc
 *Vracia: hashovaci kluc
 */
int Lhash(char *my_string)
{
	int hash_key = ORIGIN; //v tomto bude ulozen vysledok hashovacej funkcie
	int help_var = ORIGIN; //pomocna premenna na prevod z char na int
	for(int i = ORIGIN; i < strlen(my_string); i++) //cyklus na vsekty chary v stringu
 	{
 		help_var = my_string[i];
 		hash_key = hash_key + help_var;
 	}
 	hash_key = hash_key % LOCAL_HASH_ARRAY_SIZE;
 	return hash_key;
}

/*
 *Funkcia na inicializaciu/vytvorenie lokalnej hash tabulky a jej pola zoznamov 
 *Vracia: Vracia adresu lokalnej hash tabulky
 */
void *Lhash_init()
{
	Llist *localTable = malloc(sizeof(Llist) * LOCAL_HASH_ARRAY_SIZE); //naalokujeme miesto pre hash tabulku
 	for(int i = ORIGIN; i < LOCAL_HASH_ARRAY_SIZE; i++) //pre kazdy zoznam v tabulke
 	{
 		localTable[i].Act = NULL;
 		localTable[i].First = NULL;
 	}
 	return (void*) localTable; //vratime adresu vo void podobe
}

/*
 *Funkcia na vlozenie do lokalnej tabulky string id
 *Parameter: lokalna tabulka, id string
 */
void Lhash_insert_i(Llist *localTable, char *id)
{
	if(Lhash_search(localTable, id) == NOCONTAINS)
	{
		Llist_element help_var = malloc(sizeof(struct elementSL));
 		help_var->id = malloc(strlen(id) * sizeof(char));
 		if(help_var == NULL || help_var->id == NULL) //alokacia prebehla chybne
 		{
 			//doplnit error
 		}
 		else //alokacia prebehla spravne
 		{
 			int i = Lhash(id);
 			strcpy(help_var->id, id);
 			help_var->ptr = localTable[i].First;
 			localTable[i].First = help_var;
 		}
	}
}

/*
 *Funkcia na vkladanie do lokalnej hashovacej tabulky podla id, ak sa tam prvok nachadza iba prepiseme type
 *Parametre: tabulka s ktorou pracujeme, id - ktore vkladame a type - ktory vkladame
 */
void Lhash_insert_it(Llist *localTable, char *id, int type)
{
 	if(Lhash_search(localTable, id) == NOCONTAINS) //polozka sa v tabulke este nenachadza 
 	{
 		Llist_element help_var = malloc(sizeof(struct elementSL));
 		help_var->id = malloc(strlen(id) * sizeof(char));
 		if(help_var == NULL || help_var->id == NULL) //alokacia prebehla chybne
 		{
 			//doplnit error
 		}
 		else //alokacia prebehla spravne
 		{
 			int i = Lhash(id);
 			strcpy(help_var->id, id); //ulozime token data
 			help_var->type = type;
 			help_var->ptr = localTable[i].First;
 			localTable[i].First = help_var;
 		}
 	}
 	else //ak sa tam nachadza tak iba doplnim jeho typ
 	{
 		int i = Lhash(id);
		localTable[i].Act = localTable[i].First;
		while(localTable[i].Act != NULL) //prejde vsetky prvky zoznamu
		{
			if(strcmp(localTable[i].Act->id, id) == 0) //porovna retazce
			{
				localTable[i].Act->type = type;
			}
			localTable[i].Act = localTable[i].Act->ptr; //posunieme sa o prvok dalej
		}
 	}
}

/*
 *Funkcia na vytvorenie tabulky funkcie a ulozenie jej adresy do ref
 *Parameter: lokalna tabulka v ktorej to vytvorime ,id podla ktoreho najdeme kde ulozime adresu 
 */
void Lhash_insert_func(Llist *localTable, char *id)
{
	if(Lhash_search(localTable, id) == NOCONTAINS)
	{
		
	}
	else
	{
		int i = Lhash(id);
		localTable[i].Act = localTable[i].First;
		while(localTable[i].Act != NULL) //prejde vsetky prvky zoznamu
		{
			if(strcmp(localTable[i].Act->id, id) == 0) //porovna retazce
			{

				localTable[i].Act->ref = Lhash_init();
			}
			localTable[i].Act = localTable[i].Act->ptr; //posunieme sa o prvok dalej
		}
	}
}

/*
 *Funkcia na zistenie ci sa hladany prvok nachadza v lokalnej hashovacej tabulke, hlada podla kluca
 *Parameter: prehladavana tabulka ,data - hladane data
 *Vracia: 0/CONTAINS v pripade ze sa prvok nachadza, 1/NOCONTAINS v ripade ze nie
 */
int Lhash_search(Llist *localTable, char *id)
{
	int i = Lhash(id);
	localTable[i].Act = localTable[i].First;
	while(localTable[i].Act != NULL) //prejde vsetky prvky zoznamu
	{
		if(strcmp(localTable[i].Act->id, id) == 0) //porovna retazce
		{
			return CONTAINS;
		}
		localTable[i].Act = localTable[i].Act->ptr; //posunieme sa o prvok dalej
	}
	return NOCONTAINS;
}

/*
 *Funkcia na zistenie vratenie polozky tabulky, ktoru hladame
 *Parametre: adresa lokalnej hashovacej tbulky, state to sa zmeni a prida sa potrebne!!!!!!!!!!!!!!1 
 *Vracia: ak sa polozka v tabulke nachadza vracia jej void adresu, inak vracia NULL
 */
void *Lhash_adress(Llist *localTable, char *id)
{
	int i = Lhash(id);
	localTable[i].Act = localTable[i].First;
	while(localTable[i].Act != NULL) //prejde vsetky prvky zoznamu
	{
		if(strcmp(localTable[i].Act->id, id) == 0) //porovna retazce
		{
			return (void*) (localTable[i].Act); //vratime adresu aktualnej vo void formate
		}
		localTable[i].Act = localTable[i].Act->ptr; //posunieme sa o prvok dalej
	}
	return NULL;
}

void *copyLhash(Llist *localTable)
{
	void *newTable = Lhash_init();
	for(int i = ORIGIN; i < LOCAL_HASH_ARRAY_SIZE; i++) //pre kazdy zoznam v tabulke
 	{
 		localTable[i].Act = localTable[i].First;
 		while(localTable[i].Act != NULL) //prejde vsetky elementy zoznamu
 		{
 			Lhash_insert_it(newTable, localTable[i].Act->id, localTable[i].Act->type);
 			if(localTable[i].Act->ref != NULL) //ak sa nieco nachadza aj v ref tak nakopirujem tiez to
 			{
 				Lhash_insert_func(newTable, localTable[i].Act->ref);
 			}
 			localTable[i].Act = localTable[i].Act->ptr;
 		}
 	}
 	return (void*) newTable;
}

/*
 *Funkcia na zrusenie vsetkych zoznamov v lokalnej hashovacej tabulke a aj celej tabulky ktora bola namalokovana
 *Parametre: adresa lokalnej hachovacej tabulky
 */
void Lhash_destroy(Llist *localTable)
{
	for(int i = ORIGIN; i < LOCAL_HASH_ARRAY_SIZE; i++) //pre kazdy zoznam v tabulke
 	{
 		while(localTable[i].First != NULL) //prejde vsetky elementy zoznamu
 		{
 			if(localTable[i].First == localTable[i].Act) //zrusi aj aktivny prvok
 			{
 				localTable[i].Act = NULL;
 			}
 			Llist_element help_var = localTable[i].First;
 			localTable[i].First = localTable[i].First->ptr;
 			free(help_var->id);
 			free(help_var); //uvolnime prvok zoznamu
 		}
 	}
 	free(localTable); //uvolnime tabulku
}

/*
int main()
{
	void *localTable = Lhash_init(); //vytvorime tabulku a ulozime si jej adresu  //vytvorenie 2 tabulky, pre ukazku
	Lhash_insert_i(localTable, "aaa"); //vlozime hodnotu do 2 tabulky
	//Lhash_insert_it(localTable, "aaa", 5);
	Lhash_insert_it(localTable, "bbbb", 15);
	//Lhash_insert_it(local, "aaa", 10);
	//Lhash_insert_it(localTable, "aaa", 50);
	void *local = copyLhash(localTable);
	//Lhash_insert(localTable, 10); //vlozime hodnotu do 1 tabulky
	//Lhash_insert(localTable, 8); //vlozime hodnotu 2 do 1 tabulky
	//int a = hash_search(local, 8); //nic iba ukazka
	//Llist_element adresa = (Llist_element) (hash_adress(localTable, 10)); //nic iba ukazka
	printf("%d\n", (*((Llist_element) (Lhash_adress(local, "aaa")))).type); //vypiseme obsah token.state poriadna hnusoba!
	
	//Lhash_destroy(local); //zrusime vsetky prvaky aj lokalnu 2 tabulku 
	Lhash_destroy(localTable); //a teraz 1 tabulku
 	return 0;
}*/
