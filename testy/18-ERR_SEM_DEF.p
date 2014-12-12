{ prace s nedeklarovanou promenou }

var
   a : integer;
   
	
begin
	
	readln(a);
	if a > 5 then
		begin
			c := false;
		end
		
    if c = true then
		begin
		writeln('Cislo je mensi nez 5');
		end
	else
		begin
		writeln('Cislo je vetsi nez 5')
		end
	
{ nesspravny - lokalna premenna sa vola rovnako ako funkcia }
function x() : boolean;
var
	x : integer;
begin
   readln(aa);
   x := true
end;

begin
end.
