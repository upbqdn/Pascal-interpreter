/*
-----------------------------------------------------
  @Author: Marek Bielik		xbieli05@stud.fit.vutbr.cz
  @Author: Filip Gulan		xgulan00@stud.fit.vutbr.cz
  @Author: Filip Ježovica	xjezov01@stud.fit.vutbr.cz
  @Author: Luboš Matuška	xmatus29@stud.fit.vutbr.cz
  @Author: Eduard Rybár		xrybar04@stud.fit.vutbr.cz
-----------------------------------------------------          
*/

/* instruction list - hlavickovy subor  */

typedef struct
{
  int AKCIA;  // instrukcia
  void *ADDR_KDE;
  void *ADDR_PRVA;
  void *ADDR_DRUHA;
} tInstrukcia;

typedef struct tPrvokListu {
    struct tPrvokListu *dalsiPtr;
    tInstrukcia Instrukcia;
} *tPrvokListuPtr;

typedef struct { 
    tPrvokListuPtr Aktivna;
    tPrvokListuPtr Prva;
    tPrvokListuPtr Posledna;
} tListInstrukcii;

void InitInstrList (tListInstrukcii *I_List);