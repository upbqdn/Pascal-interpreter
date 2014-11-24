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
 *Funkcia ktora ulozi na zasobnik niekolko hodnot podla poctu argumentov
 *Parameter: Zasobnik, pocet dalsich parametrov a vkladanane hodnoty
 */
void myPushMul(stack *S ,int num, ...)
{
	va_list valist;
    va_start(valist, num);
    for (int i = 0; i < num; i++)
    {
    stack_element pomocna = malloc(sizeof(struct stack_elementS));
	pomocna->data = va_arg(valist, int);;
	pomocna->ptr_next = S->First;
	S->First = pomocna;
    }
    va_end(valist);
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
	printf("%d kokot \n", S->First->data);
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
	myPushMul(&S, 3, 5 ,4, 3);
	myTop(&S);
	myPop(&S);
	myTop(&S);
	myPop(&S);
	myTop(&S);
	myPop(&S);
	destroyStack(&S);
 	return 0;
}*/