#!/bin/sh


old_release=$1
new_release=$2

sh_files=`find . -name "*.sh" -print`
am_files=`find . -name "*.am" -print`
in_files=`find . -name "*.in" -print`
html_files=`find . -name "*.html" -print`
sgml_files=`find . -name "*.sgml" -print`
cvsignore_files=`find . -name ".cvsignore" -print`

files="$sh_files $am_files $in_files $html_files $sgml_files $cvsignore_files"
for i in  $files; do
    echo Processing $i ...
    sed "s/$old_release/$new_release/g" $i > $i.tmp
    if [ $? != 0 ]; then
	echo "Failed to process file $i"
	exit 1
    fi
    
    mode=`stat -c "%a" $i`
    chmod $mode $i.tmp
    if [ $? != 0 ]; then
	echo "Failed to retore permissions for $i"
	exit 1
    fi
done

for i in  $files; do
    echo Moving $i ...
    mv $i.tmp $i
done
