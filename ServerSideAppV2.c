#include<stdio.h>
#include<string.h>    
#include<sys/socket.h>
#include<arpa/inet.h> 
#include<unistd.h>    
#include<string.h>
#include<stdlib.h>

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
	int ClientPort;     //cambiar, debe ser entre 1100 y 1120
	struct sockaddr_in  remoteAddr={0};
	

	ClientPort = rand() % (1120-1100+1) + 1100;
	printf("%d", ClientPort);


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
	strcpy(message,"Hola, Bienvenido al Servidor para TCP/IP 2019! Ingrese una cadena...");
	int j=0;
	char ch;

	//Creamos socket
	socket_desc = SocketCreate();
	if (socket_desc == -1)
	{
	 printf("No se pudo crear el socket.");
	 return 1;
	}
	printf("Socket Creado.\n");



	//Bind
	if( BindCreatedSocket(socket_desc) < 0){
	 //error si falla el bind
	 perror("Falló el bind.");
	 return 1;
	}
	printf("Bind realizado.\n");

	//Listen
	listen(socket_desc , 3);

	//Espera la conexion entrante

	while(1){

		printf("Esperando por conexiones entrantes...\n");
		clientLen = sizeof(struct sockaddr_in);

		//Acepta conexion de un cliente entrante
		sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&clientLen);
		if (sock < 0){
			perror("Falló el accept");
			return 1;
		}else{
			printf("Conexión aceptada\n");
			printf("La IP del cliente es: %s", inet_ntoa(client.sin_addr));
			break;
		}
    }
	
	
	if(send(sock , message , strlen(message) , 0) < 0){
		printf("Enviar falló");
		return 1;
	}
    memset(client_message,'\0', sizeof client_message);
	memset(message, '\0', sizeof message);
	//Recibir una respuesta del cliente
	if( recv(sock , client_message , 200 , 0) < 0){
		printf("falló recv");
		exit;
	}
	printf("Mensaje del  cliente: %s\n",client_message);

	while (client_message[j]) { //Transformo el mensaje a mayusculas
        ch = client_message[j]; 
        client_message[j] = toupper(ch) ; 
        j++; 
    }

	//Mandar cadena recibida en mayuscuslas
	if( send(sock , client_message , strlen(client_message) , 0) < 0){
		printf("Enviar falló");
		close(sock);
		return 1;
	}

	close(sock);
	sleep(1);	
    return 0;
}