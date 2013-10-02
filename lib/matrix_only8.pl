#/usr/bin/perl -w

# This script creates de contact matrix given pdb box file
use Data::Dumper;

my $FileName = $ARGV[0];
my $MatrixFile = $ARGV[1];
my $x,$y,$z;
my @X,@Y,@Z;
my @matrix = ();

open(FILE,"$FileName") || die "Could not open $FileName";
@X = ();
@Y = ();
@Z = ();

while (my $Line = <FILE>)
{
   if ($Line !~ /^ATOM/){next;}
   chomp $Line;
   $Line =~ /(\d\.\d{3})   (\d\.\d{3})   (\d\.\d{3})$/;
   push(@X,$1);
   push(@Y,$2);
   push(@Z,$3);
}
close(FILE);

open(FILE2,">$MatrixFile") || die "Could not open the given file";
for(my $i=0;$i<=$#X;$i++)
{
   for (my $j=0;$j<=$#X;$j++)
   {
      #printf ("%.2f\n",sqrt(($X[$i]-$X[$j])*($X[$i]-$X[$j])+($Y[$i]-$Y[$j])*($Y[$i]-$Y[$j])+($Z[$i]-$Z[$j])*($Z[$i]-$Z[$j])));
      if (sqrt(($X[$i]-$X[$j])*($X[$i]-$X[$j])+($Y[$i]-$Y[$j])*($Y[$i]-$Y[$j])+($Z[$i]-$Z[$j])*($Z[$i]-$Z[$j])) <= 1){
         print FILE2 "1";
      }else{
         print FILE2 "0";
      }
   }
   print FILE2 "\n";
   #print "\n";
}
close(FILE2);
exit;
