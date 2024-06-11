.PHONY: all clean

all: bench bench.asan

h264-idct.o: h264-idct.c
	$(CC) -O3 $^ -o $@ -c

lossless_audiodsp.o: lossless_audiodsp.c
	$(CC) -O3 $^ -o $@ -c

aacencdsp.o: aacencdsp.c
	$(CC) -O3 $^ -o $@ -c

bench.o: bench.c
	$(CC) -O3 $^ -o $@ -c


bench: bench.o h264-idct.o lossless_audiodsp.o aacencdsp.o
	$(CC) $^ -o $@

########## ASAN build #######

h264-idct.asan.o: h264-idct.c
	$(CC) -Og $^ -o $@ -c -fsanitize=address

lossless_audiodsp.asan.o: lossless_audiodsp.c
	$(CC) -Og $^ -o $@ -c -fsanitize=address

aacencdsp.asan.o: aacencdsp.c
	$(CC) -Og $^ -o $@ -c -fsanitize=address

bench.asan.o: bench.c
	$(CC) -Og $^ -o $@ -c -fsanitize=address

bench.asan: bench.asan.o h264-idct.asan.o lossless_audiodsp.asan.o aacencdsp.asan.o
	$(CC) -fsanitize=address $^ -o $@

clean:
	rm -f *.o bench bench.asan
