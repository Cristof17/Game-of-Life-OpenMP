clean:
	rm -rf g_serial g_omp
serial: g_serial.cpp
	g++ g_serial.cpp -o g_serial
parallel: g_parallel.cpp
	g++ g_parallel.cpp -o g_omp
