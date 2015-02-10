#! /usr/local/bin/perl -w
use strict;
use warnings;

# get the names of all the file at the current directory
my $dir="klee-last";
my $filename="my_output";
my $all_file_name=`ls $dir/test*ktest`;

## get the name for each file
my @file_name_array=split /\s+/,$all_file_name;

# input the output file name
if(-e "$dir/$filename"){
 unlink("$dir/$filename");
}

# create output file
my $num=1;
foreach my $file_name (@file_name_array) {
`echo "*****test case $num *****" >>$dir/my_output`;
        `ktest-tool --write-ints $file_name>>$dir/my_output`;
$num=$num+1;
}
# output the file
#`cat "$dir/my_output"`;
open(FH,"<$dir/my_output");
my $out;
while($out=<FH>){
print $out;
}