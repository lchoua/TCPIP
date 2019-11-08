#include<stdio.h>
#include<string.h>    
#include<sys/socket.h>
#include<arpa/inet.h> 
#include<unistd.h>    
#include<string.h>

short SocketCreate(void)
{

	short hSocket;
	printf("Creamos el socket.\n");

	hSocket = socket(AF_INET, SOCK_STREAM, 0);
	return hSocket;
}


int BindCreatedSocket(int hSocket)
{

	int returnValue=-1;
	int ClientPort = 90190;     //cambiar, debe ser entre 1100 y 1120
	struct sockaddr_in  remoteAddr={0};


	remoteAddr.sin_family = AF_INET; //IPv4
	remoteAddr.sin_addr.s_addr = htonl(INADDR_ANY); //Acepta cualquier conexion entrante
	remoteAddr.sin_port = htons(ClientPort); // Utilizo temporalmente ese puerto para testear conexion
	returnValue = bind(hSocket,(struct sockaddr *)&remoteAddr,sizeof(remoteAddr));

        return returnValue;
}


int main(int argc , char *argv[])
{
	int socket_desc , sock , clientLen , read_size;
	struct sockaddr_in server , client;
	char client_message[200]={0};
	char message[100] = {0};
	const char *pMessage = "Hola, Bienvenido!";

	//Creamos socket
	socket_desc = SocketCreate();
	if (socket_desc == -1)
	{
	 printf("No se pudo crear el socket.");
	 return 1;
	}
	printf("Socket Creado.\n");



	//Bind
	if( BindCreatedSocket(socket_desc) < 0)
	{
	 //error si falla el bind
	 perror("Falló el bind.");
	 return 1;
	}
	printf("Bind realizado.\n");

	//Listen
	listen(socket_desc , 3);

	//Espera la conexion entrante

	while(1)
	{

	printf("Esperando por conexiones entrantes...\n");
	clientLen = sizeof(struct sockaddr_in);

	//Acepta conexion de un cliente entrante
	sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&clientLen);
	if (sock < 0)
	{
	 perror("Falló el accept");
	 return 1;
	}
	printf("Conexión aceptada\n");
    }

    memset(client_message,'\0', sizeof client_message);
	memset(message, '\0', sizeof message);
	//Recibir una respuesta del cliente
	if( recv(sock , client_message , 200 , 0) < 0)
	{
		printf("falló recv");
		break;
	}
	
	printf("Respuesta del cliente: %s\n",client_message);


	if(strcmp(pMessage,client_message)==0)
	{

	 strcpy(message,"Hola, Bienvenido cliente!");
	}
	else
	{

	  strcpy(message,"Mensaje inválido!");

	}

	//Mandar Datos
	if( send(sock , message , strlen(message) , 0) < 0)
	{
		printf("Enviar falló");
		return 1;
	}

	close(sock);
	sleep(1);
	
    return 0;
}