#!/usr/bin/env python

import os, time

command="dump_baseline_plus.py "

now=time.strftime("%Y-%m-%d-%H:%M:%S", time.localtime())
YESTERDAY = time.strftime("%Y%m%d", time.localtime(time.time() - (24*60*60)))

collate_filename='collate.'+now
collate_sort_filename='collate.sort.'+now

commandfull=command+"baseline.db >"+collate_filename
print commandfull
os.system(commandfull)

commandfull=command+"daily/20070312.db >>"+collate_filename
print commandfull
os.system(commandfull)

commandfull=command+"daily/20070313.db >>"+collate_filename
print commandfull
os.system(commandfull)

commandfull="sort "+collate_filename+" > "+collate_sort_filename
print commandfull
os.system(commandfull)
