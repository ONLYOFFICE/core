#!/usr/bin/perl

# print out a list of open customer/P1/regression bugs by engineer 
# if the file "Customer support list.txt" is present the customer number
# is replaced by the customer name
# the format of this file is:
#
# #1 ART Artifex Software Inc.
# #666 NSA National Security Agency
# .
# .
# .


use strict;
use warnings;

use LWP::Simple;
use Date::Calc qw(Delta_Days);

sub daysFromNow($) {
  my $date=shift;
  my $diff=0;

  if ($date =~ m/(\d\d\d\d)-(\d\d)-(\d\d)/) {
    my $year1=$1;
    my $month1=$2;
    my $day1=$3;
    my $year2;
    my $month2;
    my $day2;
    ($day2, $month2, $year2) = (localtime)[3,4,5];
    $year2+=1900;
    $month2+=1;
    $diff=Delta_Days($year1,$month1,$day1,$year2,$month2,$day2);
  }

  return($diff);
}

#read Customer support list.txt
my %customers;
{
  my $filename="Customer support list.txt";
  if (open(F,"<$filename")) {
  while(<F>) {
    chomp;
    if (m/#(\d+) (...) (.+)/) {
      my $number=$1;
      my $id=$2;
      my $name=$3;
      $customers{$number}=$name;
    }
  }
  close(F);
  } else {
    print STDERR "file \"Customer support list.txt\" not found, proceeding without.\n";
  }
  $customers{'P1'}='P1';
  $customers{'Regression'}='Regression';
}


sub printReport($) {
  my $engineer=shift;

  print "\n";
  print "-------------------------------------------------------------------------------------------------------------------\n";
  print "\n";
  print "Engineer: $engineer\n";
  print "\n";
  print "                              Days\n";
  print " Bug     Days   Days   Days   Engr\n";
  print "Number   Open  Assign  Idle   Resp    Customer(s)                      Summary\n";
  print "\n";

my $a;
my @a;
my %bugList;

$a=`lynx -nolist -width 1024 -dump http://bugs.ghostscript.com/buglist.cgi?short_desc_type=allwordssubstr\\&short_desc=\\&long_desc_type=allwordssubstr\\&long_desc=\\&keywords_type=allwords\\&bug_status=UNCONFIRMED\\&bug_status=NEW\\&bug_status=ASSIGNED\\&bug_status=REOPENED\\&bug_severity=blocker\\&bug_severity=critical\\&bug_severity=major\\&bug_severity=normal\\&bug_severity=minor\\&bug_severity=trivial\\&emailassigned_to1=1\\&emailtype1=substring\\&email1=$engineer\\&emailassigned_to2=1\\&bugidtype=include\\&chfieldto=Now\\&field0-0-0=Customer\\&type0-0-0=greaterthan\\&value0-0-0=0`;
@a=split '\n',$a;
foreach (@a) {
  chomp;
  if (m/(\d\d\d\d\d\d) ... P. /) {
    $bugList{$1}="Customer";
  }
}

$a=`lynx -nolist -width 1024 -dump http://bugs.ghostscript.com/buglist.cgi?short_desc_type=allwordssubstr\\&short_desc=\\&long_desc_type=allwordssubstr\\&long_desc=\\&keywords_type=allwords\\&bug_status=CLOSED\\&resolution=LATER\\&resolution=REMIND\\&bug_severity=blocker\\&bug_severity=critical\\&bug_severity=major\\&bug_severity=normal\\&bug_severity=minor\\&bug_severity=trivial\\&emailassigned_to1=1\\&emailtype1=substring\\&email1=$engineer\\&emailassigned_to2=1\\&bugidtype=include\\&chfieldto=Now\\&field0-0-0=Customer\\&type0-0-0=greaterthan\\&value0-0-0=0`;
@a=split '\n',$a;
foreach (@a) {
  chomp;
  if (m/(\d\d\d\d\d\d) ... P. /) {
    $bugList{$1}="Customer";
  }
}

foreach my $i (keys %bugList) {
# print "$i\n";
}
#exit;


$a=`lynx -nolist -width 1024 -dump http://bugs.ghostscript.com/buglist.cgi?short_desc_type=allwordssubstr\\&short_desc=Regression%3A\\&long_desc_type=allwordssubstr\\&long_desc=\\&keywords_type=allwords\\&bug_status=UNCONFIRMED\\&bug_status=NEW\\&bug_status=ASSIGNED\\&bug_status=REOPENED\\&bug_severity=blocker\\&bug_severity=critical\\&bug_severity=major\\&bug_severity=normal\\&bug_severity=minor\\&bug_severity=trivial\\&emailassigned_to1=1\\&emailtype1=substring\\&email1=$engineer\\&emailassigned_to2=1\\&bugidtype=include\\&chfieldto=Now`;
@a=split '\n',$a;
foreach (@a) {
  chomp;
  if (m/(\d\d\d\d\d\d) ... P. /) {
    $bugList{$1}="Regression" if (!exists $bugList{$1});
  }
}

$a=`lynx -nolist -width 1024 -dump http://bugs.ghostscript.com/buglist.cgi?short_desc_type=allwordssubstr\\&short_desc=Regression%3A\\&long_desc_type=allwordssubstr\\&long_desc=\\&keywords_type=allwords\\&bug_status=CLOSED\\&resolution=LATER\\&resolution=REMIND\\&bug_severity=blocker\\&bug_severity=critical\\&bug_severity=major\\&bug_severity=normal\\&bug_severity=minor\\&bug_severity=trivial\\&emailassigned_to1=1\\&emailtype1=substring\\&email1=$engineer\\&emailassigned_to2=1\\&bugidtype=include\\&chfieldto=Now`;
@a=split '\n',$a;
foreach (@a) {
  chomp;
  if (m/(\d\d\d\d\d\d) ... P. /) {
    $bugList{$1}="Regression" if (!exists $bugList{$1});
  }
}


$a=`lynx -nolist -width 1024 -dump http://bugs.ghostscript.com/buglist.cgi?short_desc_type=allwordssubstr\\&short_desc=\\&long_desc_type=allwordssubstr\\&long_desc=\\&keywords_type=allwords\\&bug_status=UNCONFIRMED\\&bug_status=NEW\\&bug_status=ASSIGNED\\&bug_status=REOPENED\\&bug_severity=blocker\\&bug_severity=critical\\&bug_severity=major\\&bug_severity=normal\\&bug_severity=minor\\&bug_severity=trivial\\&emailassigned_to1=1\\&emailtype1=substring\\&email1=$engineer\\&emailassigned_to2=1\\&bugidtype=include\\&chfieldto=Now\\&priority=P1`;
@a=split '\n',$a;
foreach (@a) {
  chomp;
  if (m/(\d\d\d\d\d\d) ... P. /) {
    $bugList{$1}="P1" if (!exists $bugList{$1});
  }
}

$a=`lynx -nolist -width 1024 -dump http://bugs.ghostscript.com/buglist.cgi?short_desc_type=allwordssubstr\\&short_desc=\\&long_desc_type=allwordssubstr\\&long_desc=\\&keywords_type=allwords\\&bug_status=CLOSED\\&resolution=LATER\\&resolution=REMIND\\&bug_severity=blocker\\&bug_severity=critical\\&bug_severity=major\\&bug_severity=normal\\&bug_severity=minor\\&bug_severity=trivial\\&emailassigned_to1=1\\&emailtype1=substring\\&email1=$engineer\\&emailassigned_to2=1\\&bugidtype=include\\&chfieldto=Now\\&priority=P1`;
@a=split '\n',$a;
foreach (@a) {
  chomp;
  if (m/(\d\d\d\d\d\d) ... P. /) {
    $bugList{$1}="P1" if (!exists $bugList{$1});
  }
}



foreach (sort keys %bugList) {
    my $bugNumber=$_;
    my $opened="0000-00-00 00:00";
    my $modified="0000-00-00 00:00";
    my $dateAssigned="0000-00-00 00:00";
    my $customerNumber=0;
    my $entered="unknown1";
    my $assigned="unknown2";
    my $comment="unknown3";
    my $commentedUpon="never";

    my $b=`lynx -nolist -width 1025 -dump http://bugs.ghostscript.com/show_bug.cgi?id=$bugNumber`;

    my $description="";
    $description=$1 if ($b =~ m/Bug \d\d\d\d\d.\s+?(\S.+?)\n/);
    my @b=split '\n',$b;
    foreach (@b) {
      chomp;
      if (m/Opened: (\d\d\d\d-\d\d-\d\d \d\d:\d\d)/) {
        $opened=$1;
        $commentedUpon=$opened if ($entered eq $assigned);
      }
      $modified=$1 if (m/Additional Comment .+ (\d\d\d\d-\d\d-\d\d \d\d:\d\d)/);
      $entered=$1 if (m/Reporter: \S+ \((.+)\)$/);
      $assigned=$1 if (m/Assigned To: \S+ \((.+)\)$/);
      $comment=$1 if (m/Additional Comment #\d+ From (.+?) 20/);
      $commentedUpon=$modified if ($comment eq $assigned);
    }


    if ($bugList{$bugNumber} eq "Customer") {
      my $url="http://bugs.ghostscript.com/show_bug.cgi?id=$bugNumber";
      $b=get $url;
  
      $customerNumber=$1 if ($b =~ m/"Customer"\s.+?value="(.+?)"/s);
#print "$bugNumber $customerNumber\n";
    } else {
      $customerNumber=$bugList{$bugNumber};
    }

    $b=`lynx -nolist -width 1025 -dump http://bugs.ghostscript.com/show_activity.cgi?id=$bugNumber`;
    $dateAssigned=$1 if ($b =~ m/.*(\d\d\d\d-\d\d-\d\d \d\d:\d\d):\d\d AssignedTo/s);

#   print "$opened\t$modified\t$dateAssigned\t$customerNumber\n";
    $dateAssigned=$opened if ($dateAssigned eq "0000-00-00 00:00");  # bugs that are automatically assigned when they are entered don't show AssignedTo on the activity page
    $modified    =$opened if ($modified     eq "0000-00-00 00:00");


#   print "$opened\t$modified\t$dateAssigned\t$customerNumber\n";
    $customerNumber =~ s/,/ /g;
    $customerNumber =~ s/  / /g;
    my $customerName=$customerNumber;
    my @customers=split ' ',$customerNumber;
    foreach my $customerCount (0..(scalar @customers)-1) {
      $customerNumber=$customers[$customerCount];
      $customerName=$customerNumber;
      $customerName=$customers{$customerNumber} if (exists($customers{$customerNumber}));
      $customerName=substr $customerName,0,30;
      my $responseNeeded='---';
      $responseNeeded=daysFromNow($commentedUpon) if ($commentedUpon ne "never");
      if ($customerCount==0) {
        printf "%d  %5d  %5d  %5d    %3s    %-30s   %s\n",$bugNumber,daysFromNow($opened),daysFromNow($dateAssigned),daysFromNow($modified),$responseNeeded,$customerName,$description;
      } else {
        printf "                                      %-30s\n",$customerName;
      }
    }

}
  print "\n";
}


  printReport("Alex");
  printReport("Henry");
  printReport("Ken");
  printReport("Leonardo");
  printReport("Marcos");
  printReport("Michael");
  printReport("Ralph");
  printReport("Ray");
  printReport("Tor");
  printReport("support");





