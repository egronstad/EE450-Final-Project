//serverM.c

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


void dont_steal_my_info(char info[]){
    //encrypt info
	for(int j=0; j<=strlen(info); j++){
		int info_ASCII = int(info[j]);
		if(info_ASCII>=48&&info_ASCII<=57){
			info_ASCII+=4;
			if(info_ASCII>57){
				info_ASCII-=10;
			}
			crypt_info[j]=(char)info_ASCII;
		}
		if(info_ASCII>=65&&info_ASCII<=90){
			info_ASCII+=4;
			if(info_ASCII>90){
				info_ASCII-=26;
			}
			crypt_info[j]=(char)info_ASCII;
		}
		if(info_ASCII>=97&&info_ASCII<=122){
			info_ASCII+=4;
			if(info_ASCII>122){
				info_ASCII-=26;
			}
			crypt_info[j]=(char)info_ASCII;
		}
		if((info_ASCII>=48 && info_ASCII<=57)||(info_ASCII>=65 && info_ASCII<=90)||(info_ASCII>=97 && info_ASCII<=122)){
            continue;
        }else{
            crypt_info[j]=info[j];
        }
	}
}

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

string username;
string find_user(string login_cred){
	//split
	stringstream ss(login_cred);
    string word;
    int num=1;
    getline(ss, word, ',');
    username=word;
    return word;
}

string query;
string course_code;
string category;

string find_code(string query){
	//split
	stringstream ss(query);
    string word;
    int num=1;
    getline(ss, word, ',');
    course_code=word;
    return word;
}

int dept_request;
string department;
/*
const char *send_to_c[BUFSIZE];
const char *send_to_cs[BUFSIZE];
const char *send_to_ee[BUFSIZE];
const char *send_to_client[BUFSIZE];
*/

char send_to_c[BUFSIZE];
char send_to_cs[BUFSIZE];
char send_to_ee[BUFSIZE];
char send_to_client[BUFSIZE];

char client_buf[BUFSIZE];
char c_buf[BUFSIZE];
char cs_buf[BUFSIZE];
char ee_buf[BUFSIZE];

char msg[BUFSIZE];

int read_len = 0;

int main(){
	//PHASE 2A
	//receive unencrypted information from client over TCP
	server_TCP();
	client_UDP();
	
	read_len = recv(newSd, (char*)&msg, sizeof(msg), 0);
	string login_cred = msg;
	username=find_user(login_cred);
	//After receiving the username and password from the client:
	cout<<"The main server received the authentication for "<<username<<" using TCP over port 25267.";

	//encrypt information
	strcpy(send_to_c, dont_steal_my_info(login_cred).c_str());

	//send encrypted information to the credential server
	//forward the authentication request to the credentials server over UDP
	//2. GOTO: serverC.c with string crypt_info
	sendto(main_UDP_sock, (const char *)send_to_c, strlen(send_to_c), MSG_CONFIRM, (const struct sockaddr *) &server_c_addr,  sizeof(server_c_addr)); 
	//Upon sending an authentication request to serverC
	cout<<"The main server sent an authentication request to serverC.";
	client_UDP();
	
	char event = recvfrom(main_UDP_sock, (char *)c_buf, BUFSIZE,  MSG_WAITALL, (struct sockaddr *) &server_c_addr, &sizeof(server_c_addr)); 
	
	//After receiving result of the authentication request from serverC:
	cout<<"The main server received the result of the authentication request from ServerC using UDP over port 24267.";
	
	//PHASE 2B
	send_to_client=event;
	//send result of the authentication request to the client over a TCP connection
	//4. GOTO: client.c with string event and/or corresponding integer
	send(server_TCP_sock,(char*)send_to_client,strlen(send_to_client),0);
	//After sending the authentication result to the client:
	cout<<"The main server sent the authentication result to the client.";

	//PHASE 3A
	//receive course query information from client over TCP
	//1 to represent EE
	//0 to represent CS
	read_len = recv(newSd, (char*)&msg, sizeof(msg), 0);
	string course_query = msg;
	//when receiving the information from the client, output an on-screen message
	//After receiving the query information from the client:
	cout<<"The main server received from "<<username<<" to query course "<<course_code<<" about "<<category<<" using TCP over port 25267.";
	//extract the department information from the query
	find_code(course_query);
	
	string ee_dept="EE";
	string cs_dept="CS";
	int dept_flag;
	string course_code;

	//check if CS or EE class
    if (course_code.substr(0,2)=="CS"){
    	department="CS";
        dept_flag=0;
    }else if (course_code.subtr(0,2)=="EE"){
   		department="EE";
        dept_flag=1;
    }
    
	//decide which department server has the corresponding information
	if (dept_flag==0){
		send_to_cs=query;
		//PHASE 3B
		//send the query information to the backend department server via UDP connection
		//6. GOTO: serverCS.c/serverEE.c with course_query
		sendto(main_UDP_sock, (const char *)send_to_cs, strlen(send_to_cs), MSG_CONFIRM, (const struct sockaddr *) &server_cs_addr,  sizeof(server_cs_addr));
		//output an on-screen message upon sending the request to the backend server
		//After querying EE or CS Department Server
		cout<<"The main server sent a request to server"<<department<<".";
		char course_info = recvfrom(main_UDP_sock, (char *)cs_buf, BUFSIZE,  MSG_WAITALL, (struct sockaddr *) &server_cs_addr, sizeof(server_cs_addr)); 
		//PHASE 4B
		//when main server receives the result, print out an on-screen message
		//After receiving result from EE or CS Department server i for query information:
		cout<<"The main server received the response from server"<<department<<" using UDP over port 24267.";
	}else if(dept_flag==1){
		send_to_cs=query;
		//PHASE 3B
		//send the query information to the backend department server via UDP connection
		//6. GOTO: serverCS.c/serverEE.c with course_query
		sendto(main_UDP_sock, (const char *)send_to_cs, strlen(send_to_cs), MSG_CONFIRM, (const struct sockaddr *) &server_cs_addr,  sizeof(server_cs_addr));
		//output an on-screen message upon sending the request to the backend server
		//After querying EE or CS Department Server
		cout<<"The main server sent a request to server"<<department<<".";
		char course_info = recvfrom(main_UDP_sock, (char *)cs_buf, BUFSIZE,  MSG_WAITALL, (struct sockaddr *) &server_cs_addr, sizeof(server_cs_addr)); 
		//PHASE 4B
		//when main server receives the result, print out an on-screen message
		//After receiving result from EE or CS Department server i for query information:
		cout<<"The main server received the response from server"<<department<<" using UDP over port 24267.";
	}
	//forward the result to the client using TCP
	//8. GOTO: client.c with course_info
	send(server_TCP_sock,send_to_client,strlen(send_to_client),0);
	//print out an on-screen message
	//After sending the query information to the client:
	cout<<"The main server sent the query information to the client.";
	return 0;
}
