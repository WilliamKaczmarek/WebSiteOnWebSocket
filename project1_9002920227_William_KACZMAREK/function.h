/*! 
\file function.h
\author William Kaczmarek
\version 1
\date 22 april 2022
\brief function.h
*/

#ifndef __function_H_
#define __function_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <arpa/inet.h>

char *getContentType(char *ext);

int SendData(int sckt, const void *data, int datalen);

int SendStr(int sckt, const char *str);

#endif
