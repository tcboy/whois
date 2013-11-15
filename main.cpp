#include <netinet/in.h>		// for sockaddr_in
#include <sys/types.h>		// for socket
#include <sys/socket.h>		// for socket
#include <arpa/inet.h>
#include <unistd.h> 
#include <netdb.h>
#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>

#define	PORT 43
#define BUFSIZE 1024

using namespace std;

static map<string, string> Server;

void init()
{
	Server[".com"]		=	"whois.internic.net";
	Server[".net"]		=	"whois.internic.net";
	Server[".org"]		=	"whois.internic.net";
	Server[".biz"]		=	"whois.internic.net";
	Server[".info"]		=	"whois.internic.net";
	Server[".name"]		=	"whois.internic.net";
	Server[".cn"]		=	"whois.cnnic.net.cn";
	Server[".com.cn"]	=	"whois.cnnic.net.cn";
	Server[".org.cn"]	=	"whois.cnnic.net.cn";
	Server[".net.cn"]	=	"whois.cnnic.net.cn";
	Server[".edu.cn"]	=	"whois.edu.cn";

}

string GetIP(string domain)
{
	struct hostent *hptr;
	struct in_addr in;
	struct sockaddr_in local_addr;

	if( (hptr = gethostbyname(domain.c_str()) ) == NULL )
		return "error";

	memcpy(&local_addr.sin_addr.s_addr, hptr->h_addr,4);
	in.s_addr = local_addr.sin_addr.s_addr;

	return inet_ntoa(in);
}

string GetWhois(string domain, string prev)
{
	int bytes;
	char buf[BUFSIZE];
	string Dest = GetIP(Server[prev]);
	string result = "";

	struct sockaddr_in addr;
	
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = inet_addr(Dest.c_str());

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if ( sockfd == -1 )
	{
		return "Create Socket Failed!\n";
	}

	if(connect(sockfd,(struct sockaddr *)(&addr), sizeof(struct sockaddr)) == -1)
	{
		return "Connect Failed!\n";
	}
	sprintf(buf, "%s\r\n", domain.c_str() );
	if ( write(sockfd, buf, strlen(buf)) == -1)
	{
		return "Write Data Failed!\n";
	}
	

	while ( bytes = read(sockfd, buf, BUFSIZE - 1) )
	{
		buf[bytes] = '\0';
		result += buf;
	}
	close(sockfd);
	return result;
}


int main()
{
	string line;
	init();
	while ( 1 )
	{
		cin >> line;
		if ( line == "exit" )
			break;
		
		int index = line.find_first_of(".");
		if ( index != -1 )
		{
			string prev = line.substr(index, line.size() - index + 1);
			cout << GetWhois(line, prev) << endl;
		}
		else
		{
			cout << "Domain Name Wrong!" << endl;
		}
	}
	return 0;
}
