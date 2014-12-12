{ ERR_SYNTAX}

var
	a : integer;

function f (a : integer; b : real) : integer;
begin
	f := a
end;


begin
	a := f(2 + 5, 42.5)
end.