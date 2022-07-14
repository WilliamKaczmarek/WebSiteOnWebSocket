#include "function.h"


int main(int argc, char *argv[]){
    /* --------Define variable & Struct -------- */
    int create_socket, client_socket, check_recv, length_file_name, portno;
    int Count=0;
    char *buffer, *method, *File_name;
    int bufilesize = 1024;
    long filesize;
    struct sockaddr_in server_address, addr1, addr2;
    socklen_t addrlen;
	/* --------Get the port number & Initialize buffer & create a server socket-------- */
    if (argc < 2){
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
    buffer = (char *)malloc(bufilesize);
    if (!buffer){
        printf("0- ERROR The receive buffer was not allocated\n");
        exit(1);
    }
	printf("Buffer initialized |");

    create_socket = socket(AF_INET, SOCK_STREAM, 0);//af_inet = Internet socket and sock stream = tcp socket and 0 for the protocol
    if (create_socket == -1){
        perror("1- ERROR The socket was not created\n");
        exit(1);
    }
	printf(" Server socket created |");

	/* --------Define server server_address-------- */
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;// Connect to 0.0.0.0
    portno = atoi(argv[1]); //atoi converts from String to Integer
    server_address.sin_port = htons(portno);

	/* --------bind the socket to our specified IP and Port-------- */
    if (bind(create_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1){
        perror("2- ERROR The socket was not Bind\n");
        exit(1);
    }
    printf(" Bind success|");

	/* --------Make the socket listen for 5connections-------- */
    if (listen(create_socket, 5) == -1){
        perror("3- ERROR The socket isn't opened for listening");
        exit(1);
    }
    printf(" Server socket listening\n\n");
    printf("Now go in your web browser and type localhost:_portnumber you entered_/index.html\n");

    while (1){

        addrlen = sizeof(server_address);
        client_socket = accept(create_socket, (struct sockaddr *)&server_address, &addrlen);
		if (client_socket == -1){
            perror("A client was not accepted");
            exit(1);
        }

	    /* --------Receive data from the client-------- */
        check_recv = recv(client_socket, buffer, bufilesize, 0); //we receive the client request from client_socket and store it in the buffer to be analyze/parse
        if (check_recv == -1){
            perror("The data was not received");
            exit(1);
        }

	    /* --------Parse the HTTP request from the client-------- */
        method = strtok(buffer, "/"); //we only get the method of the buffer mostly "GET" it could be POST or another request
        File_name = strtok(NULL, " ");
        length_file_name = strlen(File_name);
        char char_file_name[length_file_name - 1];
        strcpy(char_file_name, File_name);
        printf("The method is %s name of the file is'%s'", method, char_file_name);

        /* --------Split in two strings char_file_name and extension-------- */
        char *extension = strtok(char_file_name, ".");
        char *char_file_name_extension = strtok(NULL, ".");
        char *response = getContentType(char_file_name_extension); //we split the extension, to put it in parameter of our function getContentType to return us the right http.... Content-type

        /* --------open the image file in read binary mode-------- */
        FILE *file = fopen(File_name, "rb");
        if (!file){ //In case the file resquested doesn't exist we close the client connection and the server to avoid any bug or infinite loading
            close(client_socket);
            close(create_socket);
            perror("The file was not opened");
            exit(1);
        }

        if (fseek(file, 0, SEEK_END) == -1){
            perror("The file was not seeked");
            exit(1);
        }

        filesize = ftell(file);
        if (filesize == -1){
            perror("The file size was not retrieved");
            exit(1);
        }
        rewind(file);

        char *msg = (char *)malloc(filesize);
        if (!msg){
            perror("The file buffer was not allocated\n");
            exit(1);
        }

        if (fread(msg, filesize, 1, file) != 1){
            perror("The file was not read\n");
            exit(1);
        }
        fclose(file);
        printf(", his size is %ld\n\n", filesize);

        if (!SendStr(client_socket, "HTTP/1.1 200 OK\r\n")){
            close(client_socket);
            continue;
        }
        char clen[40];
        sprintf(clen, "Content-length: %ld\r\n", filesize);
        if (!SendStr(client_socket, clen)){
            close(client_socket);
            continue;
        }
        if (!SendStr(client_socket, getContentType(char_file_name_extension)) == -1){
            close(client_socket);
            continue;
        }
        if (!SendData(client_socket, msg, filesize)){
            close(client_socket);
            continue;
        }
        
        /* --------Server and clients informations -------- */
        socklen_t serv_len = sizeof(server_address);

		getsockname(create_socket, (struct sockaddr *)&addr1, &serv_len);
		getsockname(client_socket, (struct sockaddr *)&addr2, &addrlen);
	    
        if (Count==0){	//loop to print it only one time and not spam the shell with our while loop
            printf("Host Server :\n");
            printf("Host Ip : %s\n", inet_ntoa(addr1.sin_addr));
            printf("Port : %d\n\n", ntohs(addr1.sin_port));

            printf("A client is connected here his information :\n");
            printf("Client Ip : %s\n", inet_ntoa(addr2.sin_addr));
            printf("Port : %d\n", ntohs(addr2.sin_port));

            Count++;
		}
        /* --------closing the client socket ------- */
        close(client_socket); //Very Important else even if everything is okay the file ll be loading forever and the client need to stop byhimself
    }

    /* --------closing the client socket & free the buffer------- */
    close(create_socket); //if for any reason we quit the loop while, we need to close the server socket
    free(buffer);
    return 0;
}

