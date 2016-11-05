clean:
	rm -rf g_serial g_omp
serial: g_serial.cpp
	g++ g_serial.cpp -o g_serial
parallel: g_parallel.cpp
	g++ g_parallel.cpp -o g_omp
run_serial:
	./g_serial input/in1.txt 100 out/out1.txt
