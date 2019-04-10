headers = src/btree.h src/compress.h src/DLL.h src/frequency.h src/huffman.h src/minheap.h src/tokenize.h src/util.h

sources = src/btree.c src/compress.c src/DLL.c src/frequency.c src/huffman.c src/minheap.c src/tokenize.c src/util.c

all: $(headers) $(sources) filecompressor.c
	gcc filecompressor.c $(sources) -o fileCompressor

Asst2.tgz: src filecompressor.c testplan.txt readme.pdf
	mkdir Asst2
	mkdir Asst2/src
	cp -a src Asst2
	cp Makefile filecompressor.c testplan.txt readme.pdf Asst2
	tar cfz Asst2.tgz Asst2
	rm -r Asst2

clean:
	rm fileCompressor

minheap.tests: $(headers) $(sources) tests/minheaptests.c
	gcc -g $(sources) tests/minheaptests.c -o tests/minheap.tests
	./tests/minheap.tests

frequency.tests: $(headers) $(sources) tests/frequencytests.c
	gcc -g $(sources) tests/frequencytests.c -o tests/frequency.tests
	./tests/frequency.tests

huffman.tests: $(headers) $(sources) tests/huffmantests.c
	gcc -g $(sources) tests/huffmantests.c -o tests/huffman.tests
	./tests/huffman.tests

compress.tests: $(headers) $(sources) tests/compresstests.c
	gcc -g $(sources) tests/compresstests.c -o tests/compress.tests
	./tests/compress.tests

cleantests:
	rm -f tests/*.tests
	rm -f HuffmanCodebook
	rm -f tests/text/*.hcz
	rm -f tests/hcz/*.txt

