

#include <winsock2.h>
#include <stdio.h>
#pragma comment(lib, "ws2_32")

int numClient = 0;
SOCKET clients[64];
char buf[256];

DWORD WINAPI ClThr(LPVOID args)
{
    int numm;
    char *text, *pop;
    char user[1024], pass[1024];

    SOCKET client = *(SOCKET*)args;

    do {
        pop = (char*)"\nUser: ";
        send(client, pop, strlen(pop), 0);
        numm = recv(client, buf, sizeof(buf), 0);
        if (numm <= 0)
            break;
        buf[numm] = 0;
        sscanf_s(buf, "%s", user, sizeof(user));

        
        pop = (char*)"Password: ";
        send(client, pop, strlen(pop), 0);
        numm = recv(client, buf, sizeof(buf), 0);
        if (numm <= 0)
            break;
        buf[numm] = 0;
        sscanf_s(buf, "%s", pass, sizeof(pass));


        int matched = 1;

        FILE* f; 
        fopen_s(&f, "Acc.txt", "rt");
        while (!feof(f))
        {
            char line[1024], _user[1024], _pass[1024];
            fgets(line, sizeof(line), f);
            if(line[strlen(line) - 1] == '\n') line[strlen(line) - 1] = 0;
            sscanf_s(line, "%s %s", _user, sizeof(_user), _pass, sizeof(_pass));
            
            if (strcmp(user, _user) != 0) matched = 1;
            else
            {
                if (strcmp(pass, _pass) != 0) matched = 2;
                else matched = 3;
                break;
            }
        }
        fclose(f);
        
        if(matched == 1) text = (char*)"\nWrong user!\n";
        else if (matched == 2) text = (char*)"\nWrong pass!\n";
        else
        {
            text = (char*)"\nSign in successfull!\n";
            clients[numClient] = client;
            numClient++;
        }

        send(client, text, strlen(text), 0);

    } while (strcmp(text, "\nSign in successfull !\n") != 0);

    while (1)
    {
        pop = (char*)"\nCommand: ";
        send(client, pop, strlen(pop), 0);

        numm = recv(client, buf, sizeof(buf), 0);
        if (numm <= 0)
            break;
        buf[numm] = 0;

        printf("%d: %s\n", client, buf);

        
        if (buf[strlen(buf) - 1] == '\n') buf[strlen(buf) - 1] = 0;

       
        sprintf_s(buf + strlen(buf), sizeof(buf), "%s", ">out_put.txt");
        system(buf);

        
        FILE* f;
        fopen_s(&f, "output.txt", "rb");
        fseek(f, 0, SEEK_END);  
        int flen = ftell(f);    
        fseek(f, 0, SEEK_SET);  
        char* fdata = (char*)calloc(flen, 1);
        fread(fdata, 1, flen, f);
        fclose(f);

        pop = (char*)"\nRESULT\n";
        send(client, pop, strlen(pop), 0);
        send(client, fdata, flen, 0);
        free(fdata);
    }

    closesocket(client);
}

    int main()
    {

        WSADATA wsa;
        WSAStartup(MAKEWORD(2, 2), &wsa);


        SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);


        SOCKADDR_IN addr;
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
        addr.sin_port = htons(8000);

        bind(listener, (SOCKADDR*)&addr, sizeof(addr));

        listen(listener, 5);

        while (1)
        {
            SOCKET client = accept(listener, NULL, NULL);
            printf("Client connect: %d\n", client);
            CreateThread(0, 0, ClThr, &client, 0, 0);
        }
    }
