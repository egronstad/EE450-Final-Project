a. Your Full Name as given in the class list
	Emma Gronstad
b. Your Student ID
	4038445267
c. What you have done in the assignment, if you have completed the optional part (suffix). If it’s not mentioned, it will not be considered.
	I have not completed the optional part.
d. What your code files are and what each one of them does. (Please do not repeat the
project description, just name your code files and briefly mention what they do).
	client.c: prompts user for information and reports requested information back
	serverC.c: checks the credentials of the users login information
	serverCS.c: gathers course information from the CS department
	serverEE.c: gathers course information from the EE department
	serverM.c: encrypts credentials and forwards information to and from the client and the backend servers 
	(serverC.c, serverCS.c, and serverEE.cs)
e. The format of all the messages exchanged.
	in serverC.c: send event = "user error" to serverM to indicate that the authentication request has failed 
	(no username), "pass error" to serverM to indicate that the authentication request has failed (password does not 
	match) and "login" to serverM to indicate that the authentication request has passed.
g. Any idiosyncrasy of your project. It should say under what conditions the project fails, if any.
	
h. Reused Code: Did you use code from anywhere for your project? If not, say so. If so,
say what functions and where they're from. (Also identify this with a comment in the source code.)
	used code from
	https://github.com/bozkurthan/Simple-TCP-Server-Client-CPP-Example/blob/master/tcp-Server.cpp
	https://www.geeksforgeeks.org/udp-server-client-implementation-c/