all:
	gcc kpit.c -o main -lpthread
	gcc machine.c -o machine -lpthread
	gcc monitor.c -o monitor -lpthread
	gcc logger.c -o logger -lpthread