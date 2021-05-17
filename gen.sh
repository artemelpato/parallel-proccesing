#!/bin/zsh

n_events=$1
initial_seed=$2
n_cores=$3

final_seed=$((${initial_seed} + ${n_cores} - 1))

for i in $(seq ${initial_seed} ${final_seed}) 
do
	echo "Generating at seed ${i}"
	./parallel_pp ${n_events} ${i}  parallel_pp >> log/seed_${i}.log & 
done	

wait 

echo "DONE"

