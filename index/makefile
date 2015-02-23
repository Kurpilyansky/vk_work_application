.PHONY: compile

compile:
	gcc -O2 index.c -o index
run: compile
	./index ${file} 3>&1 1>&2 2>&3 | tee report.txt
runTest:
	make genTest$(num)
	./index indexing_file.txt < queries_file.txt > outputs.txt
	diff answers_file.txt outputs.txt
compileGenTest:
	gcc -O2 generateTest.c -o generateTest
genTest0: compileGenTest
	./generateTest 42 1 3 20
genTest1: compileGenTest
	./generateTest 42 1 1000 134217728
genTest2: compileGenTest
	./generateTest 42 10 20 134217728
genTest3: compileGenTest
	./generateTest 42 134217728 134217728 134217728
