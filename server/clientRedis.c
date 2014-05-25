#ifdef linux
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include "hiredis.h"
#include "async.h"
#include "adapters/libevent.h"
#endif

char globalChannel[128];
char myId[128];

/*Configura id do Servidor*/
void setMyID( char *id ) {
    sprintf(myId, "serverC-%s", id);
}

/*Callback para finalizar a aplicacao*/
void finishChannel(redisAsyncContext *redis, void *r, void *priv ) {    
    printf("Chat Finalizado\n");
    exit(EXIT_SUCCESS);
}

/*Callback chamado a cada nova mensagem no Redis*/
void subCallback( redisAsyncContext *redis, void *r, void *priv) {
    redisReply *reply = r;
 
    if (reply == NULL) return;

    if ( ( reply->type == REDIS_REPLY_ARRAY ) && ( reply->elements == 3)  ){
	    if ( strcmp( reply->element[0]->str, "subscribe" ) != 0 ) {
	        if ( strstr(reply->element[2]->str, myId) == NULL)
                printf("\t%s", reply->element[2]->str);
            if (strstr(reply->element[2]->str, "fim") != NULL) {               
                redisAsyncCommand(redis, finishChannel, (char*) "sub", globalChannel);      
              
            }
	    }
    }
}
/*Callback para sinalizar a conexao com o Redis*/
void connectCallback( const redisAsyncContext *c, int status ) {
    char subs[256] = {};
    strcpy(subs, "SUBSCRIBE ");
    strcat(subs, globalChannel );
    if (status != REDIS_OK ) {
	   printf("Error: %s\n", c->errstr);
	   exit( EXIT_FAILURE);
    }

   redisAsyncCommand(c, subCallback, (char*) "sub", subs);
}

/*Callback para sinalizaar a perda de conexao com Redis*/
void disconnectCallback(const redisAsyncContext *c, int status){
    if (status != REDIS_OK){
    	printf("Error: %s\n", c->errstr);
	    exit( EXIT_FAILURE);
    }
    printf("Desconectado \n");
}

   
/*Cria um processo filho para capturar as mensagens do teclado*/
void initStdin(const char *channel ) {
    pid_t pid;

    pid = fork();
    if ( pid == 0) {
        printf("\n\t Chat Iniciado!!!\n\n");
        redisContext *redis;
        redis = redisConnect("localhost", 6379);        
        if ( redis->err  == 0) {                        
            while( 1 > 0){
                redisReply *reply ;
                char msg[256] = {}, buffer[256] = {};                          
                strcat(msg, myId);
                strcat(msg, " ");
                scanf("%[^\n]s", buffer);  
                setbuf(stdin, NULL);                           
                strcat(msg, buffer);                
                reply = redisCommand(redis, "PUBLISH %s %s", channel, msg );
                freeReplyObject(reply);
            }
        }
    }
}

/*Inicia conexao com o Redis e configura as funcoes de callback no libEvent*/
void initRedis( const char * channel, char * id) {
    signal(SIGPIPE, SIG_IGN);
    struct event_base *base = event_base_new();
    strcpy(globalChannel, channel);    
    setMyID(id); 
    redisAsyncContext * redis = redisAsyncConnect("127.0.0.1", 6379);
    if ( redis->err ) {
	   printf("Erro conectando no redis %s\n", redis->errstr);
	   exit(EXIT_FAILURE);
    }
   
    initStdin( channel);

    redisLibeventAttach( redis,base);
    redisAsyncSetConnectCallback(redis, connectCallback);
    redisAsyncSetDisconnectCallback( redis,disconnectCallback);
    event_base_dispatch(base);

}
