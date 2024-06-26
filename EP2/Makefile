CFLAGS = -Wall -std=c99
TEST_1 = ./tests/entrada_1.txt ./tests/saida_1.txt
TEST_2 = ./tests/entrada_2.txt ./tests/saida_2.txt
TEST_3 = ./tests/entrada_3.txt ./tests/saida_3.txt
TEST_4 = ./tests/entrada_4.txt ./tests/saida_4.txt

clean:
	rm -f *.o *.exe ep2

compile: ep2.c
	gcc ${CFLAGS} ep2.c -o ep2.exe

t1: ep2
	./ep2.exe ${TEST_1} ./tests/saida.txt

t2: ep2
	./ep2.exe ${TEST_2} ./tests/saida.txt

t3: ep2
	./ep2.exe ${TEST_3} ./tests/saida.txt

t4: ep2
	./ep2.exe ${TEST_4} ./tests/saida.txt

ep2.o: ep2.c
	gcc $(CFLAGS) -c  ep2.c 
