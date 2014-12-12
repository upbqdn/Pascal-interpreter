var
	s : string;
	i : integer;

begin
	s := 'abcd';
	i := length(s);
	write(s, i);
	write(''#10'');
	s := sort(s);
	write(s);
	write(''#10'')
end.