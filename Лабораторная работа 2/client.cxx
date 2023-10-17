#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<signal.h>


using namespace std;


int main()
{
  int sock;
	sock = socket(AF_INET, SOCK_STREAM, 0);
	string message;


	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	addr.sin_port = htons(3425);
	
	if (connect(sock, (struct sockaddr*) &addr, sizeof(addr))<0)
	{
		perror("connect");
		exit(2);
	};
	
    while(1)
	{
		cin>>message;
		cout<<message<<endl;

		if (message == "exit") 
		{
			close(sock); 
			break;
		}

		send(sock, message.c_str(), message.length(), 0);
	}
	
	
	return 0;	
}
