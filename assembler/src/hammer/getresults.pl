#!/usr/bin/perl -w
use strict;
use warnings;
use Cwd;

if ($#ARGV < 3 || $#ARGV > 4) {
	die "\nUsage: getresults.pl results_folder output_folder start_range end_range [nproc]";
}

my $input_folder = $ARGV[0];
my $output_folder = $ARGV[1];
my $start = $ARGV[2];
my $end = $ARGV[3];
my $nproc = 1; if ($#ARGV > 3) { $nproc = $ARGV[4]; }

my $count_command = "/home/snikolenko/algorithmic-biology/assembler/build/release/hammer/quake_enhanced/count/count 64";
my $biggenome = "/scratchfast2/snikolenko/hammer/biggenome.kmers";
my $cleanup_command = "rm *.part";

my $dir = getcwd;
print $dir . "\n";

sub my_exec {
	my $cmd = shift;
	print $cmd . "\n";
	system $cmd;
}

for (my $i = $start; $i <= $end; $i+=$nproc) {
	for (my $j=0; $j < $nproc; ++$j) {
		last unless ($i + $j <= $end);
		#unless (fork) {
			my $num = sprintf "%02d", $i+$j;
			my_exec("mkdir $dir/tmpdir$num");
			chdir "$dir/tmpdir$num";
			my $command = "$count_command $input_folder/$num.kmers.solid $input_folder/$num.countkmers.solid 2 500";
			my_exec($command); my_exec($cleanup_command);
			$command = "$count_command $input_folder/$num.kmers.bad $input_folder/$num.countkmers.bad 2 500";
			my_exec($command); my_exec($cleanup_command);
			$command = "$count_command $input_folder/$num.reads.all.corrected $input_folder/$num.countkmers.reads 2 500";
			my_exec($command); my_exec($cleanup_command);
			chdir "$dir";
			my_exec("rm -rf tmpdir$num");
			my_exec("/home/snikolenko/algorithmic-biology/assembler/build/release/hammer/quake_enhanced/test_correction_quality/test_correction_quality $biggenome $input_folder/$num.countkmers.solid $input_folder/$num.countkmers.bad > $output_folder/$num.results.solidkmers");
			my_exec("/home/snikolenko/algorithmic-biology/assembler/build/release/hammer/quake_enhanced/test_correction_quality/test_correction_quality $biggenome $input_folder/$num.countkmers.reads $input_folder/$num.countkmers.bad > $output_folder/$num.results.newreads");

		#	die "All done with $num.";
		#}
	}
	#wait();
}
