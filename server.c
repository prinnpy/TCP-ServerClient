//
//  main.c
//  server
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
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define MYPORT 46000 // the port users will be connecting to
#define MAXDATASIZE 1024 // max number of bytes we can get at once
#define BACKLOG 10   // how many pending connections queue will hold

using namespace std;

void sigchld_handler(int s)
{
    while(waitpid(-1, NULL, WNOHANG) > 0);
}

int main(void)
{
    int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
    struct sockaddr_in my_addr; // my address information
    struct sockaddr_in their_addr; // connector's address information
    char buf[MAXDATASIZE];
    int saved_stdout;

    printf("------\n");
    printf("SERVER\n");
    printf("------\n");

    socklen_t sin_size;
    struct sigaction sa;
    int yes=1;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {  // get the socket file
        perror("socket");
        exit(1);
    }
    
    printf("[+] Socket created... \n");
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
        perror("setsockopt");
        exit(1);
    }
    my_addr.sin_family = AF_INET;        // host byte order
    my_addr.sin_port = htons(MYPORT);    // short, network byte order
    my_addr.sin_addr.s_addr = INADDR_ANY; // automatically fill with my IP
    memset(my_addr.sin_zero, '\0', sizeof my_addr.sin_zero);
    if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof my_addr) == -1) {
        perror("bind");
        exit(1); }
    printf("[+] Binding...\n");
    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }
    printf("[+] Listening for connections...\n");
    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1); }
    while(true) {  // main accept() loop
        sin_size = sizeof their_addr;
        if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, \
                             &sin_size)) == -1) {
            perror("accept");
            continue;
        }
        printf("[+] Connection from %s\n", \
                    inet_ntoa(their_addr.sin_addr));

        if (new_fd < 0) {
            printf("[-] Can't connect!\n");
        } else {
            int Fork = fork();
            if (Fork < 0) {
                printf("[-] Issue creating a fork!\n");
            } else if (Fork == 0) {

                while(true) {

                    recv(new_fd, buf, MAXDATASIZE, 0);

                    if (strcmp(buf, "catalog") == 0) {
                        printf("> Client requested 'catalog'\n");
                        saved_stdout = dup(1);
                        dup2(new_fd, 1);
                        system("ls");
                        dup2(saved_stdout, 1);
                        close(saved_stdout);
                        //send(new_fd, "catalog", sizeof(buf), 0);
                        continue;
                    //break;
                    } else if (strcmp(buf, "spwd") == 0) {
                        printf("> Client requested 'spwd'\n");
                        saved_stdout = dup(1);
                        dup2(new_fd, 1);
                        system("pwd");
                        dup2(saved_stdout, 1);
                        close(saved_stdout);
                        //send(new_fd, "pwd", sizeof(buf), 0);
                        continue;
                    } else if (strcmp(buf, "ls") == 0) {
                        printf("> Client requested 'ls'\n");
                        continue;
                    } else if (strcmp(buf, "pwd") == 0) {
                        printf("> Client requested 'pwd'\n");
                        continue;
                    } else if (strcmp(buf, "help") == 0) {
                        printf("> Client requested 'help'\n");
                        continue;
                    } else if (strcmp(buf, "bye") == 0){
                        printf("Disconnect from: %s\n", \
                            inet_ntoa(their_addr.sin_addr));
                        close(new_fd);
                        break;
                    } else {
                        printf("> Command not found!\n");
                        //printf(buf);
                        //printf("\nls or pwd\n");
                        continue;
                    }
                    
                    memset(buf, 0, MAXDATASIZE); //clear buffer everytime

                } //while loop
            } //else if
        } //else
    } //big while loop

    close(new_fd);
    return 0; 
    
}

