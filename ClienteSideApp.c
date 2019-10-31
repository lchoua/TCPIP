#include <stdio.h>
#include <sys/socket.h> 
#include <errno.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>


int main(int argc, char const *argv[]){

    int origin_socket, err,i, start, end;
    int serverIPAddress[4] = {};
    char hostname[100];
    struct sockaddr_in sa;
    struct hostent *host;

    start =1100; //Puerto de incio del escaneo
    end=1200;   //Puerto de fin del escaneo

    //Ingreso la IP con verficacion que no se ingresen numeros invalidos
    do{
        printf("\n----------------------------Ingrese la direccion IP del Servidor----------------------------\n ");
        scanf("%d%*c%d%*c%d%*c%d%*c",&serverIPAddress[0],&serverIPAddress[1],&serverIPAddress[2],&serverIPAddress[3]);
    }while (serverIPAddress[0]<0 || serverIPAddress[0]>255 || serverIPAddress[1]<0 || serverIPAddress[1]>255 || serverIPAddress[2]<0 || serverIPAddress[2]>255 || serverIPAddress[3]<0 || serverIPAddress[3]>255);
    printf("La direccion IP ingresada es %d.%d.%d.%d\n",serverIPAddress[0],serverIPAddress[1],serverIPAddress[2],serverIPAddress[3]);
    
    for(i=0; i<7; i=i+2){
        int x = i+1;
        int j = i/2;
        hostname[i]=serverIPAddress[j];
        hostname[x]= '.';
    }

    strncpy((char*) & amp;sa , "" , sizeof sa);
    sa.sin_family = AF_INET;
    if(isdigit(hostname[0])){
        printf("Doing inet_addr...");
        sa.sin_addr.s_addr = inet_addr(hostname);
        printf("Done\n");
    }

        printf("Starting the portscan loop : \n");
    for( i = start ; i &lt;= end ; i++) 
    {
        //Fill in the port number
        sa.sin_port = htons(i);
        //Create a socket of type internet
        sock = socket(AF_INET , SOCK_STREAM , 0);
         
        //Check whether socket created fine or not
        if(sock &lt; 0) 
        {
            perror("\nSocket");
            exit(1);
        }
        //Connect using that socket and sockaddr structure
        err = connect(sock , (struct sockaddr*)&amp;sa , sizeof sa);
         
        //not connected
        if( err &lt; 0 )
        {
            //printf("%s %-5d %s\r" , hostname , i, strerror(errno));
            fflush(stdout);
        }
        //connected
        else
        {
            printf("%-5d open\n",  i);
        }
        close(sock);
    }

    //printf("\n Conversion de la direccion: %d",inet_addr(hostname));
   



    //Creando el Socket Origen
    /*
	origin_socket = socket(AF_INET , SOCK_STREAM , IPPROTO_IP);   //El primer parametro indica la Suite TCP/IP, el segundo que es un Socket de flujo,
                                                                // y el tercero que utilizamos el protocolo IP
    

    if (origin_socket == -1)
        {
            printf("No se pudo crear el Socket");
        }
    */
    return 0;
}


