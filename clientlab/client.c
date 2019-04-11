//
//  main.c
//  client
//
//  Created by Hieu Nguyen on 3/20/19.
//  Copyright © 2019 Hieu Nguyen. All rights reserved.
//
//  The code is based on Beej's Guide to Network Programming

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#define PORT 46000 // the port client will be connecting to
#define MAXDATASIZE 1024 // max number of bytes we can get at once
int main(int argc, char *argv[])
{
    int sockfd, numbytes;
    char buf[MAXDATASIZE];
    struct hostent *he;
    struct sockaddr_in their_addr; // connector's address information
    int saved_stdout;

    printf("------\n");
    printf("CLIENT\n");
    printf("------\n");

    if (argc != 2) {
        fprintf(stderr,"usage: client hostname\n");
        exit(1);
    }
    if ((he=gethostbyname(argv[1])) == NULL) {  // get the host info
        herror("gethostbyname");
        exit(1);
    }
    printf("[+] Getting the server info... \n");
    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {   // get the socket file
        perror("socket");
        exit(1);
    }
    printf("[+] Creating socket...\n");
    their_addr.sin_family = AF_INET;    // host byte order
    their_addr.sin_port = htons(PORT);  // short, network byte order
    their_addr.sin_addr = *((struct in_addr *)he->h_addr); // store server ip addess (from the
                                                           // gethostbyname function)
    memset(their_addr.sin_zero, '\0', sizeof their_addr.sin_zero);
    if (connect(sockfd, (struct sockaddr *)&their_addr,     // connect to server
                sizeof their_addr) == -1) {
        perror("connect");
        exit(1); 
    }

    printf("[+] Connecting to the server!\n");

    while(1){

        printf("\nFor command options type: 'help'\n");
        printf("> ");
        scanf("%s", &buf[0]);

        send(sockfd, buf, MAXDATASIZE, 0);

        if (strcmp(buf, "catalog") == 0) {
            if(recv(sockfd, buf, 1024, 0) < 0){
                printf("\nError!! Cannot recieve data.\n");
            }else{
                printf("%s\n", buf);
            }
            continue;
        } else if (strcmp(buf, "ls") == 0) {
            system("ls");
            continue;
        } else if (strcmp(buf, "pwd") == 0) {
            system("pwd");
            continue;
        } else if (strcmp(buf, "spwd") == 0) {
            if(recv(sockfd, buf, 1024, 0) < 0){
                printf("\nError!! Cannot recieve data.\n");
            }else{
                printf("%s\n", buf);
            }
            continue;
        } else if (strcmp(buf, "help") == 0) {
            printf("\nCommands:\n");
            printf("---------\n");
            printf("catalog - show files at the server’s current directory\n");
            printf("ls - show files at the client's current directory\n");
            printf("download [source-filename] [dest-filename] - download files\n");
            printf("upload [source-filename] [dest-filename] - upload files\n");
            printf("pwd - display client current directory\n");
            printf("spwd - display server current directory\n");
            printf("bye - disconnect from server\n");
            continue;
        } else if(strcmp(buf, "bye") == 0){
            //send(sockfd, buf, strlen(buf), 0);
            //close(clientSocket);
            printf("\nDisconnected from server.\n");
            close(sockfd);
            //memset(buf, 0, 1024);
            exit(1);
        } else {
            //send(sockfd, buf, strlen(buf), 0);
            printf("\nNot avaliable command!\n");
            //memset(buf, 0, 1024);
            continue;
        }
        
        memset(buf, 0, MAXDATASIZE);
        printf("\n");

    }

    // if ((numbytes=recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) { // receiving data from server
    //     perror("recv");
    //     exit(1); }
    // buf[numbytes] = '\0';

    // printf("Received: %s",buf); // displaying data received from server
    // close(sockfd); // closing the socket file (disconnect from server)
    // printf("client: closing the socket file \n");
    return 0;
}
