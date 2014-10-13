// definice jednotlivych typu
#define TYPE_INT     1


typedef struct
{
  int varType;  // typ dane promenne, v nasem pripade vzdy int
  int varValue; // pro ruzne typy nutnost resit pomoci unie
} tData;
  
typedef struct tableItem
{
  string key;                  // klic, podle ktereho se bude vyhledavat = nazev identifikatoru
  tData data;                  // data, ktera jsou ke klici pridruzena
  struct tableItem *nextItem;  // ukazatel na dalsi prvek tabulky symbolu
} tTableItem;

typedef struct
{
  struct tableItem *first;
} tSymbolTable;

void tableInit(tSymbolTable *T);
int tableInsert(tSymbolTable *T, string *key, int varType);
tData *tableSearch(tSymbolTable *T, string *key);
void tableFree(tSymbolTable *T);
