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
void hash_insert(char *data, int state)
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
 *Implementacia Quick sortu*
 **************************/

void sort(int *A, int left, int right)
{
	int i = left;
	int j = right;
	partition(A, &left, &right, &i, &j);
	if(left < j)
	{
		sort(A, left, j);
	}
	if(i < right)
	{
		sort(A, i, right);
	}
}

void partition(int *A, int *left, int *right, int *i, int *j)
{
	i = left;
	j = right;
	int median = A[*i + *j] / 2;
	do{
		while(A[*i] < median)
		{
			i++;
		}
		while(A[*j] > median)
		{
			j--;
		}
		if(i <= j)
		{
			swap(A, i, j);
			i++;
			j--;
		}
	} while(i > j);
}

void swap(int *A, int i, int j)
{
	int temp = A[i];
	A[i] = A[j];
	A[j] = temp; 
}

void test(int *A)
{
	A[2] = 10 * A[2];
	if(A[2] == 30)
	{
		test(A);
	}
}

int main()
{
	int A[] = {0,5,3,2,1};
 	sort(A, A[0], A[4]);
 	for(int i = 0; i < 5; i++)
 	{
 		printf("%d\n", A[i]);
 	}
 	return 0;
}