all:tcp_server tcp_client udp_server udp_client select clean.o

tcp_server: tcp_server.o 
	gcc -o tcp_server tcp_server.o 
tcp_client: tcp_client.o 
	gcc -o tcp_client tcp_client.o 
udp_server: udp_server.o
	gcc -o udp_server udp_server.o
udp_client: udp_client.o
	gcc -o udp_client udp_client.o
select: select.o
	gcc -o select select.o

clean.o:
	rm -f *.o
%.o:%.c
	gcc -c $< -o $@ 
clean: 
	rm -f select *server *client *.o 

