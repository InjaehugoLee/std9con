//std1Console.cpp

#include <iostream>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <time.h>
#include <stdlib.h>

#pragma comment(lib,"ws2_32.lib")

int memcopy(char* buf, int val)
{
    int t = htonl(val);
    memcpy(buf, &t, sizeof(val));
    return sizeof(val);
}

int memcopy(char* buf, double val)
{
    memcpy(buf, &val, sizeof(val));
    return sizeof(val);
}

double makenumber(double s, double e)
{
    int si = (int)(s * 10000.0);
    int ei = (int)(e * 10000.0);
    int dif = si - ei;

    return (double)((rand() % dif) + si) / 10000.0;
}
int makeBuffer(char* msg)
{
    int count = rand() % 10 + 1; // 1~10;
    int offset = memcopy(msg, count);

    for (int i = 0; i < count; i++)
    {
        offset += memcopy(msg + offset, makenumber(-10.0, 10.0));
        offset += memcopy(msg + offset, makenumber(0.0, 50.0));
        offset += memcopy(msg + offset, makenumber(900.0, 1200.0));
    }
    return offset;
}

int main()
{
    int ret;
    //----------------------------------------------
    //validate

    const char* serverip = "127.0.0.1";
    short port = 5001;
    srand(time(NULL));
    //****************************************************************
    //--------------------------------------------------
    //initialize windows socket
    WSADATA wsaData = { 0 };
    ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (ret != 0)
    {
        printf("WSAStartup failed: %d\n", ret);
        return 1;
    }
    for (;;)
    {
        //-------------------------------------------------
        //socket
        int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (sock == INVALID_SOCKET)
        {
            printf("socket() failed![%d]\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }

        //--------------------------------------------------
        sockaddr_in name;
        memset(&name, 0x00, sizeof(name));
        name.sin_family = AF_INET;
        inet_pton(AF_INET, serverip, &name.sin_addr);
        name.sin_port = htons(port);
   
        //connect
        if (connect(sock, (sockaddr*)&name, sizeof(name)) == SOCKET_ERROR)
        {
            printf("socket() failed![%d]\n", WSAGetLastError());
            closesocket(sock);
            WSACleanup();
            return 1;
        }

        //************************************************************************
        char msg[1024];

        int t = makeBuffer(msg);

        //send
        if (send(sock, msg, t, 0) == SOCKET_ERROR)
        {
            printf("send() failed![%d]\n", WSAGetLastError());
        }
        else
            printf("success\n");
        closesocket(sock);
        Sleep(1000); // 1ÃÊ ½°
    }
       
    WSACleanup();
}

