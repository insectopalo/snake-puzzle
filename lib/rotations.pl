#/usr/bin/perl -w

# This script generates all the 24 rotationa possibilities for each solution

my $Dir = "raw";
my $FileName;
my $x,$y,$z;
my @X,@Y,@Z;



opendir (DIR,$Dir) || die "Cannot open directory";
while ($FileName = readdir(DIR))
{
	if ($FileName !~ /^res/){next;}
	if ($FileName !~ /pdb$/){next;}
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
	

# Export the "no rotation yet" file
	my $ExtraDir = "rotamers";
	my $NewFile = "${ExtraDir}/${FileName}.r01.pdb";
	open(WRITING, ">$NewFile") || die "Could not open file for writing...";
	
	$j=0;
	foreach(@X)
	{
		print WRITING 'ATOM'.' 'x(7-length($j+1)),$j+1,'  BOX BOX     1       ';
		printf WRITING '%.3f   %.3f   %.3f',$X[$j],$Y[$j],$Z[$j];
		print WRITING "\n";
		$j++;
	}
	for($j=0;$j<63;$j++)
	{
		printf WRITING 'CONECT   %2d   %2d',$j+1,$j+2;
		print WRITING "\n";
	}
	close WRITING;


# First 3 rotations through xy axis
	my $temp;
	for (my $i=0;$i<3;$i++)
	{
		$j=0;
		foreach (@X)
		{
			$temp = $X[$j];
			$X[$j]= $Y[$j]*(-1)+3;
			$Y[$j]= $temp;
			$j++;
		}
		my $aux = $i+2;
		my $NewFile2 = "${ExtraDir}/${FileName}.r0${aux}.pdb";
		open(WRITING, ">$NewFile2") || die "Could not open file for writing...";
		
		$j=0;
		foreach(@X)
		{
			print WRITING 'ATOM'.' 'x(7-length($j+1)),$j+1,'  BOX BOX     1      ';
			printf WRITING '% .3f  % .3f  % .3f',$X[$j],$Y[$j],$Z[$j];
			print WRITING "\n";
			$j++;
		}
		for($j=0;$j<63;$j++)
		{
			printf WRITING 'CONECT   %2d   %2d',$j+1,$j+2;
			print WRITING "\n";
		}
		close WRITING;
	}

# Another 3 rotations through yz axis
	for ($i=3;$i<6;$i++){
		$j=0;
		foreach (@Y)
		{
			$temp = $Y[$j];
			$Y[$j]= $Z[$j]*(-1)+3;
			$Z[$j]= $temp;
			$j++;
		}
		$aux = $i+2;
                $NewFile2 = "${ExtraDir}/${FileName}.r0${aux}.pdb";
		open(WRITING, ">$NewFile2") || die "Could not open file for writing...";
		
		$j=0;
		foreach(@X)
		{
			print WRITING 'ATOM'.' 'x(7-length($j+1)),$j+1,'  BOX BOX     1      ';
			printf WRITING '% .3f  % .3f  % .3f',$X[$j],$Y[$j],$Z[$j];
			print WRITING "\n";
			$j++;
		}
		for($j=0;$j<63;$j++)
		{
			printf WRITING 'CONECT   %2d   %2d',$j+1,$j+2;
			print WRITING "\n";
		}
		close WRITING;
	}

# More rotations
	for ($i=6;$i<9;$i++){
		$j=0;
		foreach (@X)
		{
			$temp = $X[$j];
			$X[$j]= $Y[$j]*(-1)+3;
			$Y[$j]= $temp;
			$j++;
		}
		$aux = $i+2;
                $NewFile2 = "${ExtraDir}/${FileName}.r0${aux}.pdb";
		$NewFile2 = "${ExtraDir}/${FileName}.r${aux}.pdb" if ($aux==10);
		open(WRITING, ">$NewFile2") || die "Could not open file for writing...";
		
		$j=0;
		foreach(@X)
		{
			print WRITING 'ATOM'.' 'x(7-length($j+1)),$j+1,'  BOX BOX     1      ';
			printf WRITING '% .3f  % .3f  % .3f',$X[$j],$Y[$j],$Z[$j];
			print WRITING "\n";
			$j++;
		}
		for($j=0;$j<63;$j++)
		{
			printf WRITING 'CONECT   %2d   %2d',$j+1,$j+2;
			print WRITING "\n";
		}
		close WRITING;
	}	

# And more...
	for ($i=9;$i<12;$i++){
		$j=0;
		foreach (@Y)
		{
			$temp = $Y[$j];
			$Y[$j]= $Z[$j]*(-1)+3;
			$Z[$j]= $temp;
			$j++;
		}
		$aux = $i+2;
                $NewFile2 = "${ExtraDir}/${FileName}.r${aux}.pdb";
		open(WRITING, ">$NewFile2") || die "Could not open file for writing...";
		
		$j=0;
		foreach(@X)
		{
			print WRITING 'ATOM'.' 'x(7-length($j+1)),$j+1,'  BOX BOX     1      ';
			printf WRITING '% .3f  % .3f  % .3f',$X[$j],$Y[$j],$Z[$j];
			print WRITING "\n";
			$j++;
		}
		for($j=0;$j<63;$j++)
		{
			printf WRITING 'CONECT   %2d   %2d',$j+1,$j+2;
			print WRITING "\n";
		}
		close WRITING;
	}
# And more...
	for ($i=12;$i<15;$i++){
		$j=0;
		foreach (@X)
		{
			$temp = $X[$j];
			$X[$j]= $Y[$j]*(-1)+3;
			$Y[$j]= $temp;
			$j++;
		}
		$aux = $i+2;
                $NewFile2 = "${ExtraDir}/${FileName}.r${aux}.pdb";
		open(WRITING, ">$NewFile2") || die "Could not open file for writing...";
		
		$j=0;
		foreach(@X)
		{
			print WRITING 'ATOM'.' 'x(7-length($j+1)),$j+1,'  BOX BOX     1      ';
			printf WRITING '% .3f  % .3f  % .3f',$X[$j],$Y[$j],$Z[$j];
			print WRITING "\n";
			$j++;
		}
		for($j=0;$j<63;$j++)
		{
			printf WRITING 'CONECT   %2d   %2d',$j+1,$j+2;
			print WRITING "\n";
		}		$NewFile2 = $FileName.'.r'.$aux.'.pdb';
		close WRITING;
	}
# And more...
	for ($i=15;$i<17;$i++){
		$j=0;
		foreach (@Y)
		{
			$temp = $Y[$j];
			$Y[$j]= $Z[$j]*(-1)+3;
			$Z[$j]= $temp;
			$j++;
		}
		$aux = $i+2;
                $NewFile2 = "${ExtraDir}/${FileName}.r${aux}.pdb";
		open(WRITING, ">$NewFile2") || die "Could not open file for writing...";
		
		$j=0;
		foreach(@X)
		{
			print WRITING 'ATOM'.' 'x(7-length($j+1)),$j+1,'  BOX BOX     1      ';
			printf WRITING '% .3f  % .3f  % .3f',$X[$j],$Y[$j],$Z[$j];
			print WRITING "\n";
			$j++;
		}
		for($j=0;$j<63;$j++)
		{
			printf WRITING 'CONECT   %2d   %2d',$j+1,$j+2;
			print WRITING "\n";
		}
		close WRITING;
	}
# And more...
	for ($i=17;$i<19;$i++){
		$j=0;
		foreach (@X)
		{
			$temp = $X[$j];
			$X[$j]= $Y[$j]*(-1)+3;
			$Y[$j]= $temp;
			$j++;
		}
		$aux = $i+2;
                $NewFile2 = "${ExtraDir}/${FileName}.r${aux}.pdb";
		open(WRITING, ">$NewFile2") || die "Could not open file for writing...";
		
		$j=0;
		foreach(@X)
		{
			print WRITING 'ATOM'.' 'x(7-length($j+1)),$j+1,'  BOX BOX     1      ';
			printf WRITING '% .3f  % .3f  % .3f',$X[$j],$Y[$j],$Z[$j];
			print WRITING "\n";
			$j++;
		}
		for($j=0;$j<63;$j++)
		{
			printf WRITING 'CONECT   %2d   %2d',$j+1,$j+2;
			print WRITING "\n";
		}
		close WRITING;
	}

# And more...
	for ($i=19;$i<20;$i++){
		$j=0;
		foreach (@Y)
		{
			$temp = $Y[$j];
			$Y[$j]= $Z[$j]*(-1)+3;
			$Z[$j]= $temp;
			$j++;
		}
		$aux = $i+2;
                $NewFile2 = "${ExtraDir}/${FileName}.r${aux}.pdb";
		open(WRITING, ">$NewFile2") || die "Could not open file for writing...";
		
		$j=0;
		foreach(@X)
		{
			print WRITING 'ATOM'.' 'x(7-length($j+1)),$j+1,'  BOX BOX     1      ';
			printf WRITING '% .3f  % .3f  % .3f',$X[$j],$Y[$j],$Z[$j];
			print WRITING "\n";
			$j++;
		}
		for($j=0;$j<63;$j++)
		{
			printf WRITING 'CONECT   %2d   %2d',$j+1,$j+2;
			print WRITING "\n";
		}
		close WRITING;
	}

# And more...
	for ($i=20;$i<21;$i++){
		$j=0;
		foreach (@X)
		{
			$temp = $X[$j];
			$X[$j]= $Y[$j]*(-1)+3;
			$Y[$j]= $temp;
			$j++;
		}
		$aux = $i+2;
                $NewFile2 = "${ExtraDir}/${FileName}.r${aux}.pdb";
		open(WRITING, ">$NewFile2") || die "Could not open file for writing...";
		
		$j=0;
		foreach(@X)
		{
			print WRITING 'ATOM'.' 'x(7-length($j+1)),$j+1,'  BOX BOX     1      ';
			printf WRITING '% .3f  % .3f  % .3f',$X[$j],$Y[$j],$Z[$j];
			print WRITING "\n";
			$j++;
		}
		for($j=0;$j<63;$j++)
		{
			printf WRITING 'CONECT   %2d   %2d',$j+1,$j+2;
			print WRITING "\n";
		}
		close WRITING;
	}

# And more...
	for ($i=21;$i<22;$i++){
		$j=0;
		foreach (@Y)
		{
			$temp = $Y[$j];
			$Y[$j]= $Z[$j]*(-1)+3;
			$Z[$j]= $temp;
			$j++;
		}
		$aux = $i+2;
                $NewFile2 = "${ExtraDir}/${FileName}.r${aux}.pdb";
		open(WRITING, ">$NewFile2") || die "Could not open file for writing...";
		
		$j=0;
		foreach(@X)
		{
			print WRITING 'ATOM'.' 'x(7-length($j+1)),$j+1,'  BOX BOX     1      ';
			printf WRITING '% .3f  % .3f  % .3f',$X[$j],$Y[$j],$Z[$j];
			print WRITING "\n";
			$j++;
		}
		for($j=0;$j<63;$j++)
		{
			printf WRITING 'CONECT   %2d   %2d',$j+1,$j+2;
			print WRITING "\n";
		}
		close WRITING;
	}

# And more...
	for ($i=22;$i<23;$i++){
		$j=0;
		foreach (@X)
		{
			$temp = $X[$j];
			$X[$j]= $Y[$j]*(-1)+3;
			$Y[$j]= $temp;
			$j++;
		}
		$aux = $i+2;
                $NewFile2 = "${ExtraDir}/${FileName}.r${aux}.pdb";
		open(WRITING, ">$NewFile2") || die "Could not open file for writing...";
		
		$j=0;
		foreach(@X)
		{
			print WRITING 'ATOM'.' 'x(7-length($j+1)),$j+1,'  BOX BOX     1      ';
			printf WRITING '% .3f  % .3f  % .3f',$X[$j],$Y[$j],$Z[$j];
			print WRITING "\n";
			$j++;
		}
		for($j=0;$j<63;$j++)
		{
			printf WRITING 'CONECT   %2d   %2d',$j+1,$j+2;
			print WRITING "\n";
		}
		close WRITING;
	}
}




closedir(DIR);
exit;
