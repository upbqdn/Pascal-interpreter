/*
 *Autor: Filip Gulan xgulan00@stud.fit.vutbr.cz
 *Kniznica obsahujuca funkcie na pracu so zasobnikom.
 */

/*
 *Nainkludovanie hackoveho suboru
 */
#include "stack.h"

/*
 *Premenne definicie
 */
stack S; //zasobnik

/*
 *Funkcia na inicializaciu zasobnika
 *Parameter: Zasobnik
 */
void stack_init(stack *S)
{
	S->First = NULL;
}

/*
 *Funkcia ktora ulozi na zasobnik hodnotu
 *Parameter: Zasobnik a vkladana hodnota
 */
void myPush(stack *S ,int data)
{
	stack_element pomocna = malloc(sizeof(struct stack_elementS));
	pomocna->data = data;
	pomocna->ptr_next = S->First;
	S->First = pomocna;
}

/*
 *Funkcia na popnutie vrcholu zasobniku
 *Parameter: Zasobnik
 */
void myPop(stack *S)
{
	stack_element pomocna = S->First;
	S->First = S->First->ptr_next;
	free(pomocna);
}

/*
 *Funkcia vrati hodnotu vrcholu zasobniku
 *Parameter: Zasobnik
 */
int myTop(stack *S)
{
	printf("%d\n", S->First->data);
	return S->First->data;
}

/*
 *Zrusi cely Zasobnik
 *Parameter: Zasobnik
 */
void destroyStack(stack *S)
{
	stack_element pomocna;
	while(S->First != NULL)
	{
		pomocna = S->First;
		S->First = S->First->ptr_next;
		free(pomocna);
	}
}


/*int main()
{
	stack_init(&S);
	myPush(&S, 5);
	myTop(&S);
	myPush(&S, 6);
	myTop(&S);
	myPop(&S);
	myTop(&S);
	destroyStack(&S);
 	return 0;
}*/