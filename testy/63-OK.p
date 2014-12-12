{ spravny - volanie funkcie vo funkcii}
var
	xy :real;

function w(i : integer; c : real) : real;
begin
   w := 2.2
end;

function x(i : integer; c : real) : real;
var
	a : real;
begin
   a := w(i,c);
   x := a
end;

begin
	xy := x(5, 5.2) {xy == 2.2}
end.


