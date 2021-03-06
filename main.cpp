#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <iostream>
#include <fcntl.h>
#include <string.h>
#include <vector>

using namespace std;

int sendall(int s, char *buf, int len, int flags)
{
    int total = 0;
    int n;

    while(total < len)
    {
        n = send(s, buf+total, len-total, flags);
        if(n == -1) { break; }
        total += n;
    }

    return (n==-1 ? -1 : total);
}


int recvall(int s, char *buf, int len, int flags)
{
    int total = 0;
    int n;

    while(total < len)
    {
        n = recv(s, buf+total, len-total, flags);
        if(n == -1) { break; }
        total += n;
    }

    return (n==-1 ? -1 : total);
}

int main()
{
    cout << "Server \n";

    int udp,tcp,sock;
    struct sockaddr_in addr_u, addr_t;

    int bytes_read;

    udp = socket(AF_INET, SOCK_DGRAM, 0);
    tcp = socket(AF_INET, SOCK_STREAM, 0);
    fcntl(tcp, F_SETFL, O_NONBLOCK);
    fcntl(udp, F_SETFL, O_NONBLOCK);
    if(tcp < 0)
    {
        perror("socket");
        return 0;
    }
    if(udp < 0)
    {
        perror("socket");
        return 0;
    }

    addr_u.sin_family = AF_INET;
    addr_u.sin_port = htons(3425);
    addr_u.sin_addr.s_addr = htonl(INADDR_ANY);

    addr_t.sin_family = AF_INET;
    addr_t.sin_port = htons(3450);
    addr_t.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(udp, (struct sockaddr *)&addr_u, sizeof(addr_u)) < 0)
    {
        perror("bind");
        return 0;
    }

    if(bind(tcp, (struct sockaddr *)&addr_t, sizeof(addr_t)) < 0)
    {
        perror("bind");
        return 0;
    }

    listen(tcp, 1);



    while(1)
    {
        char buf[1050];
        vector <int> nums;

        bytes_read = 0;
        bytes_read = recvall(udp, buf, 1024, 0);

        if (bytes_read > 0)
        {
          cout << buf << '\n';

          cout << bytes_read;
          buf[bytes_read] = '\n';
  //        sendto(udp,buf,1024,0, (struct sockaddr *)&addr_u, sizeof(addr_u));
        }

        sock = accept(tcp, NULL, NULL);
        if(sock > 0)
        while (1)
        {
          bytes_read = recvall(sock, buf, 1024, 0);
          if (bytes_read <= 0) break;
          cout << buf;
        }


        for (int i = 0; i < bytes_read; i++)
          if (buf[i] >= '0' && buf[i] <= '9')
            nums.push_back(buf[i]);
        if (nums.size() > 0)
        {
            cout << "Numbers are : \n";
            cout << nums.size();
            for (int i = 0; i < nums.size(); i++)
              cout << nums[i] - '0' << ' ';
        }
        nums.clear();

    }




    return 0;
}
