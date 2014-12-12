{ prace s nedeklarovanou promenou }

var
   a : integer;
   b : integer;
	
begin
	
	readln(a);
	readln(b);
	readln(c);
	write('doslo k chybe nehehe')
	
	
{ nesspravny - pokus o deklaraciu uz definovanej fcie }
function x() : boolean;
var
	aa : integer;
begin
   readln(aa);
   x := true
end;

function x() : boolean; forward;

begin
end.
