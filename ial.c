/*
 *Autor: Filip Gulan xgulan00@stud.fit.vutbr.cz
 *Kniznica obsahujuca funkcie na pracu s hashovaciou tabulkou, quick sortom a knut morissom.
 */

#include "ial.h"

list list_array[HASH_ARRAY_SIZE];

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
	for(int i = ORIGIN; i < strlen(my_string); i++) //cyklus na vsekty chary v stringu
 	{
 		help_var = my_string[i];
 		hash_key = hash_key + help_var;
 	}
 	hash_key = hash_key % HASH_ARRAY_SIZE;
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
 *Parametre: data - ktore vkladame a state - ktory vkladame
 */
void hash_insert(char *data, int state, int type, int tid)
{
 	list_element help_var = malloc(sizeof(struct elementS));
 	help_var->token_data = malloc(strlen(data) * sizeof(char));
 	if(help_var == NULL || help_var->token_data == NULL) //alokacia prebehla chybne
 	{
 		//doplnit error
 	}
 	else //alokacia prebehla spravne
 	{
 		int i = hash(data);
 		strcpy(help_var->token_data, data); //ulozime token data
 		help_var->token_state = state; //ulozime token stav
 		help_var->type = type;
 		help_var->tid = tid;
 		help_var->ptr = list_array[i].First;
 		list_array[i].First = help_var;
 	}
}

/*
 *Funkcia na zistenie ci sa hladany prvok nachadza v hashovacej tabulke, hlada podla kluca
 *Parameter: data - hladane data
 *Vracia: 0/CONTAINS v pripade ze sa prvok nachadza, 1/NOCONTAINS v ripade ze nie
 */
int hash_search(char *data)
{
	int i = hash(data);
	list_array[i].Act = list_array[i].First;
	while(list_array[i].Act != NULL) //prejde vsetky prvky zoznamu
	{
		if(strcmp(list_array[i].Act->token_data, data) == 0) //porovna retazce
		{
			return CONTAINS;
		}
		list_array[i].Act = list_array[i].Act->ptr; //posunieme sa o prvok dalej
	}
	return NOCONTAINS;
}

/*
 *Funkcia na zistenie vratenie polozky tabulky, ktoru hladame
 *Parameter: data - hladane podla data
 *Vracia: polozku ak sa v tabulke nachadza, alebo NULL ak sa polozka nenachadza
 */
list_element hash_return(char *data)
{
	int i = hash(data);
	list_array[i].Act = list_array[i].First;
	while(list_array[i].Act != NULL) //prejde vsetky prvky zoznamu
	{
		if(strcmp(list_array[i].Act->token_data, data) == 0) //porovna retazce
		{
			return list_array[i].Act;
		}
		list_array[i].Act = list_array[i].Act->ptr; //posunieme sa o prvok dalej
	}
	return NULL;
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
 			free(help_var->token_data);
 			free(help_var);
 		}
 	}
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
	int median = Array[(i + j) / 2]; //pseudo median
	do{
		while(Array[i] < median)
		{
			i++;
		}
		while(Array[j] > median)
		{
			j--;
		}
		if(i <= j)
		{
			temp = Array[i];
			Array[i] = Array[j];
			Array[j] = temp; 
			i++;
			j--;
		}
	} while(i <= j);
	if(left < j)
	{
		sort(Array, left, j);
	}
	if(i < right)
	{
		sort(Array, i, right);
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
	vector(P, Fail);
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
		free(Fail);
		return TInd - PL + 1;
	}
	else //nenasiel sa retazec
	{
		free(Fail);
		return NOSUBSTRING;
	}
}

/*
 *Funkcia na generovanie pomocneho vektoru
 *Parametre: 1 - retazec, 2 - pomocny vektor alokovany vo funckii fail 
 */
void vector(char *P, int *Fail)
{
	int k;
	int r;
	int PL = strlen(P);
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

/*
int main()
{
	hash_init();
	hash_insert("aaa", 5, 2, 3);
	
	list_element special = hash_return("aaa");
	printf("%d\n", special->token_state);
 	return 0;
}*/