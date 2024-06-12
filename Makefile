MAKE := make
.PHONY: all clean

all: bench bench.asan

h264-idct.o: h264-idct.c
	clang -O3 $^ -o $@ -c

lossless_audiodsp.o: lossless_audiodsp.c
	clang -O3 $^ -o $@ -c

aacencdsp.o: aacencdsp.c
	clang -O3 $^ -o $@ -c

aacpsdsp.o: aacpsdsp.c
	clang -O3 $^ -o $@ -c

bench.o: bench.c
	clang -O3 $^ -o $@ -c


bench: bench.o h264-idct.o lossless_audiodsp.o aacencdsp.o aacpsdsp.o
	clang $^ -o $@

########## ASAN build #######

h264-idct.asan.o: h264-idct.c
	clang -Og $^ -o $@ -c -fsanitize=address

lossless_audiodsp.asan.o: lossless_audiodsp.c
	clang -Og $^ -o $@ -c -fsanitize=address

aacencdsp.asan.o: aacencdsp.c
	clang -Og $^ -o $@ -c -fsanitize=address

aacpsdsp.asan.o: aacpsdsp.c
	clang -Og $^ -o $@ -c -fsanitize=address

bench.asan.o: bench.c
	clang -Og $^ -o $@ -c -fsanitize=address

bench.asan: bench.asan.o h264-idct.asan.o lossless_audiodsp.asan.o aacencdsp.asan.o aacpsdsp.asan.o
	clang -fsanitize=address $^ -o $@

clean:
	rm -f *.o bench bench.asan
