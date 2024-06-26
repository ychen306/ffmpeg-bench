MAKE := make
.PHONY: all clean test run-tests

CC := clang
all: bench bench.asan bench.novec
test: all run-tests

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

h263dsp.o: h263dsp.c
	$(CC) -O3 $^ -o $@ -c $(EXTRA_FLAGS)

bench.o: bench.c
	$(CC) -O3 $^ -o $@ -c $(EXTRA_FLAGS)


bench: bench.o h264-idct.o lossless_audiodsp.o aacencdsp.o aacpsdsp.o h264_dsp.o h263dsp.o
	$(CC) $^ -o $@

########## ASAN build #######

h264-idct.asan.o: h264-idct.c
	cc -O0 $^ -o $@ -c -fsanitize=address -g

lossless_audiodsp.asan.o: lossless_audiodsp.c
	cc -O0 $^ -o $@ -c -fsanitize=address -g

aacencdsp.asan.o: aacencdsp.c
	cc -O0 $^ -o $@ -c -fsanitize=address -g

aacpsdsp.asan.o: aacpsdsp.c
	cc -O0 $^ -o $@ -c -fsanitize=address -g

h264_dsp.asan.o: h264_dsp.c
	cc -O0 $^ -o $@ -c -fsanitize=address -g

h263dsp.asan.o: h263dsp.c
	cc -O0 $^ -o $@ -c -fsanitize=address -g

bench.asan.o: bench.c
	cc -O0 $^ -o $@ -c -fsanitize=address -g

bench.asan: bench.asan.o h264-idct.asan.o lossless_audiodsp.asan.o aacencdsp.asan.o aacpsdsp.asan.o h263dsp.asan.o h264_dsp.asan.o
	cc -fsanitize=address $^ -o $@ -g

###### Non-Vectorized Build ######

h264-idct.novec.o: h264-idct.c
	$(CC) -O3 $^ -o $@ -c -fno-vectorize -fno-slp-vectorize

lossless_audiodsp.novec.o: lossless_audiodsp.c
	$(CC) -O3 $^ -o $@ -c -fno-vectorize -fno-slp-vectorize

aacencdsp.novec.o: aacencdsp.c
	$(CC) -O3 $^ -o $@ -c -fno-vectorize -fno-slp-vectorize

aacpsdsp.novec.o: aacpsdsp.c
	$(CC) -O3 $^ -o $@ -c -fno-vectorize -fno-slp-vectorize

h264_dsp.novec.o: h264_dsp.c
	$(CC) -O3 $^ -o $@ -c -fno-vectorize -fno-slp-vectorize

h263dsp.novec.o: h263dsp.c
	$(CC) -O3 $^ -o $@ -c -fno-vectorize -fno-slp-vectorize

bench.novec.o: bench.c
	$(CC) -O3 $^ -o $@ -c -fno-vectorize -fno-slp-vectorize


bench.novec: bench.novec.o h264-idct.novec.o lossless_audiodsp.novec.o aacencdsp.novec.o aacpsdsp.novec.o h264_dsp.novec.o h263dsp.novec.o
	$(CC) $^ -o $@

run-tests:
	python3 run-tests.py
clean:
	rm -f *.o
