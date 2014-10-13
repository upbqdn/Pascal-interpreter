//hlavicka pro lexikalni analyzator

//identifikator
#define ID       0

//klicova slova
#define SETZERO 10
#define READ    11
#define WRITE   12
#define WHILE   13

//operatory inkrementace a dekrementace
#define INC     20
#define DEC     21

//jednotlive znaky
#define LEFT_VINCULUM  30 // '{'
#define RIGHT_VINCULUM 31 // '}'
#define SEMICOLON      32 // ';'

//specialni znaky
#define END_OF_FILE    40

//chybove hlasky
#define LEX_ERROR    -1

//hlavicka funkce simulujici lexikalni analyzator
void setSourceFile(FILE *f);
int getNextToken(string *attr);
