var
	xy: real;

function x(i : integer; c : real) : real;
var
	a : integer;
begin
 	a := i;
 	write (i);
 	write (c);
 	readln(c);
 	readln(i);
 	write(i, c);
 	x := c	
end;

begin
	xy := x(5, 5.2) {xy == 2.2}
end.


