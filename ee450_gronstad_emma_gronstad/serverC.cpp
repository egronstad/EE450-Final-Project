/*
serverC.c
Each backend server should read the corresponding file and store the information in a certain data structure.

*/
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


#define CREDTXT "cred.txt"
#define C_PORT     21267 

#define BUFSIZE 1024

char main_buf[BUFSIZE];
char send_to_main[BUFSIZE];

string event = "";

string cred_check(string login_cred){
	//read file
	string file;
	cred_file.open(CREDTXT.c_str());
	string username;
	string password;

	//split
	stringstream ss(login_cred);
    string word;
    int num=1;
    while (!ss.eof()) {
        getline(ss, word, ',');
        if (num==1){
        	username=word;
        	num++;
        }else if(num==2){
        	password=word;
        }
    }

    //need to test
    //username check
	for (int i = 0; i <= cred_file.length()-username.length(); i++){
        for (int j = 0; j < username.length(); j++)
            if (cred_file[i + j] != username[j])
                break;
        if (j == username.length()){

        	//username & password check
		    for (int k = 0; k <= cred_file.length()-login_cred.length(); k++){
		        for (int l = 0; l < login_cred.length(); l++)
		            if (s2[k + l] != s1[l])
		                break;
		        if (l == login_cred.length())
		            return "login";
		    }
            return "pass error";
        }
    }
    return "user error"
}

int c_UDP_sock;
struct sockaddr_in servaddr, cliaddr; 
struct sockaddr_in main_addr;

void server_UDP(){
	//referenced https://www.geeksforgeeks.org/udp-server-client-implementation-c/
    // Creating socket file descriptor 
    if ((c_UDP_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("Error establishing the server socket"); 
        exit(1); 
    } 
        
    memset(&servaddr, 0, sizeof(servaddr)); 
    memset(&cliaddr, 0, sizeof(cliaddr)); 
        
    // Filling server information 
    servaddr.sin_family    = AF_INET; // IPv4 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
    servaddr.sin_port = htons(C_PORT); 
    
    memset(&main_addr, 0, sizeof(main_addr)); 
        
    // Filling server information 
    main_addr.sin_family    = AF_INET; // IPv4 
    main_addr.sin_addr.s_addr = INADDR_ANY; 
    main_addr.sin_port = htons(M_PORT); 
        
    // Bind the socket with the server address 
    if (bind(c_UDP_sock, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ) { 
        perror("Error binding socket to local address"); 
        exit(1); 
    } 
    //Booting Up (Only while starting):
	cout<<"The ServerC is up and running using UDP on port 21267.";
}

int main(){
	//PHASE 2A
	//receive authentication request w encrypted form of username and password
	char login = recvfrom(c_UDP_sock, (char *)main_buf, BUFSIZE,  MSG_WAITALL, (const struct sockaddr *) &main_addr, &sizeof(main_addr));
	//Upon receiving the request from main server:
	cout<<"The ServerC received an authentication request from the Main Server.";
	//3. GOTO: serverM.c with string event and/or corresponding integer
	send_to_main=cred_check(login);
	sendto(c_UDP_sock, (char *)send_to_main, strlen(send_to_main), MSG_CONFIRM, (struct sockaddr *) &main_addr,  sizeof(main_addr));
	//After sending the results to the main server:
	cout<<"The ServerC finished sending the response to the Main Server.";
	

}