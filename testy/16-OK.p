{ spravny }

var
   a : integer;
   c : boolean;

function f() : integer;
var
	r : real;
	a : integer;

begin
   f := 42;
	c := true;
	readln(a);
	if a > 5 then
		begin
			c := false
		end
	else
      begin
      end;	
    if c = true then
		begin
         write('Cislo je mensi nez 5')
		end
	else
		begin
         write('Cislo je vetsi nez 5')
		end
end;

begin
end.
