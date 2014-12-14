/*
 *Autor: Filip Gulan xgulan00@stud.fit.vutbr.cz
 *Kniznica obsahujuca funkcie na pracu s hashovaciou tabulkou, quick sortom a knut morissom.
 */

#include "ial.h"
#include "garbage.h"

/**********************************
 *Implementacia hashovacej tabulky*
 *********************************/

/*
 *Funkcia na ziskanie zo stringu hashovaci kluc
 *Prevedie jednotlive chary stringu na intiger spocita a nasledne vydeli so zvyskom rozsahom hashovacej tabulky
 *Parameter: string/pointer na zaciatok stringu, ktory mame previes na hash kluc
 *Vracia: hashovaci kluc
 */
int hash(char *my_string)
{
	int hash_key = ORIGIN; //v tomto bude ulozen vysledok hashovacej funkcie
	int help_var = ORIGIN; //pomocna premenna na prevod z char na int
	for(unsigned int i = ORIGIN; i < strlen(my_string); i++) //cyklus na vsekty chary v stringu
 	{
 		help_var = my_string[i];
 		hash_key = hash_key + help_var;
 	}
 	hash_key = hash_key % HASH_ARRAY_SIZE;
 	return hash_key;
}

/*
 *Funkcia na alokaciu inicializaciu pola zoznamov
 *Vraxcia: voidovsku adresu naalokovanej tabulky
 */
void *hash_init()
{
	list *localTable = mymalloc(sizeof(list) * HASH_ARRAY_SIZE); //naalokujeme miesto pre hash tabulku
 	for(int i = ORIGIN; i < (HASH_ARRAY_SIZE); i++) //pre kazdy zoznam v tabulke
 	{
 		localTable[i].Act = NULL;
 		localTable[i].First = NULL;
 	}
 	return (void*) localTable; //vratime adresu vo void podobe
}

/*
 *Funkcia na vlozenie do tabulky string id
 *Parameter: tabulka, id string
 */
void hash_insert_i(list *localTable, char *id)
{
	if(hash_search(localTable, id) == NOCONTAINS)
	{
		list_element help_var = mymalloc(sizeof(struct elementS));
 		help_var->id = mymalloc(strlen(id) * sizeof(char));
 		if(help_var == NULL || help_var->id == NULL) //alokacia prebehla chybne
 		{
 			//nic
 		}
 		else //alokacia prebehla spravne
 		{
 			int i = hash(id);
 			strcpy(help_var->id, id);
 			help_var->ptr = localTable[i].First;
 			localTable[i].First = help_var;
 		}
	}
}

/*
 *Funkcia, ktora vrati vlozi hodnotu def sign
 *Parametre: tabulka, v ktorej nastavujeme, kluc polozky, ktoru nastavujeme a sign aky cheme nastavit
 */
void hash_set_sign(list *localTable, char *id, int sign)
{
	if(hash_search(localTable, id) == NOCONTAINS)
	{
		//nic
	}
	else
	{
		int i = hash(id);
		localTable[i].Act = localTable[i].First;
		while(localTable[i].Act != NULL) //prejde vsetky prvky zoznamu
		{
			if(strcmp(localTable[i].Act->id, id) == 0) //porovna retazce
			{
				localTable[i].Act->def_sign = sign;	
			}
			localTable[i].Act = localTable[i].Act->ptr; //posunieme sa o prvok dalej
		}
	}
}

void set_arg_num(list *localTable, char *id, int arg_num)
{
	if(hash_search(localTable, id) == NOCONTAINS)
	{
		//nic
	}
	else
	{
		int i = hash(id);
		localTable[i].Act = localTable[i].First;
		while(localTable[i].Act != NULL) //prejde vsetky prvky zoznamu
		{
			if(strcmp(localTable[i].Act->id, id) == 0) //porovna retazce
			{
				localTable[i].Act->arg_num = arg_num;	
			}
			localTable[i].Act = localTable[i].Act->ptr; //posunieme sa o prvok dalej
		}
	}
}

/*
 *Funkcia na vkladanie do hashovacej tabulky podla id, ak sa tam prvok nachadza iba prepiseme type
 *Parametre: tabulka, id - ktore vkladame a type - ktory vkladame
 */
void hash_insert_it(list *localTable, char *id, int type)
{
 	if(hash_search(localTable, id) == NOCONTAINS) //polozka sa v tabulke este nenachadza 
 	{
 		list_element help_var = mymalloc(sizeof(struct elementS));
 		help_var->id = mymalloc(strlen(id) * sizeof(char));
 		if(help_var == NULL || help_var->id == NULL) //alokacia prebehla chybne
 		{
 			//nic
 		}
 		else //alokacia prebehla spravne
 		{
 			int i = hash(id);
 			strcpy(help_var->id, id); //ulozime token data
 			help_var->type = type;
 			help_var->ptr = localTable[i].First;
 			localTable[i].First = help_var;
 		}
 	}
 	else //ak sa tam nachadza tak iba doplnim jeho typ
 	{
 		int i = hash(id);
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
 *Funkcia na vytvorenie tabulky funkcie a ujlozenie jej adresy do ref
 *Parameter: tabulka, id podla ktoreho najdeme kde ulozime adresu 
 */
void hash_insert_func(list *localTable, char *id)
{
	if(hash_search(localTable, id) == NOCONTAINS)
	{
		
	}
	else
	{
		int i = hash(id);
		localTable[i].Act = localTable[i].First;
		while(localTable[i].Act != NULL) //prejde vsetky prvky zoznamu
		{
			if(strcmp(localTable[i].Act->id, id) == 0) //porovna retazce
			{

				localTable[i].Act->ref = hash_init();
			}
			localTable[i].Act = localTable[i].Act->ptr; //posunieme sa o prvok dalej
		}
	}
}

/*
 *Funkcia na zistenie ci sa hladany prvok nachadza v hashovacej tabulke, hlada podla kluca
 *Parameter: tabulka, data - hladane data
 *Vracia: 0/CONTAINS v pripade ze sa prvok nachadza, 1/NOCONTAINS v ripade ze nie
 */
int hash_search(list *localTable, char *id)
{
	int i = hash(id);
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
 *Funkcia, ktora vrati podla podla kluca typ polozky
 *Parametre: prehladavana tabulka, a kluc
 *Vracia: ak najde vracia jeho typ, ak nenajde vracia NAN
 */
unsigned hash_return_type(list *localTable, char *id)
{
	int i = hash(id);
	localTable[i].Act = localTable[i].First;
	while(localTable[i].Act != NULL) //prejde vsetky prvky zoznamu
	{
		if(strcmp(localTable[i].Act->id, id) == 0) //porovna retazce
		{
			return localTable[i].Act->type;
		}
		localTable[i].Act = localTable[i].Act->ptr; //posunieme sa o prvok dalej
	}
	return NAN;
}

/*
 *Funkcia, ktora vracia podla tabulky a kluca def sign polozky
 *Parametre: prehladavana tabulka, a kluc
 *Vracia: ak najde vracia jeho def sign, ak nenajde vracia NAN
 */
int hash_is_sign(list *localTable, char *id)
{
	int i = hash(id);
	localTable[i].Act = localTable[i].First;
	while(localTable[i].Act != NULL) //prejde vsetky prvky zoznamu
	{
		if(strcmp(localTable[i].Act->id, id) == 0) //porovna retazce
		{
			return localTable[i].Act->def_sign;
		}
		localTable[i].Act = localTable[i].Act->ptr; //posunieme sa o prvok dalej
	}
	return UNDECLARED;
}

int get_arg_num(list *localTable, char *id)
{
	int i = hash(id);
	localTable[i].Act = localTable[i].First;
	while(localTable[i].Act != NULL) //prejde vsetky prvky zoznamu
	{
		if(strcmp(localTable[i].Act->id, id) == 0) //porovna retazce
		{
			return localTable[i].Act->arg_num;
		}
		localTable[i].Act = localTable[i].Act->ptr; //posunieme sa o prvok dalej
	}
	return NAN;
}

/*
 *Funkcia, ktora vrati voidovsku adresu polozky
 *Parameter: tabulka, id string
 *Vracia: void adresu
 */
void *hash_adress(list *localTable, char *id)
{
	int i = hash(id);
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

/*
 *Funkcia na zistenie vratenie polozky tabulky, ktoru hladame
 *Parameter: tabulka, data - hladane podla data
 *Vracia: polozku ak sa v tabulke nachadza, alebo NULL ak sa polozka nenachadza
 */
list_element hash_return(list *localTable, char *id)
{
	int i = hash(id);
	localTable[i].Act = localTable[i].First;
	while(localTable[i].Act != NULL) //prejde vsetky prvky zoznamu
	{
		if(strcmp(localTable[i].Act->id, id) == 0) //porovna retazce
		{
			return localTable[i].Act;
		}
		localTable[i].Act = localTable[i].Act->ptr; //posunieme sa o prvok dalej
	}
	return NULL;
}

int arg_numSearch(list *localTable, unsigned int num)
{
	for(int i = ORIGIN; i < HASH_ARRAY_SIZE; i++) //pre kazdy zoznam v tabulke
 	{
 		localTable[i].Act = localTable[i].First;
 		while(localTable[i].Act != NULL) //prejde vsetky elementy zoznamu
 		{
 			if(num == localTable[i].Act->arg_num)
 			{

 				return localTable[i].Act->type;
 			}
 			localTable[i].Act = localTable[i].Act->ptr;
 		}
 	}
 	return 0;
}

/*
 *Funkcia na skopirovanie celej hash tabulky
 *Parameter: tabulka, ktoru chceme skopirovat
 *Vracia: voidovsku adresu klonu tabulky
 */
void *copyhash(list *localTable)
{
	void *newTable = hash_init();
	for(int i = ORIGIN; i < HASH_ARRAY_SIZE; i++) //pre kazdy zoznam v tabulke
 	{
 		localTable[i].Act = localTable[i].First;
 		while(localTable[i].Act != NULL) //prejde vsetky elementy zoznamu
 		{
 			hash_insert_it(newTable, localTable[i].Act->id, localTable[i].Act->type);
 			if(localTable[i].Act->ref != NULL) //ak sa nieco nachadza aj v ref tak nakopirujem tiez to
 			{
 				hash_insert_func(newTable, localTable[i].Act->ref);
 			}
 			localTable[i].Act = localTable[i].Act->ptr;
 		}
 	}
 	return (void*) newTable;
}

/*
 *Funkcia na zrusenie vsetkych zoznamov v hashovacej tabulke
 *Parametre: tabulka
 */
void hash_destroy(list *localTable)
{
	for(int i = ORIGIN; i < HASH_ARRAY_SIZE; i++) //pre kazdy zoznam v tabulke
 	{
 		while(localTable[i].First != NULL) //prejde vsetky elementy zoznamu
 		{
 			if(localTable[i].First == localTable[i].Act) //zrusi aj aktivny prvok
 			{
 				localTable[i].Act = NULL;
 			}
 			list_element help_var = localTable[i].First;
 			localTable[i].First = localTable[i].First->ptr;
 			myfree(help_var->id);
 			myfree(help_var); //uvolnime prvok zoznamu
 		}
 	}
 	myfree(localTable); //uvolnime tabulku
}

/***************************
 *Implementacia Quick sortu**
 **************************/
/*
 *Funkcia quick sort z IAL, nedelena na funkcie
 *Parametre: Array - pole znakov na zoradenie*/
void sort(char *Array, int left, int right)
{
	char temp; //pomocna premenna na prehodenie hodnot
	int i = left;
	int j = right;
	int median = Array[(i + j) / 2]; //pseudo median bezna metoda 
	do{
		while(Array[i] < median) //hladanie prveho zlava
		{
			i++;
		}
		while(Array[j] > median) //preveho zprava
		{
			j--;
		}
		if(i <= j)
		{
			temp = Array[i]; //vymena bez funkcie jednoduchsie
			Array[i] = Array[j];
			Array[j] = temp; 
			i++;
			j--;
		}
	} while(i <= j); //partition koniec podmienka inak ako pascal neni until
	if(left < j)
	{
		sort(Array, left, j); //dolava
	}
	if(i < right)
	{
		sort(Array, i, right); //doprava
	}
}

/*********************************
 *Implementacia Knutt Moris Pratt*
 ********************************/
/*
 *Funkcia na hladanie podretazca v retazci implementovana Knutt Moris Pratt algoritmom z IAL
 *Parametre: 1 - retaze, ktory prehladavame, 2 -retazec, ktory hladame
 *Vracia: -1/NOSUBSTRING v pripade ze sa podretazec v retazci nenachadza, alebo index na ktorom mieste sa nachadza
 */
int find(char *T, char *P)
{
	int *Fail = malloc(strlen(P) * sizeof(int)); //alokovanie miesta pre pomocny vektor
	int TL = strlen(T); //dlka retazca T
	int PL = strlen(P); //dlzka retazca P
	kmpgraf(P, Fail, PL); //vytvorime pomocny vektor
	int TInd = ORIGIN; //Knutt Morris Pratt
	int PInd = ORIGIN;
	while((TInd < TL) && (PInd < PL))
	{
		if((PInd == -1) || (T[TInd] == P[PInd]))
		{
			TInd++;
			PInd++;
		}
		else
		{
			PInd = Fail[PInd];
		}
	}
	if(PInd >= PL) //vrati poziciu kde sa podretazec nasiel
	{
		myfree(Fail);
		return TInd - PL + 1;
	}
	else //nenasiel sa retazec
	{
		myfree(Fail);
		return NOSUBSTRING;
	}
}

/*
 *Funkcia na generovanie pomocneho vektoru
 *Parametre: 1 - retazec, 2 - pomocny vektor alokovany vo funckii fail 
 */
void kmpgraf(char *P, int *Fail, int PL)
{
	int k;
	int r;
	Fail[ORIGIN] = -1; //zaciatok inicializacie
	for(k = 1; k < PL; k++)
	{
		r = Fail[k - 1];
		while((r > 0) && (P[r] != P[k - 1]))
		{
			r = Fail[r];
		}
		Fail[k] = r++;
	}
}

/* najde v zadanem retezci podratezec zacinajici indexem i(pocitano od 0) o delce n znaku */

/*char *copy(char *string, int i, int n)
{
	char *pom;
	int poc = 1;
	int k = 0;
	
	while (poc <= n)
	{
		pom[k] = string[i-1];
		poc++; 
		i++;
		++k;
	}
	pom[k] = '\0';
	return pom;
}
*/


/*
list *GLOB;
int main()
{
	//UKAZKA
	GLOB = hash_init(); //vytvorime tabulku a ulozime si jej adresu  //vytvorenie 2 tabulky, pre ukazku
	//hash_insert_i(GLOB, "aaa"); //vlozime hodnotu do 2 tabulky
	//hash_insert_it(GLOB, "aaa", 5);
	//printf("%d\n", hash_return_type(GLOB, "aaa")); //vypiseme obsah token.state poriadna hnusoba!
	//printf("%d\n", (*((list_element) (hash_adress(GLOB, "aaa")))).type); //vypiseme obsah token.state poriadna hnusoba!
	//set_arg_num(GLOB, "aaa", 5555);
	hash_insert_it(GLOB, "ccc", 587);
	hash_insert_it(GLOB, "a", 1);
	set_arg_num(GLOB, "a", 3);
	set_arg_num(GLOB, "ccc", 4785);
	//printf("%d\n", get_arg_num(GLOB, "ccc")); //vypiseme obsah token.state poriadna hnusoba!
	//hash_insert_func(GLOB, "aaa");
	//list *lokalna;
	//lokalna = (*((list_element) (hash_adress(GLOB, "aaa")))).ref;
	//hash_insert_it(lokalna, "ab", 515);
	//printf("%d\n", (*((list_element) (hash_adress(lokalna, "ab")))).type);
	printf("%d novinka\n", arg_numSearch(GLOB, 3));
	//TO JE IBA PRE MNA!
	//hash_insert_it(localTable, "bbbb", 15);
	//Lhash_insert_it(local, "aaa", 10);
	//hash_insert_it(localTable, "aaa", 50);
	//void *local = copyhash(localTable);
	//Lhash_insert(localTable, 10); //vlozime hodnotu do 1 tabulky
	//Lhash_insert(localTable, 8); //vlozime hodnotu 2 do 1 tabulky
	//int a = hash_search(local, 8); //nic iba ukazka
	//Llist_element adresa = (Llist_element) (hash_adress(localTable, 10)); //nic iba ukazka
	//printf("%d\n", (*((list_element) (hash_adress(GLOB, "aaa")))).type); //vypiseme obsah token.state poriadna hnusoba!
	
	//Lhash_destroy(local); //zrusime vsetky prvaky aj lokalnu 2 tabulku 
	//hash_destroy(localTable); //a teraz 1 tabulku
 	return 0;
}*/
