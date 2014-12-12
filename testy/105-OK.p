var
	f : string;
	i : integer;

begin
	f := 'abcd';
	i := length(f);
	write(i);
	i := length(''#10'');
	write(i);
	i := length('Pat');
	write(i)
end.