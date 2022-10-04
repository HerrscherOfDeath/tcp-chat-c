#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h> // inet_addr()
#include <strings.h> // bzero()
#include <unistd.h> // read(), write(), close()
#include <sys/socket.h>
#define MAX 50
#define PORT 8080
#define SA struct sockaddr
#define IP "127.0.0.1"

void chat(int);

int main()
{
	int sockfd, connfd;
	struct sockaddr_in servaddr, cli;

	// creazione del socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");
	bzero(&servaddr, sizeof(servaddr));

	// assegnazione della Porta e IP
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(IP);
	servaddr.sin_port = htons(PORT);

	// connessione socket del client al socket del server 
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
		printf("connection with the server failed...\n");
		exit(0);
	}
	else
		printf("connected to the server..\n");


	chat(sockfd);
	close(sockfd);
}

// Funzione per la chat tra client e server
void chat(int sockfd)
{
	int n;
	char buff[MAX];
	// infinite loop
	for (;;) {
		bzero(buff, sizeof(buff));
		// prende il messaggio e lo coppia nel buffer
		printf("Enter the string : ");
		n = 0;
		while ((buff[n++] = getchar()) != '\n');
		// spedisce il buffer al server
		write(sockfd, buff, sizeof(buff));
		bzero(buff, sizeof(buff));
		// legge il buffer ricevuto dal server e lo stampa
		read(sockfd, buff, sizeof(buff));
		printf("From Server : %s", buff);

		// per terminare la conversazione basta scrivere "exit"
		if (strncmp("exit", buff, 4) == 0) {
			printf("Server Exit...\n");
			break;
		}
	}
}


