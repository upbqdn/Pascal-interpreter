{ spravny }

var
   a : integer;
   c : integer;
   r : real;

function x() : integer;
begin
   x := 0
end;

function z() : integer;
begin
   write('Hello.');
   z := 0
end;

function w(i :integer; c : real) : real;
var a : integer;
begin
   a := z();
   w := 5.0
end;

begin
   a := 42;
   r := w(a, 5.0);
   a := z();
   c := x();
   write (a)
end.


