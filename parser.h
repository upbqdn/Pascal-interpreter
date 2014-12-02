/* PREMENNE */
stack S; //zasobnik
tToken actToken; // aktualny token

typedef enum {
  G_VAR_DEC,
  FUNCTION_DEC
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
