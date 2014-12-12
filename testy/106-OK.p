{OK}
var
	str1 : string;
	str2 : string;
	n : integer;
begin
	str1 := 'Toto je nejaky text';
	write(str1,''#10''); { stdout: "Toto je nejaky text" }
	str2 := copy(str1, 9, 11); 
	write(str1, ''#10'', str2, ''#10''); {  }
	str2 := '22 '#10's   ';
	
	n := find(str2, ''#10' ');
	write('Prvy index na ktorom je retazec text v str2 ', str2,' je: ' , n, ''#10'')
end.