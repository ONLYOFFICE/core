#!/usr/bin/perl

my $file;
while ($file = shift @ARGV) {
    # print "Processing file $file...\n";
    open(IN, "$file") || die "Unable to open file $file";
    $state = "";
    $line=0;
    while(<IN>) {
	$line++;
	chomp;
	
	if($state eq "") {
	    if(/xmlSecError\((.*)/) {
		$state = "$file,$line," . $1;
	    }
	} else {
	    if(/(.*);/) {
		$_ = $state . $1;
		$state = "";

		while(/\t/) {
		    s/\t//;
		}
		while(/\, /) {
		    s/\, /\,/;
		}
		while(/\,/) {
		    s/\,/\;/;
		}		
		print "$_\n";
	    } else {
		$state = $state . $_;
	    }
	}
    }
    close IN;
}
