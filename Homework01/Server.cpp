// Server1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>

#pragma comment(lib, "ws2_32")

int main(int argc, char* argv[])
{
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(atoi(argv[1]));

    bind(listener, (SOCKADDR*)&addr, sizeof(addr));
    listen(listener, 5);

    SOCKET client = accept(listener, NULL, NULL);

    char buf[256];
    int ret;

    // Doc xau chao tu file
    FILE* f = fopen(argv[2], "r");
    fgets(buf, sizeof(buf), f);
    fclose(f);

    // Gui sang client
    send(client, buf, strlen(buf), 0);

    // Nhan du lieu tu client va ghi vao file
    f = fopen(argv[3], "wb");
    while (1)
    {
        ret = recv(client, buf, sizeof(buf), 0);
        if (ret <= 0)
            break;
        fwrite(buf, 1, ret, f);
    }
    fclose(f);

    closesocket(client);
    closesocket(listener);
    WSACleanup();
}