{ spravne escape sekvencie - ale nie som si isty }
var
	r : real;
	a : integer;

function w(i :integer; c : real) : real;
var a : string;
begin
	a := 'ahoj'#12'Martin'#32'jak se mas?'; {"Martin jak se mas?" je na novej strane}
   w := 42.0
end;


begin
	a := 5;
	r := w(a, 5.0)
end.