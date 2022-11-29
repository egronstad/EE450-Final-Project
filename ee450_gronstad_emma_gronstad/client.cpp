/*
client.c


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

#define port 	25267
#define BUFSIZE 1024

using namespace std;

string username;
string password;
string login_cred;

void login(){
	cout<<"Please enter the username: ";
	getline(cin, username);
   	cout<<"Please enter the password: ";
	getline(cin, password);
	login_cred=username+","+password;
}

string course_code;
string category;
string course_query;

string query(){
	cout<<"Please enter the course code to query: ";
	getline(cin, course_code);
	cout<<"Please enter the category (Credit/Professor/Days/CourseName): ";
	getline(cin, category);
	course_query=course_code+","+category;
	return course_query;
}

struct sockaddr_in servAddr;
struct sockaddr_in my_addr;
int client_TCP_sock;
unsigned int client_port;
int client_fd;

void client_TCP(){
	//heavily referenced https://github.com/bozkurthan/Simple-TCP-Server-Client-CPP-Example/blob/master/tcp-Server.cpp
	servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(port);

    client_TCP_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(client_TCP_sock < 0){
        cout<<("Error establishing the server socket"); 
        //exit(1);
    }
    
    if (inet_pton(AF_INET, "127.0.0.1", &servAddr.sin_addr) <= 0) {
        cout<<"Address not supported"<<endl;
    }
    
    if ((client_fd = connect(sock, (struct sockaddr*)&servAddr, sizeof(servAddr))) < 0) {
        cout<<"Connection Failed"<<endl;
    }
    
    int bindStatus = bind(client_TCP_sock, (struct sockaddr*) &servAddr, sizeof(servAddr));
    if(bindStatus < 0){
        cout<<("Error binding socket to local address"); 
        //exit(1);
    }
    bzero((char*)&my_addr, sizeof(my_addr));
    socklen_t len = sizeof(my_addr);
  	getsockname(client_TCP_sock, (struct sockaddr*) &my_addr, (socklen_t*)&len);
    client_port = ntohs(my_addr.sin_port);
    //exit(0);
}

int login_attempt=3;
int authentication_flag=0;
string event;

char cred_buf[BUFSIZE];
char query_buf[BUFSIZE];
char send_to_main[BUFSIZE];

int read_len;

int main(){
	client_TCP();
	cout<<"The client is up and running."<<endl;
	//Booting Up:
	
	while(1){
		//If not authorized yet
		if(authentication_flag!=1){
			//ask to enter the username and password on the terminal
			if(login_attempt>=0){//PHASE 1A
				//send authentication request to the serverM over TCP connection
				//1. GOTO: serverM.c with string login_cred & username
				
				login();
				cout<<login_cred<<endl;
				strcpy(send_to_main, login_cred.c_str());
				cout<<send_to_main<<endl;
				send(client_TCP_sock,(char*)send_to_main,strlen(send_to_main),0);
				
				//Upon sending authentication request to Main Server:
				cout<<username<<" sent an authentication request to the main server.";
				
				//PHASE 2B
				read_len = recv(client_TCP_sock, cred_buf, sizeof(cred_buf), 0);
				string event=cred_buf;
				//display result of authentication request (on-screen message) on the client screen
				//if the result of the authentication request is a failure then the client will have two more attempts
				if (event=="login"){
					//After receiving the result of the authentication request from Main server (if the authentication passed):
					authentication_flag=1;
					cout<<username<<" received the result of authentication using TCP over port "<<port<<". Authentication is successful";
				}else if(event=="user error"){
					//After receiving the result of the authentication request from Main server (username does not exist): 
					//login_attempt=2,1,0 after the First,second and third attempt respectively
					login_attempt--;
					cout<<username<<" received the result of authentication using TCP over port "<<port<<". Authentication failed: Username does not exist"<<endl<<"Attempts remaining: "<<login_attempt;
				}else if(event=="pass error"){
					//After receiving the result of the authentication request from Main server (Password does not match): 
					//login_attempt=2,1,0 after the First,second and third attempt respectively
					login_attempt--;
					cout<<username<<" received the result of authentication using TCP over port "<<port<<". Authentication failed: Password does not match"<<endl<<"Attempts remaining: "<<login_attempt;
				}
			}else{
				//if all of the attempts fail then the client shuts down after indication that all 3 attempts failed
				//After receiving the result of the authentication request from Main server (Failure after 3 attempts): 
				cout<<"Authentication Failed for 3 attempts. Client will shut down.";
				//Shut down client.
			}
		}else{
			//if any of the athentication attempts pass then the client stays on until it is manually shut down
			query();
			//PHASE 3A
			//send course query request to serverM over TCP connection
			//5. GOTO serverM.c with string course_query
			strcpy(send_to_main, course_query.c_str());
			send(client_TCP_sock,(char*)send_to_main,strlen(send_to_main),0);
			//upon sending the request to the main server, output an on-screen message
			//Upon sending the request to Main server:
			cout<<username<<" sent a request to the main server.";

			//PHASE 4B
			read_len = recv(client_TCP_sock, query_buf, sizeof(query_buf), 0);
			string info = query_buf; 
			//when the client receives the result, print out the query information
			//After receiving the query information from the Main server:
			cout<<"The client received the response from the Main server using TCP over port "<<port<<".";
			//print out the prompt messages for a new request
			if(query_buf){
				//If the information is successfully found:
				cout<<"The "<<category<<" of "<<course_code<<" is "<<info<<".\n\n-----Start a new request-----\nPlease enter the course code to query: ";
			}else{
				//If the course is not found:
				cout<<"Didn't find the course: "<<course_code<<".\n\n-----Start a new request-----\nPlease enter the course code to query: ";
			}
		}
	}
	
	return 0;

	
}