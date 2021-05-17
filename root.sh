#!/bin/zsh

name=parallel_pp

cd ${1}

for i in ${name}*.root
do 
	cd ..
	echo "Proccesing "${i}""
	root "root_analyze.C(\"${1}/${i}\", \"${2}/${i}\")" & 
	cd ${2}
done

wait 

hadd ${name}_final_histo.root ${name}*.root

cd ..

root "draw_histo.C(\"${2}/${name}_final_histo.root\")"

echo "DONE"
