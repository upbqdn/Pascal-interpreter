#define SYNTAX_OK     1
#define SYNTAX_ERROR -2
#define SEM_ERROR    -3

// hlavicka pro syntakticky analyzator
int parse(tSymbolTable *ST, tListOfInstr *instrList);
