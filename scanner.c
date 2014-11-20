/* IFJ: scanner */

/*
textovy retazec NOT OK
komentar! OK
chyba OK
asci OK?
double OK

hlavickovy subor
*/


/* hlavickovy subor */
#include "header.h"
#include "scanner.h"


/*FUNKCIE*/

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
  for (int i = S_KLIC_BEGIN; i < POCET_KLICOVYCH_SLOV; i++)
    if (!(strcmp(slovo, klicova_slova[i]))) return i;
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
  error = vsechno_ok;
  
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
      else if	(c == ',')			stav = S_CARKA;
      else if	(c == '<')			stav = S_MENSI;
      else if	(c == '{')			{stav = S_LEVA_SLOZENA_ZAVORKA; break;}
      else if	(c == '}')			{stav = S_PRAVA_SLOZENA_ZAVORKA; break;}
      else if	(c == '(')			stav = S_LEVA_ZAVORKA;
      else if	(c == ')')			stav = S_PRAVA_ZAVORKA;
      else if	(c == '[')			stav = S_LEVA_HRANATA_ZAVORKA;
      else if 	(c == ']')			stav = S_PRAVA_HRANATA_ZAVORKA;
      else if	(c == '>')			stav = S_VETSI;
      else if	(c == '#')			{stav = S_MRIZKA; break;}
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
	stav = S_DOUBLE_POMOCDES1;
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
    
    case S_DOUBLE_POMOCDES1:
    {
      if (isdigit(c))
      {
	stav = S_DOUBLE_POMOCDES2;
	vloz_znak_do_tokenu(c, &i);
      }
      /*else if ((c == 'e') || (c == 'E'))
      {
	stav = S_DOUBLE_CELA_A_DESETINNA_CAST_A_EXPONENT;
	vloz_znak_do_tokenu(c, &i);
      }*/
      else
      {
	napln_token(stav);
	stav = S_CHYBA;
	vrat_se_o_znak((char) c);
      }
      break;
    }
    
    case S_DOUBLE_POMOCDES2:
    {
      if (isdigit(c))
      {
	stav = S_DOUBLE_POMOCDES2;
	vloz_znak_do_tokenu(c, &i);
      }
      else if ((c == 'e') || (c == 'E'))
      {
	stav = S_DOUBLE_POMOCDES3;
	vloz_znak_do_tokenu(c, &i);
      }
      else
      {
	napln_token(stav);
	stav = S_DOUBLE;
	vrat_se_o_znak((char) c);
      }
      break;
    }
    
    case S_DOUBLE_POMOCDES3:
    {
      if (isdigit(c))
      {
	stav = S_DOUBLE_POMOCDES3;
	vloz_znak_do_tokenu(c, &i);
      }
      else if ((c == '+') || (c == '-'))
      {
	stav = S_DOUBLE_POMOCDES4;
	vloz_znak_do_tokenu(c, &i);
      }
      else
      {
	napln_token(stav);
	stav = S_DOUBLE;
	vrat_se_o_znak((char) c);
      }
      break;
    }
    
    case S_DOUBLE_POMOCDES4:
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
	stav = S_END_OF_FILE;
	//vrat_se_o_znak((char) c);
      }
      else if (c != '}')
      {
	stav = S_LEVA_SLOZENA_ZAVORKA;
      }
      else
      {
	stav = S_KOMENTAR;
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
     // else if (c == '#')
     // {
	//stav = S_MRIZKA;
      //}
      else if (c == EOF)
      {
	stav = S_END_OF_FILE;
      }
      else
      {
	napln_token(stav);
	stav = S_END;
	//vrat_se_o_znak((char) c);
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
    

    case S_KLIC_BEGIN:      // klucove slova
    case S_KLIC_BOOLEAN:
    case S_KLIC_DO:
    case S_KLIC_ELSE:
    case S_KLIC_END:
    case S_KLIC_FALSE:
    case S_KLIC_FIND:
    case S_KLIC_FORWARD:
    case S_KLIC_FUNCTION:
    case S_KLIC_IF:
    case S_KLIC_INTEGER:
    case S_KLIC_READLN:
    case S_KLIC_REAL:
    case S_KLIC_SORT:
    case S_KLIC_STRING:
    case S_KLIC_THEN:
    case S_KLIC_TRUE:
    case S_KLIC_VAR:
    case S_KLIC_WHILE:
    case S_KLIC_WRITE:      // klucove slova potialto

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
    case S_KOMENTAR:
    {
      napln_token(stav);
      stav = S_END;
      vrat_se_o_znak((char) c);
      break;
    }
    
    case S_CHYBA:
    {
      vloz_znak_do_tokenu(c, &i);
      napln_token(stav);
      error = chyba_v_programu_v_ramci_lexikalni_analyzy;
      //vrat_se_o_znak((char) c);
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
