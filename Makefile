test = 4
test_file = test/out$(test)_100iterations.txt
infile = input/in$(test).txt
outfile_serial = out/out$(test).txt
outfile_parallel = out/out$(test)_omp.txt
iterations = 100

clean:
	rm -rf g_serial g_omp

build_serial: g_serial.c
	gcc g_serial.c -o g_serial -g

build_parallel: g_omp.c
	gcc g_omp.c -o g_omp -fopenmp

run_serial:
	./g_serial $(infile) $(iterations) $(outfile_serial)

run_parallel:
	./g_omp $(infile) $(iterations) $(outfile_parallel)

debug_serial:
	gdb -tui --args ./g_serial $(in_file) $(iterations) out.txt

check_parallel: $(test_file) $(outfile_parallel)
	diff $(test_file) $(outfile_parallel)

check serial: $(test_file) $(outfile_serial)
	diff $(test_file) $(outfile_serial)

