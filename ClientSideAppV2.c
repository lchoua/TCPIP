#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/time.h>
#include<arpa/inet.h>
#include<unistd.h>


//Crear el socket para comunicarse con el servidor
short SocketCreate(void)
{
		short hSocket;
		hSocket = socket(AF_INET, SOCK_STREAM, 0);
        return hSocket;
}

//tratamos de conectarnos con el servidor
int SocketConnect(int hSocket, int hostAddress)
{
		int i, start=1100, end=1200;
		int returnValue=-1;
		struct sockaddr_in remoteAddr={0};
		printf("\nEmpezando el escaneo de puertos entre el %d y el %d: \n", start, end);
    	for( i = start ; i <= end ; i++) 
    		{
				remoteAddr.sin_addr.s_addr = inet_addr(hostAddress); //Local Host
        		remoteAddr.sin_family = AF_INET;   //IPv4
        		remoteAddr.sin_port = htons(ServerPort);
        		returnValue = connect(hSocket , (struct sockaddr *)&remoteAddr , sizeof(struct sockaddr_in));
				if(returnValue != -1){
					char host[128];
            		char service[128];
					getnameinfo((struct sockaddr *)&remoteAddr, sizeof remoteAddr, host, (sizeof host), service, sizeof service, 0);
					printf("\nPuerto: %d, Servicio: %s, Abierto\n", i, service);
					break;
				}
				else{
					printf("\nPuerto: %d, Cerrado\n", i);
				}
			}
	return returnValue;
}


// Mandamos al servidor y esperamos timeout de 20 segundos.
int SocketSend(int hSocket,char* Rqst,short lenRqst)

{

        int returnValue = -1;
        struct timeval timeOut;
        timeOut.tv_sec = 20;  // Timeout de 20 segundos
        timeOut.tv_usec = 0;  

        if(setsockopt(hSocket, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeOut,sizeof(timeOut)) < 0)
        {
          printf("Time Out\n");
          return -1;
        }
        returnValue = send(hSocket , Rqst , lenRqst , 0);

        return returnValue;
 }


//Recibir datos del servidor, tambien con timeout de 20 segundos 
int SocketReceive(int hSocket,char* Rsp,short RvcSize)
{

	int returnValue = -1;
	struct timeval timeOut;
	timeOut.tv_sec = 20;  /* 20 Secs Timeout */
	timeOut.tv_usec = 0;  

	if(setsockopt(hSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeOut,sizeof(timeOut)) < 0)
	{
	  printf("Time Out\n");
	  return -1;

	}
	returnValue = recv(hSocket, Rsp , RvcSize , 0);

	printf("La respuesta del servidor es: %s\n",Rsp);

	return returnValue;
 }

 
 //Principal
int main(int argc , char *argv[])
{
	int hSocket, read_size;
	struct sockaddr_in server;
	struct hostent *host;
	char SendToServer[100] = {0};
	char server_reply[200] = {0};
	char hostAddress;

	printf("Ingrese la direccion IP (o el nombre) del servidor: ");
    gets(hostAddress);
    printf("El usuario ingreso %s\n", hostAddress);
    //Compruebo si ingreso una direccion IP o un nombre
    if((host = gethostbyname(hostAddress)) == NULL) //Puede resolver el nombre o ip ?
		{ 
			printf ("No se puede resolver el nombre o dirección\n");
			exit(-1);
		}

	//Crear socket
	hSocket = SocketCreate();
	if(hSocket == -1)
	{
	  printf("No se pudo crear el socket.\n");
	  return 1;
	}

	printf("Se creó el socket.\n");

	//Conectarse al servidor remoto
	if (SocketConnect(hSocket, hostAddress) < 0)
	{
		perror("conexión fallida.\n");
		return 1;
	}

	printf("Se conectó satisfactoriamente al servidor.\n");

	printf("Enviar el mensaje: ");
	gets(SendToServer);

	//Mandamos los datos al servidor
	SocketSend(hSocket , SendToServer , strlen(SendToServer));


    //Recibimos los datos del servidor
	read_size = SocketReceive(hSocket , server_reply , 200);
	
	printf("Respuesta del servidor: %s\n\n",server_reply);


	close(hSocket);
	shutdown(hSocket,0);
	shutdown(hSocket,1);
	shutdown(hSocket,2);
    return 0;
}