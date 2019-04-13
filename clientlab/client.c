//Name: Prinn Prinyanut
//Class: CSCI 3761
//Description: lab 2 client

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
    char newFile[MAXDATASIZE];
    struct hostent *he;
    struct sockaddr_in their_addr; // connector's address information
    int saved_stdout;

    printf("------\n");
    printf("CLIENT\n"); //print out client
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
    printf("[+] Connecting to the server!\n"); //connected to server

    while(1){
        printf("\nFor command options type: 'help'\n"); //ask for user commnad input
        printf("> ");
        scanf("%s", &buf[0]); //store in buffer
        send(sockfd, buf, MAXDATASIZE, 0); //send buffer to server

        if (strcmp(buf, "catalog") == 0) { // catalog
            memset(buf, 0, MAXDATASIZE);
            if(recv(sockfd, buf, sizeof(buf), 0) < 0){ //recieve data
                printf("\nError!! Cannot recieve data.\n");
            }else{
                printf("%s", buf); //print out ls from server
            }
            continue;
        } else if (strcmp(buf, "ls") == 0) { // ls
            system("ls"); //print filenames in client directory
            continue;
        } else if (strcmp(buf, "pwd") == 0) { // pwd
            system("pwd"); //print client directory path
            continue;
        } else if (strcmp(buf, "spwd") == 0) { // spwd
            memset(buf, 0, MAXDATASIZE);
            if(recv(sockfd, buf, sizeof(buf), 0) < 0){ //recieve data
                printf("\nError!! Cannot recieve data.\n");
            }else{
                printf("%s", buf); //print out pwd from server
            }
            continue;
        } else if (strcmp(buf, "download") == 0) { // download
            memset(buf, 0, MAXDATASIZE);
            printf("\nEnter download source filename:\n"); //ask for user file
            printf("> ");
            scanf("%s", &buf[0]);
            send(sockfd, buf, MAXDATASIZE, 0); //send file name to server and check if it's valid
            //open file
            recv(sockfd, buf, MAXDATASIZE, 0); //check if fail 

            if (strcmp(buf, "fail") == 0) { //if server return fail (meaning can't find file)
                printf("[-] File not found in server.\n");
            } else {
                printf("\nEnter download destination filename:\n"); //ask for what name should the file be saved to
                scanf("%s", &newFile[0]);
                FILE *fp;
                fp = fopen(newFile, "w"); //creating new file
                memset(buf, 0, MAXDATASIZE);
                recv(sockfd, buf, MAXDATASIZE, 0); //get data from server
                fprintf(fp, buf); //print buffer stuff into newly created file
                fclose(fp);
                printf("[+] File downloaded.\n");
            }
            continue;
        } else if (strcmp(buf, "upload") == 0) { // upload
            printf("\nEnter upload source filename:\n"); //ask for file from client directory
            printf("> ");
            scanf("%s", &buf[0]); 
            FILE *f; 
            f = fopen(buf, "r"); //open file
            if (f == NULL) { //check if file exist
                printf("[-] Fail. Cannot find file in client!!\n");
            } else {
                printf("\nEnter upload destination filename:\n"); //ask for file name to be saved on server
                printf("> ");
                scanf("%s", &newFile[0]); //store des filename
                send(sockfd, newFile, MAXDATASIZE, 0); //sending the destination name to server
                fseek(f, 0, SEEK_END);
                rewind(f);
                memset(buf, 0, MAXDATASIZE);
                fread(buf, sizeof(char), sizeof(buf), f); //read from the source file in client
                send(sockfd, buf, MAXDATASIZE, 0); //send data to server
                fclose(f);
                printf("[+] File uploaded.\n");
            }
            continue;
        } else if (strcmp(buf, "help") == 0) { // help
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
        } else if(strcmp(buf, "bye") == 0){ // bye
            printf("\nDisconnected from server.\n");
            close(sockfd);
            exit(1);
        } else { //check for invalid commands
            printf("\nNot avaliable command!\n");
            continue;
        }
        memset(buf, 0, MAXDATASIZE); //clear buffer
        printf("\n");
    }
    return 0;
}
