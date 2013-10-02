#/usr/bin/perl -w

# This script creates de contact matrix given  pdb box file
my $Dir = "raw";
my $DirDest = "contacts";
my $FileName;
my $x,$y,$z;
my @X,@Y,@Z;
my @matrix = ();

opendir (DIR,$Dir) || die "Cannot open directory";
while ($FileName = readdir(DIR))
{
	if ($FileName !~ /^res/){next;}
	if ($FileName =~ /matrix$/){next;}
	open(FILE,"${Dir}/${FileName}") || die "Could not open the given file";
	@X = ();
	@Y = ();
	@Z = ();
	
	while (my $Line = <FILE>)
	{
		if ($Line !~ /^ATOM/){next;}
		chomp $Line;
		$x = substr($Line, 32, 6);
		$y = substr($Line, 40, 6);
		$z = substr($Line, 48, 6);
		
		push(@X,$x);
		push(@Y,$y);
		push(@Z,$z);
	}
	close(FILE);

	my $MatrixFile = "${DirDest}/${FileName}.matrix";
	open(FILE2,">$MatrixFile") || die "Could not open the given file";
	for(my $i=0;$i<=$#X;$i++)
	{
		for (my $j=0;$j<$i;$j++)
		{
			if ($i == $j || $i == $j+1 || $i == $j-1)
			{
				next;
			}
			elsif ($X[$i] == $X[$j] && $Y[$i] == $Y[$j])
			{
				if ($Z[$i] == $Z[$j]+1 || $Z[$i] == $Z[$j]-1)
				{
					print FILE2 " 1";
				}
				else {print FILE2 " 0";}
			}
			elsif ($X[$i] == $X[$j] && $Z[$i] == $Z[$j])
			{
				if ($Y[$i] == $Y[$j]+1 || $Y[$i] == $Y[$j]-1)
				{
					print FILE2 " 1";
				}
				else {print FILE2 " 0";}
			}
			elsif ($Y[$i] == $Y[$j] && $Z[$i] == $Z[$j])
			{
				if ($X[$i] == $X[$j]+1 || $X[$i] == $X[$j]-1)
				{
					print FILE2 " 1";
				}
				else {print FILE2 " 0";}
			}
			else 
			{
				print FILE2 " 0";
			}
		}
		print FILE2 "\n";
	}
	close(FILE2);
}
exit;
