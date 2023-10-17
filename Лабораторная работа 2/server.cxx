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
volatile sig_atomic_t wasSigHup = 0;


void sigHupHandler (int r)
{
   wasSigHup = 1;
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
	int client, acception;
	vector<int> clients;
	
	
	char buf[1024];
	int len;
	int maxFd, num=1;

    struct timespec tv;
    tv.tv_sec =30;
    tv.tv_nsec = 0;
    
	
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(3425);
	
	bind(serversock, (struct sockaddr*) &server_addr, sizeof(server_addr));
	listen (serversock, 5);

	
	sigRegistrator();
	sigBlocking ();
	
		
	while(1)
	{
			
		fd_set server_fds;

        FD_ZERO (&server_fds);		
		FD_SET(serversock, &server_fds);

		if (maxFd<serversock) maxFd = serversock + 1; 

		if (num==2) 
		{
			FD_SET (client, &server_fds);
		    if (maxFd<client) {maxFd = client + 1;}
		}
		
		int Fd = pselect(maxFd, &server_fds, NULL, NULL, NULL, &origMask);
		
		if (Fd==-1)
		{

				if (errno == EINTR) 
				{
					if (wasSigHup==1) 
					{
						cout << "signal"<< endl;
						close(acception);
						close(client);
						close(serversock);
		            }
					 	
				}
					
				else {cout<<"error"; exit(2);}
		};
			
		
		
		if (FD_ISSET(serversock, &server_fds)) 
		{   
			struct sockaddr_in client_addr;
			socklen_t client_address_len = sizeof(client_addr);
			acception = accept(serversock, (struct sockaddr*) &client_addr, &client_address_len);
			
			if (acception== -1)
			{
				perror("accept");
				exit(EXIT_FAILURE);
			}	

			cout<<"new connection"<<endl;
			num++;
			
			
			if (num<=2)
			{
				client = acception;
		    }

		    else {num--; close(acception);}
		    
		}
		
	    if (FD_ISSET (client, &server_fds))	
	    {	
			len = read(client, buf, 1024);			
			if (len<=0) 
			{
					close (client);
                    FD_CLR(client, &server_fds);
                    clients.pop_back();
                    num--;
			};
				
			cout<<len<<endl;
						
			
		}
		
	}
		
	close (acception);
	close (client);
	close (serversock);
    return 0;
    
		
}


