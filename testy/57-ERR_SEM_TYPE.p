{ aritmeticke operace ERR_SEM_TYPE }

var
   a : integer;
   b : real;
   c : boolean;
	
begin
	a := 5;
	b := 6.0;
	c := true;
	a := a-c; {zde chyba}
	
	
	write('doslo k chybe nehehe')
	
	
end.