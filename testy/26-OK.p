var
	a : integer;
	b : real;
	c : string;
	d : boolean;
	e : boolean;

function f(a : string; b : string; c : string) : integer; forward;

function g() : string;
var 
	a : string;
	b : string;
	c : integer;

begin
	a := 'aaaaa';
	b := 'bbbb';
	c := f(a, b, a)
end;


function f(a : string; b : string; c : string) : integer;
begin
	f := 42
end;

begin
end.