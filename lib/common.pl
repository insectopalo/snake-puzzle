#/usr/bin/perl -w

# This script creates de contact matrix given pdb box file
use Data::Dumper;
use strict;

my $Dir = "raw";

my @matrix = ();
for (my $i=1;$i<=8;$i++){
   for (my $j=1;$j<8;$j++){
      my $done = 0;
      my $file1 = sprintf("${Dir}/res000%i.pdb",$i);
      my $file2 = sprintf("${Dir}/res000%i.pdb",$j);
      open (FILE1,$file1) || die "Could not open $file1";
      open (FILE2,$file2) || die "Could not open $file2";

      while (my $Line1 = <FILE1>){
         my $Line2 = <FILE2>;
         if ($Line1 !~ /^ATOM/){next;}
         chomp $Line1;
         chomp $Line2;
         $Line1 =~ /(\d\.\d{3})   (\d\.\d{3})   (\d\.\d{3})$/;
         my ($x,$y,$z) = ($1,$2,$3);
         $Line2 =~ /(\d\.\d{3})   (\d\.\d{3})   (\d\.\d{3})$/;
         #print "$x $y $z $1 $2 $3\n";
         if ($x == $1 && $y == $2 && $z == $3){
            $done++;
         }else{
            last;
         }
      }
      print "$done ";
      close(FILE2);
      close(FILE1);
   }
   print "\n";
}

exit;
