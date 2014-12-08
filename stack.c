/*
 *Autor: Filip Gulan xgulan00@stud.fit.vutbr.cz
 *Kniznica obsahujuca funkcie na pracu so zasobnikom.
 */

/*
 *Nainkludovanie hackoveho suboru
 */
#include "stack.h"
#include "garbage.h"

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
	stack_element pomocna = mymalloc(sizeof(struct stack_elementS));
	if (pomocna != NULL)
	{
		pomocna->data = data;
		pomocna->ptr_next = S->First;
		S->First = pomocna;
	}
}

/*
 *Funkcia ktora ulozi na zasobnik niekolko hodnot podla poctu argumentov
 *Parameter: Zasobnik, pocet dalsich parametrov a vkladanane hodnoty
 */
void myPushMul(stack *S ,int num, ...)
{
	va_list valist;
	int array[num];
    va_start(valist, num); //alokujeme pamat pre premene mnozstvo info
    for (int i = 0; i < num; i++) //ziskame vsetky argumenty a dame do pola
    {
    	array[i] = va_arg(valist, int);
    }
    for (int j = num - 1; j >= 0; j--) //a z pola pushneme na zasobnik aby sme to mali v zelanom poradi
    {
    	myPush(S, array[j]);
    }
    va_end(valist); //uvoolnime pamat
}

/*
 *Funkcia na popnutie vrcholu zasobniku
 *Parameter: Zasobnik
 */
void myPop(stack *S)
{
	if(S->First != NULL)
	{
		stack_element pomocna = S->First;
		S->First = S->First->ptr_next;
		myfree(pomocna);
	}
} 

/*
 *Funkcia vrati hodnotu vrcholu zasobniku, ak je stack prazdny, vrati NAN
 *Parameter: Zasobnik
 */
int myTop(stack *S)
{
	if(S->First != NULL)
	{
		return S->First->data;
	}
	return NAN;
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
		myfree(pomocna);
	}
}

/*
 *Funkcia na vypis celeho zasobniku, pre testovacie ucely
 *Parameter: zasobnik
 */
void showStack(stack *S)
{
	printf("\nXXXXXXXXX        Stack Begin       XXXXXXXXXXX \n");
	stack_element pomocna = S->First;
	while(pomocna != NULL)
	{
		printf("%d\n", pomocna->data);
		pomocna = pomocna->ptr_next;
	}
	printf("YYYYYYYYY          Stack End         YYYYYYYYYYY \n\n");
}

/*
int main()
{
	stack_init(&S);
	myPushMul(&S ,8, 1,2,3,4,5,80,75,87);
	myPush(&S, 505);
	myPush(&S, 7805);
	myPush(&S, 55);
	showStack(&S);
	destroyStack(&S);
 	return 0;
}*/
