#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#define MAX 50
#define PORT 8080
#define SA struct sockaddr

void chat(int);

int main()
{
	int sockfd, connfd, len;
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
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	// associazione al socket appena creato un IP
	if((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
		printf("socket bind failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully binded..\n");

	// controllo se il server e' pronto per l'ascolto
	if ((listen(sockfd, 5)) != 0) {
		printf("Listen failed...\n");
		exit(0);
	}
	else
		printf("Server listening..\n");
	len = sizeof(cli);

	// controllo del arrivo dei dati dal client
	connfd = accept(sockfd, (SA*)&cli, &len);
	if (connfd < 0) {
		printf("server accept failed...\n");
		exit(0);
	}
	else
		printf("server accept the client...\n");

	chat(connfd);
	close(sockfd);
}

// Funzione per la chat tra client e server
void chat(int connfd)
{
	int n;
	char buff[MAX];
	// infinite loop
	for (;;) {
		bzero(buff, MAX);
		// legge il messaggio da client e lo coppia all'interno del buffer
		read(connfd, buff, sizeof(buff));
		// stampa del buffer
		printf("From client: %s\t To client : ", buff);
		bzero(buff, MAX);
		n = 0;
		// coppia il messaggio di server all'interno del buffer
		while ((buff[n++] = getchar()) != '\n');
		// invia il buffer con la risposta al client
		write(connfd, buff, sizeof(buff));

		// per terminare la conversazione basta scrivere "exit"
		if (strncmp("exit", buff, 4) == 0) {
			printf("Server Exit...\n");
			break;
		}
	}
}

