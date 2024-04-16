server: webserver.c
	gcc webserver.c -o server

clean:
	rm -f *.o server