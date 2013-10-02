#!/bin/bash
rm -rf contacts
mkdir contacts
rm -rf rotamers
mkdir rotamers
rm -rf clean
mkdir clean

perl lib/translate.pl
for file in `ls -1 raw | grep -v pdb`
do
  rm raw/${file}
done

perl lib/matrix.pl

perl lib/rotations.pl 
perl lib/compare_all_rotamers.pl > output.temp
sort output.temp | uniq > output
rm -f output.temp


