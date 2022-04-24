// Client2.cpp : BT1
//

#include <stdio.h>

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>

#pragma comment(lib, "ws2_32")

int main()
{
    /*WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
    SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons(atoi(argv[2]));*/

    // connect(client, (SOCKADDR*)&addr, sizeof(addr));

    char buf[256];

    char computerName[256];
    DWORD nameLength = sizeof(computerName);
    GetComputerNameA(computerName, &nameLength);

    char driveLetters[256];
    int ret = GetLogicalDriveStringsA(sizeof(driveLetters), driveLetters);

    DWORD sectorsPerCluster, bytesPerSector, numberOfFreeClusters, totalNumberOfClusters;

    // Tao buffer de gui du lieu
    char numDrives = ret / 4;
    buf[0] = numDrives;
    int pos = 1;
    for (int i = 0; i < numDrives; i++)
    {
        // Drive letter
        buf[pos] = driveLetters[i * 4];
        pos += 1;

        // Drive size
        GetDiskFreeSpaceA(driveLetters + i * 4, &sectorsPerCluster, &bytesPerSector,
            &numberOfFreeClusters, &totalNumberOfClusters);

        float size = (float)totalNumberOfClusters * (float)sectorsPerCluster * (float)bytesPerSector / (float)(1 << 30);
        memcpy(buf + pos, &size, sizeof(size));
        pos += sizeof(size);
    }
    memcpy(buf + pos, computerName, nameLength + 1);
    pos += nameLength + 1;

}
