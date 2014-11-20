/*
 *Hlavickovy subor pre scanner.c
 */

/* konstanty */
#define POCET_KLICOVYCH_SLOV 20

/* struktura pro token */
typedef struct
{
  tStav stav;
  char *data;
  int radek;
  int sloupec;
} tToken;

/* globalni promenne */
FILE *soubor; //scanner používa súbor
tToken token;
tERROR error;
int sloupec;
int radek;


/*
 *Deklaracie funkcii
 */

tToken get_token(void);
static void inicializuj_token(void);
static void vloz_znak_do_tokenu(int , int *);
static void napln_token(tStav);
static void vrat_se_o_znak(int);
static tStav porovnej_rezervovana_slova(char *);