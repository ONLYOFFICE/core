#!/bin/sh 

if [ "z$TMPFOLDER" = "z" ] ; then
    TMPFOLDER=/tmp
fi

logfiles='$TMPFOLDER/test*.log'

echo "-------------------- MEMORY USAGE --------------------"
grep 'in use' $logfiles | \
    sed 's/==.*==//' | \
    sort -u

echo "-------------------- ERRORS --------------------"
grep 'ERROR SUMMARY' $logfiles | \
    sed 's/==.*==//' | \
    sed 's/(suppressed: .*//' | \
    sort -u

 