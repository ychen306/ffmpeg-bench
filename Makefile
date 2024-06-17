MAKE := make
.PHONY: all clean

all: bench bench.asan

h264-idct.o: h264-idct.c
	$(CC) -O3 $^ -o $@ -c $(EXTRA_FLAGS)

lossless_audiodsp.o: lossless_audiodsp.c
	$(CC) -O3 $^ -o $@ -c $(EXTRA_FLAGS)

aacencdsp.o: aacencdsp.c
	$(CC) -O3 $^ -o $@ -c $(EXTRA_FLAGS)

aacpsdsp.o: aacpsdsp.c
	$(CC) -O3 $^ -o $@ -c $(EXTRA_FLAGS)

h264_dsp.o: h264_dsp.c
	$(CC) -O3 $^ -o $@ -c $(EXTRA_FLAGS)

bench.o: bench.c
	$(CC) -O3 $^ -o $@ -c $(EXTRA_FLAGS)


bench: bench.o h264-idct.o lossless_audiodsp.o aacencdsp.o aacpsdsp.o h264_dsp.o
	$(CC) $^ -o $@

########## ASAN build #######

h264-idct.asan.o: h264-idct.c
	$(CC) -O0 $^ -o $@ -c -fsanitize=address -g

lossless_audiodsp.asan.o: lossless_audiodsp.c
	$(CC) -O0 $^ -o $@ -c -fsanitize=address -g

aacencdsp.asan.o: aacencdsp.c
	$(CC) -O0 $^ -o $@ -c -fsanitize=address -g

aacpsdsp.asan.o: aacpsdsp.c
	$(CC) -O0 $^ -o $@ -c -fsanitize=address -g

bench.asan.o: bench.c
	$(CC) -O0 $^ -o $@ -c -fsanitize=address -g

bench.asan: bench.asan.o h264-idct.asan.o lossless_audiodsp.asan.o aacencdsp.asan.o aacpsdsp.asan.o
	$(CC) -fsanitize=address $^ -o $@ -g

clean:
	rm -f *.o bench bench.asan
