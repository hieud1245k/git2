//
// Client.cpp
//
// Extremely simple, stream client example.
// Works in conjunction with Server.cpp.
//
// The program attempts to connect to the server and port
// specified on the command line. The Server program prints
// the needed information when it is started. Once connected,
// the program sends data to the server, waits for a response
// and then exits.
//
// Compile and link with wsock32.lib
//
// Pass the server name and port number on the command line. 
//
// Example: Client MyMachineName 2000
//
#include <stdio.h>
#include <winsock.h>

// Function prototype
void StreamClient(char *szServer, short nPort);

// Helper macro for displaying errors
#define PRINTERROR(s)	\
		fprintf(stderr,"\n%: %d\n", s, WSAGetLastError())

////////////////////////////////////////////////////////////

int main(int argc, char **argv)
{
	WORD wVersionRequested = MAKEWORD(1,1);
	WSADATA wsaData;
	int nRet;
	short nPort;

	//
	// Check for the host and port arguments
	//
	if (argc != 3)
	{
		fprintf(stderr,"\nSyntax: client ServerName PortNumber\n");
		return 0;
	}

	nPort = atoi(argv[2]);


	//
	// Initialize WinSock and check the version
	//
	nRet = WSAStartup(wVersionRequested, &wsaData);
	if (wsaData.wVersion != wVersionRequested)
	{	
		fprintf(stderr,"\n Wrong version\n");
		return 0;
	}


	//
	// Go do the stuff a stream client does
	//
	StreamClient(argv[1], nPort);

	
	//
	// Release WinSock
	//
	WSACleanup();
}

////////////////////////////////////////////////////////////

void StreamClient(char *szServer, short nPort)
{
	printf("\nStream Client connecting to server: %s on port: %d",
				szServer, nPort);

	//
	// Find the server
	//
    LPHOSTENT lpHostEntry;

	lpHostEntry = gethostbyname(szServer);
    if (lpHostEntry == NULL)
    {
        PRINTERROR("gethostbyname()");
        return;
    }

	//
	// Create a TCP/IP stream socket
	//
	SOCKET	theSocket;

	theSocket = socket(AF_INET,// Address family
		SOCK_STREAM,// Socket type
		IPPROTO_TCP);		// Protocol
	if (theSocket == INVALID_SOCKET)
	{
		PRINTERROR("socket()");
		return;
	}

	//
	// Fill in the address structure
	//
	SOCKADDR_IN saServer;

	saServer.sin_family = AF_INET;

	// Server's address
	saServer.sin_addr = *((LPIN_ADDR)*lpHostEntry->h_addr_list);
	saServer.sin_port = htons(nPort);	// Port number from command line

	//
	// connect to the server
	//
	int nRet;

	nRet = connect(theSocket,	// Socket
		(LPSOCKADDR)&saServer,	// Server address
		sizeof(struct sockaddr));// Length of server address structure
	if (nRet == SOCKET_ERROR)
	{
		PRINTERROR("socket()");
		closesocket(theSocket);
		return;
	}


	//
	// Send data to the server
	//
	char szBuf[256];

	strcpy(szBuf, "From the Client");
	nRet = send(theSocket,// Connected socket
		szBuf,	// Data buffer
		strlen(szBuf),	// Length of data
		0);	// Flags
	if (nRet == SOCKET_ERROR)
	{
		PRINTERROR("send()");
		closesocket(theSocket);
		return;
	}


	//
	// Wait for a reply
	//
	nRet = recv(theSocket, // Connected socket
		szBuf,	// Receive buffer
		sizeof(szBuf),	// Size of receive buffer
		0);	// Flags
	if (nRet == SOCKET_ERROR)
	{
		PRINTERROR("recv()");
		closesocket(theSocket);
		return;
	}


	//
	// Display the received data
	//

        szBuf[nRet]=0;
	printf("\nData received: %s", szBuf);


	closesocket(theSocket);
	return;
}


