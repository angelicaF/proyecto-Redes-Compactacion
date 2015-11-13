#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>

#include <string.h>


#include "zlib.h"
#define CHECK_ERR(err, msg) { \
    if (err != Z_OK) { \
        fprintf(stderr, "%s error: %d\n", msg, err); \
        exit(1); \
    } \
}


//const char hello[] = "hello, hello!";
/* "hello world" would be more standard, but the repeated "hello"
 * stresses the compression code better, sorry...
 */

//const char dictionary[] = "hello";
uLong dictId; /* Adler32 value of the dictionary */

const char hello[] = "hello, hello!";
#define SIZE 512

void doprocessing (int sock);
void mensajeEstado(int valorRetorno);

int main( int argc, char *argv[] ) {
   
   int sockfd, newsockfd, portno, clilen;
   char buffer[SIZE];
   struct sockaddr_in serv_addr, cli_addr;
   int n, pid;
   
   portno = 9876;
   /* First call to socket() function */
   sockfd = socket(AF_INET, SOCK_STREAM, 0);   
   printf("Abriendo Socket");
   mensajeEstado(sockfd);

   
   /* Initialize socket structure */
   bzero((char *) &serv_addr, sizeof(serv_addr));
   
   
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = INADDR_ANY;
   serv_addr.sin_port = htons(portno);
   
   /* Now bind the host address using bind() call.*/
   int r=bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
   printf("Binding Socket");
   mensajeEstado(r);
   
   /* Now start listening for the clients, here
      * process will go in sleep mode and will wait
      * for the incoming connection
   */
   
   n=listen(sockfd,5);
    printf("Listen");
      mensajeEstado(n);
   clilen = sizeof(cli_addr);
   
   while (1) {
      newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
      printf("Accept");
      mensajeEstado(newsockfd);
      
      
      /* Create child process */
      pid = fork();
      
      if (pid < 0) {
         perror("ERROR en el fork\n");
         exit(1);
      }
      
      if (pid == 0) {
         /* This is the client process */
         close(sockfd);
         doprocessing(newsockfd);
         exit(0);
      }
      else {
         close(newsockfd);
      }
      
   } /* end of while */
}

void doprocessing (int sock) {

   int n, id, st;
   char buffer[SIZE], name[128], buffer_dest[SIZE];
   bzero(buffer,512);

   /*n = read(sock,name,128);// Read the file name from client
   printf("Readind ");
   mensajeEstado(n);

   n=write(sock, "OK", 20);
   printf("writing ");
   mensajeEstado(n);

   strcat(name, ".recv");*/

   id = open( "name", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR );
   if ( -1 == id ) {
      printf( "Can not create file %s\n", "name" );
      exit( 1 );
   }

  

   while ((st = read(sock,buffer,SIZE)) > 0 ) { // Read returns EOF when we shutdown 
      printf("Reading compressed file ");
      mensajeEstado(st);
       uLong ucompSize = st; // "Hello, world!" + NULL delimiter.
         ucompSize++;
      uLong compSize = compressBound(ucompSize);
      // Inflate
      int err=uncompress((Bytef *)buffer_dest, &ucompSize, (Bytef *)buffer, compSize);
       CHECK_ERR(err, "uncomprss");
      //write( id, buffer_dest, st ); 
      //bzero(buffer_dest,SIZE);           // the writing socket in client
      //bzero(buffer,SIZE); 
   }

   n=write(sock,"ACK", 20 ); // Send an ACK to client, it will stop time counting upon receive
   printf("writing ");
   mensajeEstado(n);
   close ( id );
   exit( 0 );
   
}

void mensajeEstado(int valorRetorno){
if (valorRetorno < 0) {
      printf("  ERROR\n");
      exit(1);
   }else{
       printf("  CORRECTO\n");
   }

}