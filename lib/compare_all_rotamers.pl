#/usr/bin/perl -w
use Data::Dumper;
use strict;
# This script compares all rotamers to check if there are redundant conformations

my $Dir = "rotamers";
my $FileName;
my @Files;

opendir (DIR,$Dir) || die "Cannot open directory";
while ($FileName = readdir(DIR))
{
   if ($FileName !~ /^res/){next;}
   if ($FileName !~ /pdb$/){next;}
   push(@Files,$FileName);
}
closedir(DIR);

my $Line1;
my $Line2;
my $EqualLines;
my $ModelNumber1;
my $ModelNumber2;

for(my $i=0;$i<$#Files;$i++)
{
   for(my $j=$i+1;$j<=$#Files;$j++)
   {
      $ModelNumber1 = $Files[$i];
      $ModelNumber2 = $Files[$j];
      $ModelNumber1 =~ s/res(\d{4})\.pdb\.r\d{2}\.pdb/$1/;
      $ModelNumber2 =~ s/res(\d{4})\.pdb\.r\d{2}\.pdb/$1/;
      #	print "Model $ModelNumber1 vs $ModelNumber2\n";
      if ($ModelNumber1 == $ModelNumber2){next;}
      #	print "   Different results\n";
      open(FILEONE,"${Dir}/$Files[$i]") || die "Could not open the first file!";
      open(FILETWO,"${Dir}/$Files[$j]") || die "Could not open the second file";
      #	print "Comparing ",$Files[$i]," with ",$Files[$j],"\n";
      
      $EqualLines = 0;
      while ($Line1 = <FILEONE>)
      {
         $Line2 = <FILETWO>;
         chomp $Line1;
         chomp $Line2;
         if ($Line1 =~ /^CONECT/){
            if  ($EqualLines==64){
               #print "EQUAL: $Files[$i] and $Files[$j]\n";
               print "$ModelNumber1 $ModelNumber2\n";
            }
            last;
         }
         if ($Line1 eq $Line2){$EqualLines++;}
      }
      close FILETWO;
      close FILEONE;
   }
}
exit;
