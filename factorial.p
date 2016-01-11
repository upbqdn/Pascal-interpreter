var
	a : integer;
	res : integer;

{ function for calcuating factorial}
function factorial(n : integer) : integer;
var
	temp_result : integer;
	decremented_n : integer;
begin
	if n < 2 then
	begin
		factorial := 1
	end
	else
	begin
		decremented_n := n - 1;
		temp_result := factorial(decremented_n);
		factorial := n * temp_result
	end
end;

{ main body }
begin
	write('Enter a number ');
	readln(a);

	if a < 0 then
	begin
		write('You are kidding'#10'')
	end
	else
	begin
		res := factorial(a);
		write('Result is: ', res, ''#10'')
	end
end.
