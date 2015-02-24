.PHONY: compile

clean:
	rm -f index generateTest *.txt
compile:
	gcc -O2 index.c -o index

run: compile
	./index indexing_file.txt < queries_file.txt > outputs.txt
	diff answers_file.txt outputs.txt

compileGenTest:
	gcc -O2 generateTest.c -o generateTest
genTest0: compileGenTest
	./generateTest -1 1 4 40
genTest1: compileGenTest
	./generateTest -1 1 1000 134217728
genTest2: compileGenTest
	./generateTest -1 10 20 134217728
genTest3: compileGenTest
	./generateTest -1 1 1 134217728
genTest4: compileGenTest
	./generateTest -1 134217728 134217728 134217728

runCustom: compile
	./index ${file} 3>&1 1>&2 2>&3 | tee report.txt
