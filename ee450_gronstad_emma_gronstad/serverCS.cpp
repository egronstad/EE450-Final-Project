/*
serverCS.c
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
#include <cstring>

using namespace std;

#define DEPTFILE "cs.txt"
#define CS_PORT    22267
#define EE_PORT    23267
#define M_PORT     24267
#define BUFSIZE 1024

char main_buf[BUFSIZE];
char send_to_main[BUFSIZE];
int cs_UDP_sock;
struct sockaddr_in servaddr, cliaddr; 
struct sockaddr_in main_addr;

void server_UDP(){ 
	//referenced https://www.geeksforgeeks.org/udp-server-client-implementation-c/
    // Creating socket file descriptor 
    if ((cs_UDP_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("Error establishing the server socket"); 
        exit(1); 
    } 
        
    memset(&servaddr, 0, sizeof(servaddr)); 
    memset(&cliaddr, 0, sizeof(cliaddr)); 
        
    // Filling server information 
    servaddr.sin_family    = AF_INET; // IPv4 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
    servaddr.sin_port = htons(CS_PORT); 
    
    memset(&main_addr, 0, sizeof(main_addr)); 
        
    // Filling server information 
    main_addr.sin_family    = AF_INET; // IPv4 
    main_addr.sin_addr.s_addr = INADDR_ANY; 
    main_addr.sin_port = htons(M_PORT); 
        
    // Bind the socket with the server address 
    if (bind(cs_UDP_sock, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ) { 
        perror("Error binding socket to local address"); 
        exit(1); 
    } 
    //Booting Up (Only while starting):
	cout<<"The ServerCS is up and running using UDP on port 22267.";
}

vector<string> cs_vec;
string word;
fstream cs_file;

void extract_dept_file(){
	cs_file.open(DEPTFILE.c_str());
    while(cs_file>>word){
    	cs_vec.push_back(word);
    }
}

int code_index;
int info_index;
int found;

string find_info(string course_code, string category){
	if (find(cs_vec.begin(), cs_vec.end(), course_code) != cs_vec.end()){
		info_index=code_index;
	    if (category=="Credit"){
	    	info_index+=1;
	    }else if(category=="Professor"){
	    	info_index+=2;
	    }else if(category=="Days"){
	    	info_index+=3;
	    }else if(category=="CourseName"){
	    	info_index+=4;
	    }
	    return cs_vec[info_index];
	}else{
		return "0";
	}
}

string course_code;
string category;

void query_split(string query){
	stringstream ss(query);
    string word;
    int num=1;
    while (!ss.eof()) {
        getline(ss, word, ',');
        if (num==1){
        	course_code=word;
        	num++;
        }else if(num==2){
        	category=word;
        }
    }
}

int main_buf_len;

int main(){
	//PHASE 3B
	//after getting the query information, look through stored local information to obtain the corresponding course information.
	unsigned int len= sizeof(main_addr);
	main_buf_len = recvfrom(cs_UDP_sock, (char *)main_buf, BUFSIZE,  MSG_WAITALL, (const struct sockaddr *) &main_addr, &len); 
	string query = main_buf;
	//After receiving the request from main server:
	cout<<"The ServerCS received a request from the Main Server about the "<<category<<" of "<<course_code<<".";
	extract_dept_file();
	//Split course_query info into course_code & category
	string course_query = query;
	query_split(course_query);
	string course_info = find_info(course_code, category);
	
	if (course_info!="0"){
		//If the course is found:
		cout<<"The course information has been found: The "<<category<<" of "<<course_code<<" is "<<course_info<<".";
	}else{
		//If the course if not found:
		cout<<"Didn't find the course: "<<course_code<<".";
	}

	//PHASE 4A
	//have the query information ready
	send_to_main = course_info;
	//query information is sent back to the Main server using UDP
	//7. GOTO: serverM.c with course_info
	sendto(cs_UDP_sock, (char *)send_to_main, strlen(send_to_main), MSG_CONFIRM, (struct sockaddr *) &main_addr, len);
	//After sending the results to the main server: 
	cout<<"The ServerCS finished sending the response to the Main Server.";
	return 0;
}