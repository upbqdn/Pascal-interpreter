var b : boolean;
	a : real;
	c : integer;
	tmp : real;

begin
	a := 20.0;
	c := 2;
	while a / c > 1.0 do
	begin
		a := a / c;
		write('a = ', a , ''#010'')
	end
end.