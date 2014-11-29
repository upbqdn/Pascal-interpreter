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
 *Funkcia na vkladanie do lokalnej hashovacej tabulky
 *Parametre: adresa lokalnej hash tabulky, stav to a doplni podla toho co este vlozim!!!!!!!!!!!!!!
 */
void Lhash_insert(Llist *localTable, int state)
{
 	Llist_element help_var = malloc(sizeof(struct elementSL));
 	if(help_var == NULL) //alokacia prebehla chybne
 	{
 		//doplnit error
 	}
 	else //alokacia prebehla spravne
 	{
 		int i = 5; //teba doplnit hash funckiu!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 		help_var->token_state = state; //ulozime token stav
 		help_var->ptr = localTable[i].First;
 		localTable[i].First = help_var;
 	}
}

/*
 *Funkcia na zistenie vratenie polozky tabulky, ktoru hladame
 *Parametre: adresa lokalnej hashovacej tbulky, state to sa zmeni a prida sa potrebne!!!!!!!!!!!!!!1 
 *Vracia: ak sa polozka v tabulke nachadza vracia jej void adresu, inak vracia NULL
 */
void *Lhash_adress(Llist *localTable, int state)
{
	int i = 5; //teba doplnit hash funckiu!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	localTable[i].Act = localTable[i].First;
	while(localTable[i].Act != NULL) //prejde vsetky prvky zoznamu
	{
		if(localTable[i].Act->token_state == state) //porovna retazce
		{
			return (void*) (localTable[i].Act); //vratime adresu aktualnej vo void formate
		}
		localTable[i].Act = localTable[i].Act->ptr; //posunieme sa o prvok dalej
	}
	return NULL;
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
 			free(help_var); //uvolnime prvok zoznamu
 		}
 	}
 	free(localTable); //uvolnime tabulku
}

/* Ako sa s tym pracuje
int main()
{
	void *localTable = Lhash_init(); //vytvorime tabulku a ulozime si jej adresu 
	void *local = Lhash_init(); //vytvorenie 2 tabulky, pre ukazku
	Lhash_insert(local, 5); //vlozime hodnotu do 2 tabulky
	Lhash_insert(localTable, 10); //vlozime hodnotu do 1 tabulky
	Lhash_insert(localTable, 8); //vlozime hodnotu 2 do 1 tabulky
	//int a = hash_search(local, 8); //nic iba ukazka
	//Llist_element adresa = (Llist_element) (hash_adress(localTable, 10)); //nic iba ukazka
	printf("%d\n", (*((Llist_element) (Lhash_adress(localTable, 10)))).token_state); //vypiseme obsah token.state poriadna hnusoba!
	
	Lhash_destroy(local); //zrusime vsetky prvaky aj lokalnu 2 tabulku 
	Lhash_destroy(localTable); //a teraz 1 tabulku
 	return 0;
}*/
