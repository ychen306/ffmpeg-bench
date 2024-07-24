MAKE := make
.PHONY: all clean benchmark run-tests cpp_build acc-tests

CC := clang-18
all: bench bench.novec bench.asan test cpp_build test.asan
all-tests: all run-tests
SANITIZER := address

h264-idct.o: h264-idct.c
	$(CC) -O3 $^ -o $@ -c $(EXTRA_FLAGS) -march=native

lossless_audiodsp.o: lossless_audiodsp.c
	$(CC) -O3 $^ -o $@ -c $(EXTRA_FLAGS) -march=native

aacencdsp.o: aacencdsp.c
	$(CC) -O3 $^ -o $@ -c $(EXTRA_FLAGS) -march=native

aacpsdsp.o: aacpsdsp.c
	$(CC) -O3 $^ -o $@ -c $(EXTRA_FLAGS) -march=native

h264_dsp.o: h264_dsp.c
	$(CC) -O3 $^ -o $@ -c $(EXTRA_FLAGS) -march=native

h263dsp.o: h263dsp.c
	$(CC) -O3 $^ -o $@ -c $(EXTRA_FLAGS) -march=native

cavsdsp.o: cavsdsp.c
	$(CC) -O3 $^ -o $@ -c $(EXTRA_FLAGS) -march=native

bench.o: bench.c
	$(CC) -O3 $^ -o $@ -c $(EXTRA_FLAGS) -march=native

test.o: test.c
	$(CC) -O3 $^ -o $@ -c $(EXTRA_FLAGS) -march=native

bench: bench.o h264-idct.o lossless_audiodsp.o aacencdsp.o aacpsdsp.o \
	h264_dsp.o h263dsp.o cavsdsp.o
	$(CC) $^ -o $@ $(EXTRA_FLAGS)

test: test.o h264-idct.o lossless_audiodsp.o aacencdsp.o aacpsdsp.o \
	h264_dsp.o h263dsp.o cavsdsp.o
	$(CC) $^ -o $@ $(EXTRA_FLAGS)

########## ASAN build #######

h264-idct.asan.o: h264-idct.c
	$(CC) -O0 $^ -o $@ -c -fsanitize=$(SANITIZER) -g

lossless_audiodsp.asan.o: lossless_audiodsp.c
	$(CC) -O0 $^ -o $@ -c -fsanitize=$(SANITIZER) -g

aacencdsp.asan.o: aacencdsp.c
	$(CC) -O0 $^ -o $@ -c -fsanitize=$(SANITIZER) -g

aacpsdsp.asan.o: aacpsdsp.c
	$(CC) -O0 $^ -o $@ -c -fsanitize=$(SANITIZER) -g

h264_dsp.asan.o: h264_dsp.c
	$(CC) -O0 $^ -o $@ -c -fsanitize=$(SANITIZER) -g

h263dsp.asan.o: h263dsp.c
	$(CC) -O0 $^ -o $@ -c -fsanitize=$(SANITIZER) -g

cavsdsp.asan.o: cavsdsp.c
	$(CC) -O0 $^ -o $@ -c -fsanitize=$(SANITIZER) -g

bench.asan.o: bench.c
	$(CC) -O0 $^ -o $@ -c -fsanitize=$(SANITIZER) -g

test.asan.o: test.c
	$(CC) -O0 $^ -o $@ -c -fsanitize=$(SANITIZER) -g

test.asan: test.asan.o h264-idct.asan.o lossless_audiodsp.asan.o aacencdsp.asan.o aacpsdsp.asan.o \
				h263dsp.asan.o h264_dsp.asan.o cavsdsp.asan.o
	$(CC) -fsanitize=$(SANITIZER) $^ -o $@ -g

bench.asan: bench.asan.o h264-idct.asan.o lossless_audiodsp.asan.o aacencdsp.asan.o aacpsdsp.asan.o \
				h263dsp.asan.o h264_dsp.asan.o cavsdsp.asan.o
	$(CC) -fsanitize=$(SANITIZER) $^ -o $@ -g

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

cavsdsp.novec.o: cavsdsp.c
	$(CC) -O3 $^ -o $@ -c -fno-vectorize -fno-slp-vectorize

bench.novec.o: bench.c
	$(CC) -O3 $^ -o $@ -c -fno-vectorize -fno-slp-vectorize


bench.novec: bench.novec.o h264-idct.novec.o lossless_audiodsp.novec.o aacencdsp.novec.o \
				aacpsdsp.novec.o h264_dsp.novec.o h263dsp.novec.o cavsdsp.novec.o
	$(CC) $^ -o $@

cpp_build:
	make -C cpp-build

save_results:
	make save_results -C cpp-build

cpp_clean:
	make clean -C cpp-build

diff:
	make diff -C cpp-build

run-tests:
	python3 run-tests.py
clean:
	rm -f *.o
