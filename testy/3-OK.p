{ spravny }

var
	a : integer;

function x() : boolean;
var
	aa : integer;
	bb : string;
begin
   readln(aa);
   x := true
end;

function z() : integer; forward;
function w(i : integer; c : real) : real; forward;

function z() : integer;
begin
   z := 1
end;

function w(i : integer; c : real) : real;
begin
   w := 1.0
end;

begin
   a := 5;
	write(a);
	begin
      write(a)
	end
end.


