MAKE := make
.PHONY: all clean save_results diff

CC := clang++
all: bench test

h264-idct.o: h264-idct.cpp
	$(CC) -O3 $^ -o $@ -c $(EXTRA_FLAGS) -march=native

lossless_audiodsp.o: lossless_audiodsp.cpp
	$(CC) -O3 $^ -o $@ -c $(EXTRA_FLAGS) -march=native

aacencdsp.o: aacencdsp.cpp
	$(CC) -O3 $^ -o $@ -c $(EXTRA_FLAGS) -march=native

aacpsdsp.o: aacpsdsp.cpp
	$(CC) -O3 $^ -o $@ -c $(EXTRA_FLAGS) -march=native

h264_dsp.o: h264_dsp.cpp
	$(CC) -O3 $^ -o $@ -c $(EXTRA_FLAGS) -march=native

h263dsp.o: h263dsp.cpp
	$(CC) -O3 $^ -o $@ -c $(EXTRA_FLAGS) -march=native

cavsdsp.o: cavsdsp.cpp
	$(CC) -O3 $^ -o $@ -c $(EXTRA_FLAGS) -march=native

bench.o: bench.cpp
	$(CC) -O3 $^ -o $@ -c $(EXTRA_FLAGS) -march=native

test.o: test.cpp
	$(CC) -O3 $^ -o $@ -c $(EXTRA_FLAGS) -march=native

bench: bench.o h264-idct.o lossless_audiodsp.o aacencdsp.o aacpsdsp.o \
	h264_dsp.o h263dsp.o cavsdsp.o
	$(CC) $^ -o $@ $(EXTRA_FLAGS)

test: test.o h264-idct.o lossless_audiodsp.o aacencdsp.o aacpsdsp.o \
	h264_dsp.o h263dsp.o cavsdsp.o
	$(CC) $^ -o $@ $(EXTRA_FLAGS)

save_results:
	touch test_output_c.txt && touch test_output_cpp.txt && ../test > test_output_c.txt && ./test > test_output_cpp.txt

diff:
	make save_results && diff -c test_output_c.txt test_output_cpp.txt

clean:
	rm -f *.o
