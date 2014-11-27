{ CIFSUCET.PAS              Copyright (c) fake man, gazda					  }
{ Program urèí ciferný súèet n-èísel plus e¹te nieèo k tomu        	}
{                                                                   }
{ Author: fake man, gazda                                           }
{ Date  : 15.12.2010                           http://www.trsek.com }

var n:integer; i:integer; cislo:integer; k:integer; z:integer; zsucet:integer;
ksucet:integer; sucet:integer;

function gabs(a : integer) : integer; forward;

function gdiv(a : integer; b : integer) : integer;
var sign : integer; tmp : integer; 
begin
	if a * b < 0 then
		begin
			sign := 0 - 1
		end
	else
		begin
			sign := 1
		end;

	a := gabs(a);
	b := gabs(b);

	gdiv := 0;
	tmp := b;

	while tmp < a do
		begin
			tmp := tmp + b;
			gdiv := gdiv + 1
		end;
	gdiv := gdiv * sign
end;

function gmod(a : integer; b : integer) : integer;
begin
	gmod := a - b * gdiv(a, b)
end;

function gabs(a : integer) : integer;
begin
	if a < 0 then
		begin
			a := a * (0 - 1)
		end
	else
		begin
		end; { if-else }
	gabs := a
end;

begin
  write('Zadaj pocet cisel ktorych chces ciferny sucet: ');
	readln(n);

  k:=0; z:=0; zsucet:=0; ksucet:=0; sucet:=0;
	
	i := 1;
	while i <= n do
		begin
			write('Napis ', i, '. cislo: ');
			readln(cislo);
			if cislo > 0 then 
				begin
					k := k + 1;
					while cislo <> 0 do
						begin
							ksucet := ksucet + gmod(cislo, 10);
							cislo := gdiv(cislo, 10)
						end { while }
				end 
			else
				begin
				end; { if-else }

			if cislo < 0 then
				begin
					z := z + 1;
					while cislo <> 0 do
						begin
							zsucet := zsucet + gabs(gmod(cislo, 10));
							cislo := gdiv(cislo, 10)
						end { while }
				end
			else
				begin
				end; { if-else }

			i := i + 1
		end;
	sucet := zsucet + ksucet;
	write('Zapornych cisel je ', z,' ich sucet je ', zsucet,''#10'');
	write('Kladnych cisel je ', k,' ich sucet je ', ksucet,''#10'');
	write('Sucet vsetkych cisel je ',sucet,''#10'')
end.
