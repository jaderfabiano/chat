#ifdef linux
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#endif
#include "chat.h"

/*Cria o servidor e aguarda o client conectar*/
void createServer( int *myfd, int *clifd, unsigned int port ) {

    struct sockaddr_in client; 
    struct sockaddr_in server; 
    socklen_t socksize = sizeof(struct sockaddr_in);
    printf("Servidor escutando porta %d\n", port);

    memset(&server, 0, sizeof(server));   
    server.sin_family = AF_INET;     
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);      

    *myfd = socket(AF_INET, SOCK_STREAM, 0);

    setsockopt( *myfd, SOL_SOCKET, SO_REUSEADDR, (int*)0, sizeof(int)); //Nao fica em TIME_WAIT quando o programa para a execucao
    bind(*myfd, (struct sockaddr *)&server, sizeof(struct sockaddr));

    listen(*myfd, 1);
    *clifd = accept( *myfd, (struct sockaddr *)&client, &socksize );
    printf("Conexao client IP: %s \n", inet_ntoa(client.sin_addr));
}

/*Recebe do client o canal associado no Redis e envia o Id da aplicacao*/
void initChat( unsigned int port ) {
    int myFileDescriptor, clientFileDescriptor;
    int bytes = -1;
    char bufferMsg[256] = {};
    myFileDescriptor = clientFileDescriptor = 0;
    createServer( &myFileDescriptor, &clientFileDescriptor, port);   

    while( bytes < 0 )  {	
	 
	    bytes =recv(clientFileDescriptor, bufferMsg,  sizeof( bufferMsg), 0 ) ;
	    if ( bytes >0 ) {
	        printf("Canal a ser usado no char %s \n", bufferMsg);
	    
	        send(clientFileDescriptor, "8888", strlen("8888"), 0); 
	       break;
	    }
    }
    
    close( myFileDescriptor );
    close( clientFileDescriptor );
    initRedis( bufferMsg, "8888");

}


