infile = input/in1.txt
outfile_serial = out/out1.txt
outfile_parallel = out/out1_omp.txt
iterations = 100000
clean:
	rm -rf g_serial g_omp
serial: g_serial.cpp
	g++ g_serial.cpp -o g_serial -g
parallel: g_omp.cpp
	g++ g_omp.cpp -o g_omp -fopenmp
run_serial:
	./g_serial $(infile) $(iterations) $(outfile_serial)
run_parallel:
	./g_omp $(infile) $(iterations) $(outfile_parallel)
#TODO Remove
debug_serial:
	gdb -tui --args ./g_serial input/in1.txt 2 out.txt
check:
	diff $(outfile_serial) $(outfile_parallel)
