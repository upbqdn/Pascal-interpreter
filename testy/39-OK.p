var
	x : integer;
	pocitadlo : integer;
	temp : integer;

function f() : integer;
begin
	write('Priechod cyklu c. ', pocitadlo);
	pocitadlo := pocitadlo +1;
	f := 42
end;

begin
	pocitadlo := 1;
	x:=5;
	while (x<5+6) do
		begin
			x := x + 1;
			write(x);
			temp := f()
		end
end.