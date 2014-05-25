#ifdef linux

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#endif
#include "chat.h"


void help () {
    printf("\t#################################\n");
    printf("\t#\t  USAGE         \t#\n");
    printf("\t#\t server --help  \t#\n");
    printf("\t#\t server --chat  \t#\n");
    printf("\t#################################\n");
}

/*Verifica o argumento passado*/
void chooseArgument ( char *argument ) {
    if ( ! strcmp( argument, "--help") ) {
		help();
    } else if ( ! strcmp ( argument , "--chat" ) ) {	
		initChat( 8567 );
    } else {
		printf( "Argumento invalido\n");
    }
}

/* Inicia aplicacao */
int main ( int argc, char *argv[] ) {
		
		if ( argc < 2 ) {
			help();
			return ( EXIT_FAILURE );
		} else {
			printf("Argumento %s\n", argv[1]);
			chooseArgument( argv[1] );
			return ( EXIT_SUCCESS );

		}
}
