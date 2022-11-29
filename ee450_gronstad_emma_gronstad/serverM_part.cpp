//serverM_part.cpp
#include <cstring>
#include <iostream>
#include <string.h>
#include <bits/stdc++.h>
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h> 
#include <sys/wait.h>

using namespace std;

//UDP PORTS
#define C_PORT     21267 
#define CS_PORT    22267
#define EE_PORT    23267
#define M_PORT     24267

#define BUFSIZE 1024

int client_buf_len;
int c_buf_len;
int cs_buf_len;
int ee_buf_len;

char crypt_info[BUFSIZE];

struct sockaddr_in sendSockAddr;
struct sockaddr_in my_addr;
int server_TCP_sock;
int newSd;

void server_TCP(){
	//heavily referenced https://github.com/bozkurthan/Simple-TCP-Server-Client-CPP-Example/blob/master/tcp-Server.cpp
	bzero((char*)&sendSockAddr,sizeof(sendSockAddr));
    sendSockAddr.sin_family = AF_INET;
    sendSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    sendSockAddr.sin_port = htons(server_TCP_sock);
    server_TCP_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(server_TCP_sock < 0){
        perror("Error establishing the server socket"); 
        exit(1);
    }
    int bindStatus = bind(server_TCP_sock, (struct sockaddr*) &sendSockAddr, sizeof(sendSockAddr));
    if(bindStatus < 0){
        cout << "Error binding socket to local address" << endl;
        exit(1);
    }
    listen(server_TCP_sock, 5);
    sockaddr_in newSockAddr;
    socklen_t newSockAddrSize = sizeof(newSockAddr);
    newSd = accept(server_TCP_sock, (sockaddr *)&newSockAddr, &newSockAddrSize);
    if(newSd < 0){
        cout << "Error accepting request from client!" << endl;
        exit(0);
    }
    //Booting Up (only while starting):
	cout<<"The main server is up and running.";
}

int main_UDP_sock;
struct sockaddr_in server_c_addr;
struct sockaddr_in server_cs_addr;
struct sockaddr_in server_ee_addr;
struct sockaddr_in server_m_addr;

void client_UDP(){
	//referenced https://www.geeksforgeeks.org/udp-server-client-implementation-c/
	main_UDP_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(main_UDP_sock < 0){
        perror("Error establishing the server socket"); 
        exit(1);
    }

    memset(&server_c_addr, 0, sizeof(server_c_addr)); 
    server_c_addr.sin_family = AF_INET; 
    server_c_addr.sin_port = htons(C_PORT); 
    server_c_addr.sin_addr.s_addr = INADDR_ANY; 

    memset(&server_cs_addr, 0, sizeof(server_cs_addr)); 
    server_cs_addr.sin_family = AF_INET; 
    server_cs_addr.sin_port = htons(CS_PORT); 
    server_cs_addr.sin_addr.s_addr = INADDR_ANY;     

    memset(&server_ee_addr, 0, sizeof(server_ee_addr)); 
    server_ee_addr.sin_family = AF_INET; 
    server_ee_addr.sin_port = htons(EE_PORT); 
    server_ee_addr.sin_addr.s_addr = INADDR_ANY;  

    memset(&server_m_addr, 0, sizeof(server_m_addr)); 
    server_m_addr.sin_family = AF_INET; 
    server_m_addr.sin_port = htons(M_PORT); 
    server_m_addr.sin_addr.s_addr = INADDR_ANY;

    int bindStatus = bind(main_UDP_sock, (struct sockaddr*) &sendSockAddr, sizeof(sendSockAddr));
    if(bindStatus < 0){
        perror("Error binding socket to local address"); 
        exit(1);
    }
}

int main(){
	//PHASE 2A
	//receive unencrypted information from client over TCP
	server_TCP();
	client_UDP();
}