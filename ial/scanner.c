/* IFJ: scanner */

/* hlavickove soubory */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* konstanty */
#define POCET_KLICOVYCH_SLOV 20

/* chyby a chybove kody */
typedef enum ERROR
{
  vsechno_ok,							// 0
  chyba_v_programu_v_ramci_lexikalni_analyzy,			// 1
  chyba_v_programu_v_ramci_syntakticke_analyzy,		// 2
  semanticka_chyba_v_programu,					// 3
  semanticka_chyba_typove_kompatibility,			// 4
  ostatni_semanticke_chyby,					// 5
  behova_chyba_pri_nacitani_ciselne_hodnoty_ze_vstupu,	// 6
  behova_chyba_pri_praci_s_neinicializovanou_promennou,	// 7
  behova_chyba_deleni_nulou,					// 8
  ostatni_behove_chyby,					// 9
  interni_chyba_interpretu,					// 99
} tERROR;

/* jednotlive stavy automatu */
typedef enum
{
    S_START,					//pocatecni stav
    S_END,					//koncovy stav
    S_CHYBA,					//chybovy stav
    S_INTEGER,					//cislo typu integer
    S_KLICOVE_SLOVO,				// klicove slovo
    S_DOUBLE,					//cislo typu double
    S_DOUBLE_CELA_A_DESETINNA_CAST,		//cislo typu double
    S_DOUBLE_CELA_CAST_A_EXPONENT,		//cislo typu double
    S_DOUBLE_CELA_A_DESETINNA_CAST_A_EXPONENT,//cislo typu double
    S_DOUBLE_POMOC,				//pomocny double
    S_IDENTIFIKATOR,				//identifikator
    S_PLUS,					// +
    S_MINUS,					// -
    S_KRAT,					// *
    S_DELENO,					// /
    S_UKAZATEL,					// ^
    S_MENSI,					// <
    S_MENSI_NEBO_ROVNO,				// <=
    S_VETSI,					// >
    S_VETSI_NEBO_ROVNO,				// >=
    S_ROVNO,					// =
    S_NEROVNO,					// <>
    S_STREDNIK,					// ;
    S_DVOJTECKA,				// :
    S_PRIRAZENI,				// :=
    S_TECKA,					// .
    S_DVE_TECKY,				// ..
    S_CARKA,					// ,
    S_LEVA_ZAVORKA,				// (
    S_PRAVA_ZAVORKA,				// )
    S_LEVA_HRANATA_ZAVORKA,			// [
    S_PRAVA_HRANATA_ZAVORKA,			// ]
    S_LEVA_SLOZENA_ZAVORKA,			// {
    S_PRAVA_SLOZENA_ZAVORKA,			// }
    S_RETEZEC,					// string
    S_MRIZKA,					// #
    S_ESCAPE_SEKVENCE,				// #0-255
    S_END_OF_FILE,				// EOF
    
} tStav;

/* struktura pro token */
typedef struct
{
  tStav stav;
  char *data;
  int radek;
  int sloupec;
} tToken;

/* globalni promenne */
extern tToken token;
tERROR error;
int sloupec = 0;
int radek = 0;

/* prototypy funkci */
tToken get_token(void);
static void inicializuj_token(void);
static void vloz_znak_do_tokenu(int znak, int *i);
static void napln_token(tStav stav);
static void vrat_se_o_znak(int znak);
static tStav porovnej_rezervovana_slova(char *slovo);

/* klicova slova */
const char *klicova_slova[POCET_KLICOVYCH_SLOV] =
{
    "begin\0", "boolean\0", "do\0", "else\0", "end\0", "false\0", "find\0", "forward\0", "function\0", "if\0", "integer\0", "readln\0", "real\0", "sort\0", "string\0", "then\0", "true\0", "var\0", "while\0", "write\0" 
};

static void vloz_znak_do_tokenu(int znak, int *i)
{
  if ((token.data = (char *) realloc(token.data, (*i) + 2)))
  {
    token.data[(*i) + 1] = '\0';
    token.data[(*i)] = znak;
    (*i)++;
  }
  else error = interni_chyba_interpretu; /* interni chyba prekladace */
}

static void vrat_se_o_znak(int znak)
{
  if (!isspace(znak)) ungetc(znak, soubor);
  if (isprint(znak)) sloupec--;
}

static tStav porovnej_rezervovana_slova(char *slovo)
{
  for (int i = 0; i < POCET_KLICOVYCH_SLOV; i++)
    if ((strcmp(slovo, klicova_slova[i]))) return S_KLICOVE_SLOVO;
    return S_IDENTIFIKATOR;
}

static void inicializuj_token(void)
{
  token.stav = S_START;
  token.data = NULL;
  token.radek = radek;
  token.sloupec = sloupec;
}

static void napln_token(tStav stav)
{
  token.stav = stav;
}


tToken get_token(void)
{
  tStav stav = S_START;
  int i = 0;
  int c;
  bool konec = false;
  
  inicializuj_token();
  
  while (!(konec))
  {
  c = getc(soubor);
  switch (stav)
  {
    case S_START:
    {
      token.radek = radek;
      token.sloupec = sloupec;
      
      if 	((isalpha(c)) || (c == '_'))	stav = S_IDENTIFIKATOR;
      else if	(isdigit(c))			stav = S_INTEGER;
      else if	(c == EOF)			stav = S_END_OF_FILE;
      else if 	(c == '+')			stav = S_PLUS;
      else if	(c == '-')			stav = S_MINUS;
      else if	(c == '*')			stav = S_KRAT;
      else if	(c == '/')			stav = S_DELENO;
      else if	(c == '=')			stav = S_ROVNO;
      else if	(c == '.')			stav = S_TECKA;
      else if	(c == ':')			stav = S_DVOJTECKA;
      else if	(c == ';')			stav = S_STREDNIK;
      else if	(c == '^')			stav = S_UKAZATEL;
      else if	(c == '<')			stav = S_MENSI;
      else if	(c == '{')			stav = S_LEVA_SLOZENA_ZAVORKA;
      else if	(c == '}')			stav = S_PRAVA_SLOZENA_ZAVORKA;
      else if	(c == '(')			stav = S_LEVA_ZAVORKA;
      else if	(c == ')')			stav = S_PRAVA_ZAVORKA;
      else if	(c == '[')			stav = S_LEVA_HRANATA_ZAVORKA;
      else if 	(c == ']')			stav = S_PRAVA_HRANATA_ZAVORKA;
      else if	(c == '>')			stav = S_VETSI;
      else if	(c == '\'')		{	stav = S_RETEZEC;
						break;
      }
      else if	(isspace(c))		{	stav = S_START;
						break;
      }
      else 				{	stav = S_CHYBA;
						break;
      }
      vloz_znak_do_tokenu(c, &i);
      break;
    }
    
    case S_IDENTIFIKATOR:
    {
      if ((isalpha(c) || isdigit(c) || (c == '_')))
      {
	stav = S_IDENTIFIKATOR;
	vloz_znak_do_tokenu(c, &i);
      }
      else
      {
	token.stav = porovnej_rezervovana_slova(token.data);
	stav = S_END;
	vrat_se_o_znak((char) c);
      }
      break;
    }
    
    case S_INTEGER:
    {
      if (isdigit(c))
      {
	stav = S_INTEGER;
	vloz_znak_do_tokenu(c, &i);
      }
      else if (c == '.')
      {
	stav = S_DOUBLE_CELA_A_DESETINNA_CAST;
	vloz_znak_do_tokenu(c, &i);
      }
      else if ((c == 'e') || (c == 'E'))
      {
	stav = S_DOUBLE_CELA_CAST_A_EXPONENT;
	vloz_znak_do_tokenu(c, &i);
      }
      else
      {
	napln_token(stav);
	stav = S_END;
	vrat_se_o_znak((char) c);
      }
      
      break;
	
    }
    
    case S_DOUBLE_CELA_A_DESETINNA_CAST:
    {
      if (isdigit(c))
      {
	stav = S_DOUBLE;
	vloz_znak_do_tokenu(c, &i);
      }
      else if ((c == 'e') || (c == 'E'))
      {
	stav = S_DOUBLE_CELA_A_DESETINNA_CAST_A_EXPONENT;
	vloz_znak_do_tokenu(c, &i);
      }
      else
      {
	napln_token(stav);
	stav = S_CHYBA;
	vrat_se_o_znak((char) c);
      }
      break;
    }
      
    case S_DOUBLE_CELA_CAST_A_EXPONENT: 
    {
      if (isdigit(c))
      {
	stav = S_DOUBLE;
	vloz_znak_do_tokenu(c, &i);
      }
      else if ((c == '+') || (c == '-'))
      {
	stav = S_DOUBLE_POMOC;
	vloz_znak_do_tokenu(c, &i);
      }
      else
      {
	napln_token(stav);
	stav = S_CHYBA;
	vrat_se_o_znak((char) c);
      }
      break;
    }
    
    case S_DOUBLE_POMOC:
    {
      if (isdigit(c))
      {
	stav = S_DOUBLE;
	vloz_znak_do_tokenu(c, &i);
      }
      else
      {
	napln_token(stav);
	stav = S_CHYBA;
	vrat_se_o_znak((char) c);
      }
      break;
    }
    
    case S_DOUBLE_CELA_A_DESETINNA_CAST_A_EXPONENT:
    {
      if (isdigit(c))
      {
	stav = S_DOUBLE;
	vloz_znak_do_tokenu(c, &i);
      }
      else if ((c == '+') || (c == '-'))
      {
	stav = S_DOUBLE_POMOC;
	vloz_znak_do_tokenu(c, &i);
      }
      else
      {
	napln_token(stav);
	stav = S_CHYBA;
	vrat_se_o_znak((char) c);
      }
      break;
    }
    
    case S_DOUBLE:
    {
      if (isdigit(c))
      {
	stav = S_DOUBLE;
	vloz_znak_do_tokenu(c, &i);
      }
      else
      {
	napln_token(stav);
	stav = S_END;
	vrat_se_o_znak((char) c);
      }
      break;
    }
    
    case S_MENSI:
    {
      if (c == '=')
      {
	stav = S_MENSI_NEBO_ROVNO;
	vloz_znak_do_tokenu(c, &i);
      }
      else if (c == '>')
      {
	stav = S_NEROVNO;
	vloz_znak_do_tokenu(c, &i);
      }
      {
	napln_token(stav);
	stav = S_END;
	vrat_se_o_znak((char) c);
      }
      break;
    }
    
    case S_VETSI:
    {
      if (c == '=')
      {
	stav = S_VETSI_NEBO_ROVNO;
	vloz_znak_do_tokenu(c, &i);
      }
      else
      {
	napln_token(stav);
	stav = S_END;
	vrat_se_o_znak((char) c);
      }
      break;
    }

    case S_DVOJTECKA:
    {
      if (c == '=')
      {
	stav = S_PRIRAZENI;
	vloz_znak_do_tokenu(c, &i);
      }
      else
      {
	napln_token(stav);
	stav = S_END;
	vrat_se_o_znak((char) c);
      }
      break;
    }
    
    case S_TECKA:
    {
      if (c == '.')
      {
	stav = S_DVE_TECKY;
	vloz_znak_do_tokenu(c, &i);
      }
      else
      {
	napln_token(stav);
	stav = S_END;
	vrat_se_o_znak((char) c);
      }
      break;
    }
 
    case S_LEVA_SLOZENA_ZAVORKA:
    {
      if (c == EOF)
      {
	napln_token(stav);
	stav = S_CHYBA;
	vrat_se_o_znak((char) c);
      }
      else if (c != '}')
      {
	stav = S_LEVA_SLOZENA_ZAVORKA;
      }
      else
      {
	stav = S_START;
	vrat_se_o_znak((char) c);
      }
      break;
    }
    
    case S_PRAVA_SLOZENA_ZAVORKA:
    {
      stav = S_CHYBA;
      break;
    }
    
    case S_RETEZEC:
    {
      if (c != '\'')
      {
	stav = S_RETEZEC;
	vloz_znak_do_tokenu(c, &i);
      }
      else if (c == EOF)
      {
	stav = S_CHYBA;
      }
      else
      {
	napln_token(stav);
	stav = S_END;
	vrat_se_o_znak((char) c);
      }
      break;
    }
    
    case S_MRIZKA:
    {
      if (isdigit(c))
      {
	stav = S_ESCAPE_SEKVENCE;
	vloz_znak_do_tokenu(c, &i);
      }
      else
      {
	napln_token(stav);
	stav = S_CHYBA;
	vrat_se_o_znak((char) c);
      }
      break;
    }
    
    case S_ESCAPE_SEKVENCE: /* asci hodnota 0 - 255 POZOR NEosetruji*/
    {
      if (isdigit(c))
      {
	stav = S_ESCAPE_SEKVENCE;
	vloz_znak_do_tokenu(c, &i);
      }
      else
      {
	napln_token(stav);
	stav = S_END;
	vrat_se_o_znak((char) c);
      }
      break;
    }
    
    case S_KLICOVE_SLOVO:
    case S_CARKA:
    case S_STREDNIK:
    case S_PRIRAZENI:
    case S_DVE_TECKY:
    case S_PLUS:
    case S_MINUS:
    case S_KRAT:
    case S_DELENO:
    case S_ROVNO:
    case S_LEVA_ZAVORKA:
    case S_PRAVA_ZAVORKA:
    case S_LEVA_HRANATA_ZAVORKA:
    case S_PRAVA_HRANATA_ZAVORKA:
    case S_VETSI_NEBO_ROVNO:
    case S_MENSI_NEBO_ROVNO: 
    case S_NEROVNO:
    case S_UKAZATEL:
    case S_END_OF_FILE:
    {
      napln_token(stav);
      stav = S_END;
      vrat_se_o_znak((char) c);
      break;
    }
    
    case S_CHYBA:
    {
      error = chyba_v_programu_v_ramci_lexikalni_analyzy;
      konec = true;
      break;
    }
    case S_END:
    {
      vrat_se_o_znak((char) c);
      konec = true;
      break;
    }     
    }
    if (error) break;
    if (c == '\n')
    {
      radek++;
      sloupec = 1;
    }
    else if (isprint(c))
      sloupec++;
    
    }
    return token;
  }
