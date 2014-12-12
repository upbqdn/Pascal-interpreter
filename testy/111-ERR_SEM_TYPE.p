{ ERR_SEM_DEF copy-chyba 1 parameter }
var
	str1 : string;
	str2 : string;
	n : integer;
begin
	str1 := 'Toto je nejaky text';
	write(str1,''#10''); { stdout: "Toto je nejaky text" }
	str2 := copy(str1, 9)
end.