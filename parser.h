/*                @Project: IFJ14
-----------------------------------------------------
  @Author: Marek Bielik   xbieli05@stud.fit.vutbr.cz
  @Author: Filip Gulan    xgulan00@stud.fit.vutbr.cz
  @Author: Filip Ježovica xjezov01@stud.fit.vutbr.cz
  @Author: Luboš Matuška  xmatus29@stud.fit.vutbr.cz
  @Author: Eduard Rybár   xrybar04@stud.fit.vutbr.cz
-----------------------------------------------------
*/



/* PREMENNE */
stack S; //zasobnik
tToken actToken; // aktualny token

#define DEFINED (0)

typedef enum {
  G_VAR_DEC,        //kontext deklaracii glob. premennych
  FUNCTION_DEC,     //kontext deklaracii funkcii
  FUNC_ARG_DEC      //kontext deklaracii argumentov funkcie
} tContext;

typedef struct {
  tERROR err;
  tContext context;
  tStav act_type;
  char *act_id;
  char *act_fun;
} tSem_context;

typedef enum {
  rem_id,
  for_id
} tId_sign;

bool parse();
void extractRule(tSem_context* sem_context);
void sem_check (tSem_context* sem_context);
