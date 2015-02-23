.PHONY: compile

compile:
	gcc -O2 index.c -o index
run: compile
	./index ${file} 3>&1 1>&2 2>&3 | tee report.txt
genTest1:
	gcc -O2 generateTest.c -o generateTest
	./generateTest -1 1 1000 134217728
genTest2:
	gcc -O2 generateTest.c -o generateTest
	./generateTest -1 1 20 134217728
genTest3:
	gcc -O2 generateTest.c -o generateTest
	./generateTest -1 134217728 134217728 134217728
