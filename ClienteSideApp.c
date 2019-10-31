#include <stdio.h>
#include <sys/socket.h> 
#include <errno.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <ctype.h>


int main(int argc, char const *argv[]){

    int origin_socket, err,i, start, end;
    int serverIPAddress[4] = {};
    char hostname[100];
    struct sockaddr_in sa;
    struct hostent *host;

    start =1100; //Puerto de incio del escaneo
    end=1200;   //Puerto de fin del escaneo
/*
    //Ingreso la IP con verficacion que no se ingresen numeros invalidos
    do{
        printf("\n----------------------------Ingrese la direccion IP del Servidor----------------------------\n ");
        scanf("%d%*c%d%*c%d%*c%d%*c",&serverIPAddress[0],&serverIPAddress[1],&serverIPAddress[2],&serverIPAddress[3]);
    }while (serverIPAddress[0]<0 || serverIPAddress[0]>255 || serverIPAddress[1]<0 || serverIPAddress[1]>255 || serverIPAddress[2]<0 || serverIPAddress[2]>255 || serverIPAddress[3]<0 || serverIPAddress[3]>255);
    printf("La direccion IP ingresada es %d.%d.%d.%d\n",serverIPAddress[0],serverIPAddress[1],serverIPAddress[2],serverIPAddress[3]);
    
    for(i=0; i<=3; i=i+1){
        itoa(serverIPAddress[i],hostname,10);
        printf("%c", hostname[i]);
    }
*/
    printf("Ingrese la direccion IP: ");
    gets(hostname);
    //strncpy((char*) & amp;sa , "" , sizeof sa);
    sa.sin_family = AF_INET;
    if(isdigit(hostname[0])){
        sa.sin_addr.s_addr = inet_addr(hostname);
    }

        printf("Starting the portscan loop : \n");
    for( i = start ; i <= end ; i++) 
    {
        //Fill in the port number
        sa.sin_port = htons(i);
        //Create a socket of type internet
        origin_socket = socket(AF_INET , SOCK_STREAM , 0);
         
        //Check whether socket created fine or nots
        if(origin_socket < 0) 
        {
            printf("Error en puerto %d", i);
            perror("\nSocket");
            exit(1);
        }else{

        }
        //Connect using that socket and sockaddr structure
    
        err = connect(origin_socket , (struct sockaddr*)&amp;sa , sizeof sa);
         
        //not connected
        if( err < 0 )
        {
            //printf("%s %-5d %s\r" , hostname , i, strerror(errno));
            close(origin_socket);
            continue;
        }
        //connected
        else
        {
            printf("%-5d open\n",  i);
        }
        close(origin_socket);
    }
   
    return 0;
}


