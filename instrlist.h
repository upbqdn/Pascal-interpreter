/*                @Project: IFJ14
-----------------------------------------------------
  @Author: Marek Bielik   xbieli05@stud.fit.vutbr.cz
  @Author: Filip Gulan    xgulan00@stud.fit.vutbr.cz
  @Author: Filip Ježovica xjezov01@stud.fit.vutbr.cz
  @Author: Luboš Matuška  xmatus29@stud.fit.vutbr.cz
  @Author: Eduard Rybár   xrybar04@stud.fit.vutbr.cz
-----------------------------------------------------
*/

/* instruction list - hlavickovy subor  */


/* STAVY AKCII */
/*
 *Enumarecie instrukcii
 */
typedef enum
{
    I_STOP = 0,     //0
    I_NICNEROBA,    //1
    I_READ,         //2     
    I_WRITE_INT,    //3 
    I_WRITE_IDE,    //4
    I_WRITE_DOU,    //5
    I_WRITE_STR,    //6
    I_WRITE_BOO,    //7
    I_PREC,         //8    
    I_IDENT,        //9     
    I_PRIRAD,       //10
    I_PLUS,         //11    
    I_MINUS,        //12    
    I_KRAT,         //13     
    I_DELENO,       //14  
    I_ALLOC_INT,    //15
    I_ALLOC_DOU,    //16
    I_ALLOC_BOO,    //17
    I_ALLOC_STR,    //18
    I_VAR_ZARAZKA,  //19
    I_VETSI,        //20
    I_MENSI,        //21
    I_MENSIROVNO,   //22   
    I_VETSIROVNO,   //23
    I_ROVNO,        //24
    I_NEROVNO,      //25
                        // instrukcie na funkcie
    I_JMPF_KEY,     //26
    I_JMPF_KEY_S,   //27
    I_JMP_S,        //28
    I_JMP_BACK,     //29    
    I_FUN_PRIRAD_PARAM, //30
    I_RUNFUN_COPY,  //31
    I_RUNFUN_PARAM, //32
    I_FRAME_DEL,    //33
    I_PODM_JMP,     //34
    I_JMP,          //35


} tINSTR;


/* JEDNA INSTRUKCIA */
typedef struct{
    tINSTR AKCIA;        // ** 1 > AKCIA AKA SA MA VYKONAT {int konstanta} : JMP, +,-,*,/,aloc,....
    void *ADDR_KDE;   // ** 2 > ADDR_KDE   > KAM SA MA UKLADAT nieco, pri jednoadresovych instrukciach sa pouziva univerzalne ostatne budu NULL!
    void *ADDR_PRVA;  // ** 3 > ADDR_PRVA  > ODKIAL sa cita 1 adresa
    void *ADDR_DRUHA; // ** 4 > ADDR_DRUHA > ODKIAL sa cita 2 adresa
} tInstrukcia;      // ** ::: vsetky adresy su typu void * >> pri pouzivani pouzi PRETYPOVANIE :::


/* toto je jeden prvok instrukcnej pasky {1 instrukcia + ukazatel na dalsiu} */
typedef struct tPrvokListu{
    struct tPrvokListu *dalsiPtr; // ukazatel na dalsiu instrukciu na paske > posledny ukazuje na NULL
    tInstrukcia Instrukcia;       // tu je ulozena cela instrukcia (struktura vid hore)
} *tPrvokListuPtr;


/* toto je cela INSTRUKCNA PASKA, vieme pristupit na aktivnu instrukciu, prvu a poslednu */
typedef struct{
    tPrvokListuPtr Aktivna;
    tPrvokListuPtr Prva;
    tPrvokListuPtr Posledna;
} tListInstrukcii;


extern tListInstrukcii INSTR_PASKA; // INSTRUKCNA PASKA GLOBALNA

/* Deklaracia funkcii nad INSTRUKCNOU PASKOU */
void InitInstrList (tListInstrukcii *I_List);
void DestroyInstrList (tListInstrukcii *I_List);
void InstrInsert (tListInstrukcii *I_List, tInstrukcia DataOfInstr);
void InstrStart(tListInstrukcii *I_List);
void InstrGoto(tListInstrukcii *I_List, void *gotoAddr);
void InstrDalsia(tListInstrukcii *I_List);
void *InstrDajPosledPoz(tListInstrukcii *I_List);
void *InstrDajPoz(tListInstrukcii *I_List);
tInstrukcia *DajInstr(tListInstrukcii *I_List);
void NaplnInstr(tINSTR AKCIA, void *ADDR_KDE, void *ADDR_PRVA, void *ADDR_DRUHA);
void *spracuj(tStav TIPSTAV, char* data);
