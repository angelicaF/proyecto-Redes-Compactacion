#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
//#include "Chrono.h" // Include the chrono interface
#include "zlib.h"
#include <stdio.h>

#define CHECK_ERR(err, msg) { \
    if (err != Z_OK) { \
        fprintf(stderr, "%s error: %d\n", msg, err); \
        exit(1); \
    } \
}
#define SIZE 512

void mensajeEstado(int valorRetorno);

int main(int argc, char *argv[]) {

//------------------------------------------------------------------

   //Chrono ci, cf;
   int sockfd, portno, n, id, st;
   long sent, psize;
   double rate;
   FILE *file;
   Byte * buffer [SIZE];
   Byte * buff_dest [SIZE];
   long filelen;
   int i;

   psize = 128 * 1024 * 1024; // Print info each 128 MB

   struct sockaddr_in serv_addr;
   struct hostent *server;
   
   
   if (argc < 3) {
      fprintf(stderr,"usage %s hostname port\n", argv[0]);
      exit(0);
   }

   file = fopen(argv[3], "r");         
   fseek(file, SEEK_SET, 0);
   int err;
   sent = 0;
   uLong ucompSize = st; // "Hello, world!" + NULL delimiter.
   uLong compSize = compressBound(ucompSize);

   portno = atoi(argv[2]);
   
   /* Create a socket point */
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   printf("Abriendo Socket");
   mensajeEstado(sockfd);
	
   server = gethostbyname(argv[1]);
   
   if (server == NULL) {
      fprintf(stderr,"ERROR, no such host\n");
      exit(0);
   }else{
      printf("host obtenido exitosamente\n");
   }

   bzero((char *) &serv_addr, sizeof(serv_addr));
   serv_addr.sin_family = AF_INET;
   bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
   serv_addr.sin_port = htons(portno);

   /*Now connect to the server */
   n=connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
   printf("Conectando Socket para enviar nombre");
   mensajeEstado(n);

   /*char buf_name[128];
   st = write(sockfd, argv[3], SIZE);
   printf("Wrting the file name");
   mensajeEstado(st);

   n = read(sockfd, buf_name, SIZE);
   printf("reading file name confirmation");
   mensajeEstado(n);
   printf("%s\n",buf_name);*/


  //ci.getTime(); // Start the time measurement

   sent = 0;
      while ( st = fread(buffer, 1,   SIZE, file)) { // Send the file   
      printf("va a comprimir\n");
      int err=compress((Bytef *)buff_dest, &compSize, (Bytef *)buffer, ucompSize);
      CHECK_ERR(err, "compress\n");
     // printf("va a escribir\n");
      n = write(sockfd, buff_dest, SIZE);
      printf("Wrting");
      mensajeEstado(n);
      bzero(buff_dest,SIZE);
      bzero(buffer,SIZE);
      sent += st;
      //printf("enviados\n");
      /*if ( (sent % psize) == 0 ) {  // Print info each 128 Mb, can be removed
         cf.getTime();
         cf -= ci;
         printf( "Time taken to transfer %ld bytes is: %ld sec., %ld ns\n", sent);//, cf.getSecs(), cf.getnSecs() );
         rate = cf.getSecs() + (cf.getnSecs()/1000000000);
         rate = (double) (sent / 1024 /1024) / rate;
         printf( "Transfer rate: %f MBps\n", rate );
      }*/
   }

   n=shutdown(sockfd,0);
   printf("Shutting Down Socket");
   mensajeEstado(n);
   char buff_res[128];
   /* Now read server response */
   bzero(buffer,SIZE);
   n = read(sockfd, buff_res, 128);
   //printf("reading ack");
   //mensajeEstado(n);
   printf( "%s\n", (char *)buff_res);  

   //cf.getTime();
   //cd-=ci;
   //printf( "Time taken to transfer %ld bytes is: %ld sec., %ld ns\n", sent, cf.getSecs(), cf.getnSecs() );

   /*printf( "%s\n", buffer );     // Print the string
   rate = cf.getSecs() + (cf.getNSecs()/1000000000);
   rate = (double) (sent / 1024 /1024) / rate;
   printf( "Total transfer rate: %f MBps\n", rate );
*/
   fclose(file);
   

   return 0;
}
void mensajeEstado(int valorRetorno){
if (valorRetorno < 0) {
      printf("  ERROR\n");
      exit(1);
   }else{
       printf("  CORRECTO\n");
   }

}



