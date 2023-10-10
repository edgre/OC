#include <stdio.h>
#include <iostream>
#include <vector>
#include <netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<signal.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

using namespace std;

sigset_t blockedMask, origMask;
volatile sig_atomic_t wasSigHub = 0;


void sigHupHandler (int r)
{
   wasSigHub = 1;
}

void sigRegistrator ()
{
	struct sigaction sa;
	sigaction (SIGHUP, NULL, &sa);
	sa.sa_handler = sigHupHandler;
	sigaction(SIGHUP, &sa, NULL);
}

void sigBlocking ()
{

	sigemptyset (&blockedMask);
	sigaddset (&blockedMask, SIGHUP);
	sigprocmask(SIG_BLOCK, &blockedMask, NULL);
	sigemptyset (&origMask);
}

 
int main()
{
	int serversock = socket(AF_INET, SOCK_STREAM, 0);
	vector<int> clientsocks;
	
	
	char buf[1024];
	int len;
	int maxFd = 0;

    struct timespec tv;
    tv.tv_sec = 5;
    tv.tv_nsec = 0;
    
	
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(3425);
	
	bind(serversock, (struct sockaddr*) &server_addr, sizeof(server_addr));
	listen (serversock, 5);

	
	fd_set server_fds;
    FD_ZERO (&server_fds);		
	FD_SET(serversock, &server_fds);
	maxFd+=2;
	
	sigRegistrator();
	sigBlocking ();
	
		
	while(1)
		{
		int clientsock;
		fd_set client_fds = server_fds;
		int Fd = pselect(maxFd, &client_fds, NULL, NULL, &tv, &origMask);
		cout<<Fd<<endl;
		if (Fd==-1)
			{

				if (errno == EINTR) 
				{
					cout << "signal"<<endl;
						
						
				}
					
				else {cout<<"error"; break;}
			};
		
			
		if (FD_ISSET(serversock, &client_fds)) 
		{   
			cout<<'a'<<endl;
			struct sockaddr_in client_addr;
			socklen_t client_address_len = sizeof(client_addr);
			clientsock = accept(serversock, (struct sockaddr*) &client_addr, &client_address_len);
			
			if (clientsock == -1) {
					perror("accept");
					exit(EXIT_FAILURE);}
					
			cout<<"new connection"<<endl;
			maxFd++;
			if (maxFd>3) {maxFd--; close(clientsock);}
			else FD_SET(clientsock, &client_fds);
		}
		
	    if (FD_ISSET (clientsock, &client_fds))	
	    {	
			len = read(clientsock, buf, 1024);			
			if (len<=0) 
				{
					
					close (clientsock);
                    FD_CLR(clientsock, &client_fds);
                    maxFd--;
						
				};
				
				cout<<len<<endl;
						
				//for (int i=0; i<len; i++) cout<<buf[i];
				
				//FD_CLR(i, &client_fds);
		}
		//if (Fd==0) break;
		
		}
			
      return 0;
    
		
}


