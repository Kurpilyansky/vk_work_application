.PHONY: compile

compile:
	gcc -O2 index.c -o index
run: compile
	./index ${file}
genTest:
	gcc -O2 generateTest.c -o generateTest
	./generateTest -1 1 1000 128000000
