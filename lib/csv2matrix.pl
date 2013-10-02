#/usr/bin/perl -w

# This script creates a matrix format file from the 
# csv file with the coordinates

if ( scalar(@ARGV) != 2 ) {
    print "USAGE: program input.csv ouput.matrix\n";
    exit;
}

my $csvFile = $ARGV[0];
my $bpseqFile = $ARGV[1];

my $x,$y,$z;
my @matrix = ();

open(FILE,"$csvFile") || die "Could not open the given file";

my @X = ();
my @Y = ();
my @Z = ();

my $tot = 0;
while (my $Line = <FILE>)
{
	chomp $Line;
	if ($Line =~ /^([^,]+),([^,]+),([^,]+)/){
	    push(@X,$1);
	    push(@Y,$2);
	    push(@Z,$3);
        $tot++;
    }
}
close(FILE);

print("Number of residues parsed: $tot\n");

open(FILE,">$bpseqFile") || die "Could not open the given file";
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
				print FILE " 1";
			}
			else {print FILE " 0";}
		}
		elsif ($X[$i] == $X[$j] && $Z[$i] == $Z[$j])
		{
			if ($Y[$i] == $Y[$j]+1 || $Y[$i] == $Y[$j]-1)
			{
				print FILE " 1";
			}
			else {print FILE " 0";}
		}
		elsif ($Y[$i] == $Y[$j] && $Z[$i] == $Z[$j])
		{
			if ($X[$i] == $X[$j]+1 || $X[$i] == $X[$j]-1)
			{
				print FILE " 1";
			}
			else {print FILE " 0";}
		}
		else 
		{
			print FILE " 0";
		}
	}
	print FILE "\n";
}
close(FILE);
exit;
