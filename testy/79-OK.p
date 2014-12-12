var 
	a : integer;

function f1() : integer;
begin
	while a > 0 do
		begin
			write(a);
			a := a - 1
		end;
	f1 := 2
end;

begin
	a := 10;
	a := f1()
end.