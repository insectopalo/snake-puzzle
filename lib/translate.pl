#/usr/bin/perl -w

# Translate each solution in order for it to be in the same euclidean
# space (0<=x<=3;0<=y<=3; 0<=z<=3)

my $Dir = "raw";
my $FileName;
my $x,$y,$z;
my @X,@Y,@Z;


opendir (DIR,$Dir) || die "Cannot open directory";
while ($FileName = readdir(DIR))
{
   if ($FileName !~ /^res/){next;}
   if ($FileName =~ /pdb$/){next;}
   $FileName = "$Dir/$FileName";
   #print $FileName;
   open(FILE,"$FileName") || die "Could not open the given file";
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
   
   my $xmax=0, $ymax=0, $zmax=0, $j=0;
   foreach(@X)
   {
      if ($_ > $xmax)
      {
         $xmax = $_;
      }
   }
   $j=0;
   if ($xmax < 3)
   {
      foreach(@X)
      {
         $X[$j] = $_ + (3-$xmax);
         $j++;
      }
   }
   
   foreach(@Y)
   {
      if ($_ > $ymax)
      {
         $ymax = $_;
      }
   }
   $j=0;
   if ($ymax < 3)
   {
      foreach(@Y)
      {
         $Y[$j] = $_ + (3-$ymax);
         $j++;
      }
   }
   
   foreach(@Z)
   {
      if ($_ > $zmax)
      {
         $zmax = $_;
      }
   }
   $j=0;
   if ($zmax < 3)
   {
      foreach(@Z)
      {
         $Z[$j] = $_ + (3-$zmax);
         $j++;
      }
   }

   # Export translations to new files
   my $NewFile = "${FileName}.pdb";
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
}
closedir(DIR);
exit;
