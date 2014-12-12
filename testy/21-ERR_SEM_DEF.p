{ nesspravny - deklaracia sa nezhoduje s definiciou }
function y():integer;forward;
function x() : boolean;
var
	aa : integer;
begin
   readln(aa);
   x := true
end;

function y() :real;
var
	tmp : integer;
begin
   write(tmp);
   y := true
end;

begin
end.