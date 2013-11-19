#!/usr/bin/python

import sys, os, re, datetime, optparse

myself=sys.argv[0]
usage="USAGE: %prog [options] filename"
p=optparse.OptionParser(usage=usage)
p.add_option('--file','-f',action='store',type="string",help="filename: name of the file with a list of testfiles to be updated")
p.add_option('--verbose','-v',action='store_true',help="noisy")
options,arguments=p.parse_args()

if len(arguments) > 0:
	filename=arguments.pop(0)
else:
	print usage
	sys.exit(1)

file=open(filename,'r')
lines=file.readlines()
file.close()

discard_trailing_whitespace=re.compile(" +$")
extract_comment=re.compile(".*# +")
discard_comment=re.compile(" *#.*")

listlines=lines
total=0
for line in listlines:
	line=line.strip('\n')
	if len(line) > 0:
		line=discard_trailing_whitespace.sub("",line)
		fullline=line
		comment=extract_comment.sub("",line)
		line=discard_comment.sub("",line)
		testfile=line
		if len(testfile) > 0:
			print testfile
			total+=1
		else:
			print fullline

count=0
for line in lines:
	line=line.strip('\n')
	line=discard_trailing_whitespace.sub("",line)
	comment=extract_comment.sub("",line)
	line=discard_comment.sub("",line)
	testfile=line

	if len(testfile) > 0:
		count+=1
		command="./update_baseline.py "+testfile
		print command
		os.system(command)
		command="./update_baseline.py --pdf "+testfile
		print command
		os.system(command)

sys.exit(0)
