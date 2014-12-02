/*                @Project: IFJ14
-----------------------------------------------------
  @Author: Marek Bielik   xbieli05@stud.fit.vutbr.cz
  @Author: Filip Gulan    xgulan00@stud.fit.vutbr.cz
  @Author: Filip Ježovica xjezov01@stud.fit.vutbr.cz
  @Author: Luboš Matuška  xmatus29@stud.fit.vutbr.cz
  @Author: Eduard Rybár   xrybar04@stud.fit.vutbr.cz
-----------------------------------------------------
*/

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


extern tToken actToken; // global TOKEN


/*
 *Deklaracie funkcii
 */

tToken get_token(void);
void inicializuj_token(void);
void vloz_znak_do_tokenu(int , int *);
void napln_token(tStav);
void vrat_se_o_znak(int);
tStav porovnej_rezervovana_slova(char *);