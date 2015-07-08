all:
	arm-hisiv100nptl-linux-gcc -c arm-backtrace.c -fno-omit-frame-pointer -mapcs -rdynamic -Wall
	arm-hisiv100nptl-linux-ar rs libarm-backtrace.a *.o
	arm-hisiv100nptl-linux-g++ test.cc -fno-omit-frame-pointer -mapcs -rdynamic -Wall -L./ -larm-backtrace -ldl -o test

.PHONY:clean
clean:
	rm -rf *.o *.a test
