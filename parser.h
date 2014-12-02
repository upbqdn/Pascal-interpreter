/* PREMENNE */
stack S; //zasobnik
tToken actToken; // aktualny token

typedef enum {
  g_var_dec,
  function_dec
} tContext;

typedef struct {
  tERROR err;
  tContext context;
  tStav act_type;
  char *act_id;
} tSem_context;


bool parse();
void extractRule(tSem_context* sem_context);
void sem_check (tSem_context* sem_context);
