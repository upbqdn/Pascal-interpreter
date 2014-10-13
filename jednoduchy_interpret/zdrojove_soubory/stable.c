#include <stdio.h>
#include <malloc.h>
#include "str.h"
#include "stable.h"

void tableInit(tSymbolTable *T)
// funkce inicializuje tabulku symbolu
{
  T->first = NULL;
}

int tableInsert(tSymbolTable *T, string *key, int varType)
// funkce vlozi do tabulky symbolu novy identifikator
{
  tTableItem *ptr;
  int found;
  ptr = T->first;
  found = 0;
  // nejprve zjistime, zda se v tabulce polozka s danym klicem jiz nenachazi
  while ((ptr != NULL) && (!found))
  {
    found = (strCmpString(&(ptr->key), key) == 0);
    if (!found) ptr = ptr->nextItem;
  } 
  if (found)
  {
     return 1;
  }
  else
  {
    // vlozime do tabulky novou polozku
    tTableItem *newItem;
    newItem = (tTableItem*) malloc(sizeof(tTableItem));
    strInit(&(newItem->key));
    strCopyString(&(newItem->key), key);
    newItem->data.varType = varType;
    newItem->nextItem = T->first;
    T->first = newItem;
    return 0;
  }
}

tData *tableSearch(tSymbolTable *T, string *key)
// pokud se dana polozka s klicem key v tabulce symbolu nachazi,
// funkce vrati ukazatel na data teto polozky, jinak vrati NULL
{
  tTableItem *ptr;
  int found;
  ptr = T->first;
  found = 0;
  while ((ptr != NULL) && (!found))
  {
    found = (strCmpString(&(ptr->key), key) == 0);
    if (!found) ptr = ptr->nextItem;
  } 
  if (found)
     return &(ptr->data);
  else
     return NULL;
}

void tableFree(tSymbolTable *T)
// funkce dealokuje tabulku symbolu
{
  tTableItem *ptr;
  while (T->first != NULL)
  {
     ptr = T->first;
     T->first = T->first->nextItem;
     // uvolnime klic
     strFree(&ptr->key);
     // nakonec uvolnime celou polozku
     free(ptr);
  }
}
