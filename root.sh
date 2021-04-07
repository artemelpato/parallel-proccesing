#!/bin/zsh

for i in $@
do 
	echo "Proccesing "${i}""
	{root <<- EOF
	char* argv = "$i"
	.x root_read.C(argv)
	.q
	EOF
	} >> log/root.log & 
done
