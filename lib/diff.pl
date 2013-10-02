#/usr/bin/perl -w

# This script compares matrixes of contacts to check how similar the structures
# are. It compares all pairs from 1 to 8

my $Dir = "contacts";

for (my $i=1;$i<=8;$i++){
   for (my $j=1;$j<8;$j++){
      my $tot=0;
      my $diff=0;
      my $file1 = sprintf("${Dir}/res000%i.matrix",$i);
      my $file2 = sprintf("${Dir}/res000%i.matrix",$j);
      # printf "%i vs %i ",$i,$j;
      # print "$file1 $file2\n";
      open (FILE1,$file1) || die "Could not open $file1";
      open (FILE2,$file2) || die "Could not open $file2";
      while (my $line1 = <FILE1>){
         my $line2 = <FILE2>;
         chomp $line1;
         chomp $line2;
         for (0..length($line1)){
           if (substr($line1, $_, 1) eq 1){
              $tot++;
              if (substr($line2, $_, 1) eq 0){
                 $diff++;
              }
           }
         }
      }
      close FILE1 || die "Could not close $file1";
      close FILE2 || die "Could not close $file2";
      #printf ("%i %i %.3f\n",$tot-$diff, $tot, 1-$diff/$tot);
      printf ("%i ",($tot-$diff-64)/2);
   }
   print "\n";
}


exit;
