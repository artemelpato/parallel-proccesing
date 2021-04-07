#!/bin/zsh

initial_seed=$1
n_cores=$2
n_events=$3

final_seed=$((${initial_seed} + ${n_cores} - 1))

for i in $(seq ${initial_seed} ${final_seed}) 
do
	echo "Generating at seed ${i}"
	./main91 ${i} ${n_events} lelel >> log/seed_${i}.log & disown
done	

