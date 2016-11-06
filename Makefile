clean:
	rm -rf g_serial g_omp
serial: g_serial.cpp
	g++ g_serial.cpp -o g_serial -g
parallel: g_omp.cpp
	g++ g_omp.cpp -o g_omp -fopenmp
run_serial:
	./g_serial input/in1.txt 2 out/out1.txt
run_parallel:
	./g_omp input/in1.txt 2 out/out1_omp.txt
#TODO Remove
debug_serial:
	gdb -tui --args ./g_serial input/in1.txt 2 out.txt
