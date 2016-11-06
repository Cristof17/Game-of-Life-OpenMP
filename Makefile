infile = input/in1.txt
outfile_serial = out/out1.txt
outfile_parallel = out/out1_omp.txt
iterations = 10000
clean:
	rm -rf g_serial g_omp
serial: g_serial.c
	gcc g_serial.c -o g_serial -g
parallel: g_omp.c
	gcc g_omp.c -o g_omp -fopenmp
run_serial:
	./g_serial $(infile) $(iterations) $(outfile_serial)
run_parallel:
	./g_omp $(infile) $(iterations) $(outfile_parallel)
#TODO Remove
debug_serial:
	gdb -tui --args ./g_serial input/in1.txt 2 out.txt
check:
	diff $(outfile_serial) $(outfile_parallel)
