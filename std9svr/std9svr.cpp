#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>      //printf
#include <stdlib.h>      //atoi
#include <string.h>      //strerror()
#include <string>
#include <errno.h>      //errno
#include <winsock2.h>
#include <WS2tcpip.h>   //inet_ntop

#pragma comment(lib,"ws2_32.lib")

#define MAXPENDING         5   //Maximum outstanding connection requests
#define STRINGBUFSIZE   1024

using namespace std;
//
//// 함수를 이용, short형, int형, char형 
//int memcopy(char* buf, short val)
//{
//    short t = htons(val);
//    memcpy(buf, &t, sizeof(val));
//    return sizeof(val);
//}
//
//int memcopy(char* buf, int val)
//{
//    int t = htonl(val);
//    memcpy(buf, &t, sizeof(val));
//    return sizeof(val);
//}
//
//int memcopy(char* buf, const char* val)
//{
//    int len = strlen(val);
//    int t = htonl(len);
//    memcpy(buf, &t, sizeof(t));
//    memcpy(buf + sizeof(t), val, len);
//    return (len + sizeof(t));
//}
//
//int memcopy(char* buf, double val)
//{
//    memcpy(buf, &val, sizeof(val));
//    return sizeof(val);
//}


int main(int argc, char* argv[])
{
    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    SOCKADDR_IN serverAddress, clientAddress;

    if (argc != 2)//Test for correct number of arguments
    {
        printf("std1Svr <Server Port>\n");
        return 0;
    }

    //소켓 라이브러리 초기화
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("WSAStartup() Error!\n");
        return 0;
    }

    int servPort = atoi(argv[1]);   //First arg: local port

    //=======================================================
    //1.socket() - Create socket for incoming connections
    //=======================================================
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET)
    {
        printf("socket() failed![%d:%s]\n", errno, strerror(errno));
        return 0;
    }

    //Construct local address structure
    memset(&serverAddress, 0, sizeof(serverAddress));   //Zero out structure
    serverAddress.sin_family = AF_INET;               //IPv4 address family
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);   //Any incoming interface
    serverAddress.sin_port = htons(servPort);         //Local port

    //========================================================
    //2.bin() - Bin to the local address
    //========================================================
    if (bind(serverSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
    {
        printf("bind() failed![%d:%s]\n", errno, strerror(errno));
        closesocket(serverSocket);
        return 0;
    }

    //=========================================================
    //3.listen() - Mark the socket so it will listen for incoming connections
    //=========================================================
    if (listen(serverSocket, MAXPENDING) == SOCKET_ERROR)
    {
        printf("listen() failed![%d:%s]\n", errno, strerror(errno));
        closesocket(serverSocket);
        return 0;
    }

    char clientName[STRINGBUFSIZE];

    int clientAddressLen = sizeof(clientAddress);

    printf("stand by...\n");

    for (;;)
    {
        clientName[0] = '\0';   //setempty

        //==============================================
        //4.accept() - Wait for a client to connect --> block mode
        //==============================================
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLen);
        if (clientSocket < 0)
        {
            printf("accept() failed![%d:%s]\n", errno, strerror(errno));
            continue;
        }

        //clientSocket is connected to a client!
        if (inet_ntop(AF_INET, &clientAddress.sin_addr.s_addr, clientName, sizeof(clientName)) == NULL)
        {
            printf("Unable to get client address![%d:%s]\n", errno, strerror(errno));
            closesocket(clientSocket);
            continue;
        }

        printf("[%s:%d]", clientName, ntohs(clientAddress.sin_port));

        //**************************************************************************************
        //========================================================
        //5.recv() - Receive message from client
        //========================================================
      
        char msg[1024];                             
        int tttt = 0;      
        double ttta[3];
        recv(clientSocket, (char*)&tttt, sizeof(int), 0);
        int scount = ntohl(tttt);
        printf("%d\n", scount);
        for (int i = 0; i < scount; i++)
        {
           // sizeof(double)*3 == sizeof(ttta)
            recv(clientSocket, (char*)ttta, sizeof(double)*3, 0);
            printf("\t%.2f, %.2f, %.2f\n", ttta[0], ttta[1], ttta[2]);

        }
        

        closesocket(clientSocket);//Close client socket
    }
    WSACleanup();

    return 1;
}