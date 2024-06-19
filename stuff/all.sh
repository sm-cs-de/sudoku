#! /bin/bash
rm -f $(\ls *Lösung)
for i in $(\ls test*) 
do
	../Release/Sudoku $i
done

