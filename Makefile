clean:
	rm -rf g_serial g_omp
serial: g_serial.cpp
	g++ g_serial.cpp -o g_serial -g
parallel: g_parallel.cpp
	g++ g_parallel.cpp -o g_omp
run_serial:
	./g_serial input/in5.txt 2 out/out5.txt
#TODO Remove
debug_serial:
	gdb -tui --args ./g_serial input/in1.txt 2 out.txt
