{ syntakticka_chyba }

var
   a : integer;
   c : boolean;
	c := true;
begin
	
	readln(a);
	if a > 5 then
		begin
			c := false;
		end
		
    if c = true then
		begin
		write('Cislo je mensi nez 5');
		end
	else
		begin
		write('Cislo je vetsi nez 5')
		end
	
end.