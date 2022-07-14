/*! 
\file function.c
\author William Kaczmarek
\version 1
\date 17 april 2022
\brief function.h
*/


#include "function.h"

int SendData(int sckt, const void *data, int datalen)
{
    const char *pdata = (const char *)data;

    while (datalen > 0)
    {
        int numSent = send(sckt, pdata, datalen, 0);
        if (numSent <= 0)
        {
            if (numSent == 0)
            {
                printf("The client was not written to: disconnected\n");
            }
            else
            {
                perror("The client was not written to");
            }
            return 0;
        }
        pdata += numSent;
        datalen -= numSent;
    }

    return 1;
}

int SendStr(int sckt, const char *str)
{
    return SendData(sckt, str, strlen(str));
}

// Funtion with the extention file return the content type switch case

char *getContentType(char *ext){
    if (strcmp(ext, "html") == 0){
        return "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
    }
    else if (strcmp(ext, "css") == 0){
        return "HTTP/1.1 200 OK\r\nContent-Type: text/css\r\n\r\n";
    }
    else if (strcmp(ext, "png") == 0){
        return "HTTP/1.1 200 OK\r\nContent-Type: image/png\r\n\r\n";
    }
    else if (strcmp(ext, ".jpg") == 0){
        return "HTTP/1.1 200 OK\r\nContent-Type: image/jpeg\r\n\r\n";
    }
    else if (strcmp(ext, "gif") == 0){
        return "HTTP/1.1 200 OK\r\nContent-Type: image/gif\r\n\r\n";
    }
    else if (strcmp(ext, "pdf") == 0){
        return "HTTP/1.1 200 OK\r\nContent-Type: application/pdf\r\n\r\n";
    }
    else if (strcmp(ext, "txt") == 0){
        return "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n";
    }
    else if (strcmp(ext, "ico") == 0){
        return "HTTP/1.1 200 OK\r\nContent-Type: image/x-icon\r\n\r\n";
    }
    else{
        return "text/plain";
    }
}