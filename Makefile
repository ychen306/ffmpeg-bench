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

h264_dsp.o: h264_dsp.c
	clang -O3 $^ -o $@ -c

bench.o: bench.c
	clang -O3 $^ -o $@ -c


bench: bench.o h264-idct.o lossless_audiodsp.o aacencdsp.o aacpsdsp.o h264_dsp.o
	clang $^ -o $@

########## ASAN build #######

h264-idct.asan.o: h264-idct.c
	clang -O0 $^ -o $@ -c -fsanitize=address -g

lossless_audiodsp.asan.o: lossless_audiodsp.c
	clang -O0 $^ -o $@ -c -fsanitize=address -g

aacencdsp.asan.o: aacencdsp.c
	clang -O0 $^ -o $@ -c -fsanitize=address -g

aacpsdsp.asan.o: aacpsdsp.c
	clang -O0 $^ -o $@ -c -fsanitize=address -g

bench.asan.o: bench.c
	clang -O0 $^ -o $@ -c -fsanitize=address -g

bench.asan: bench.asan.o h264-idct.asan.o lossless_audiodsp.asan.o aacencdsp.asan.o aacpsdsp.asan.o
	clang -fsanitize=address $^ -o $@ -g

clean:
	rm -f *.o bench bench.asan
