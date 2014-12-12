{ 2*3*()}
{a*(2+3+(b-b))}
{a := 2-     hadze ERR_LEX, co je chyba syntakticka}
{a := 2-5   ERR_LEX}
{a := 2*    ERR_SYNTAX}
{a := 2*2   OK}
{a := 2+2   ERR_LEX}
{a := (2/2/(2-3)+ 2+2)   ERR_SYNTAX}
var 
   a : integer;
   b: integer;

begin
   a:= 1;
   b:= 2;
   a := a*(2+3+(b-b))
end. 
