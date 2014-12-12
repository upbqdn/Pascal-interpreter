var
	a : integer;

function factorial(n : integer) : integer;
var
	tmp2 : integer;

begin
	if n = 1 then
		begin
			factorial := 1
		end
	else
		begin
			tmp2 := n - 1;
			tmp2 := factorial(tmp2);
			factorial := tmp2 * n
		end
end;

begin
	a := factorial(5);
	write(a)
end.