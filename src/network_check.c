#include <stdio.h>
#include <stdlib.h>
#include <ifaddrs.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <net/if.h>

int network_check()
{
    struct ifaddrs *ifaddr, *ifa;
    int connected = 0;

    // getting all the the network interfaces

    if (getifaddrs(&ifaddr) == -1)
        return 0;

    /* 
     * trying to find out a interface with a
     * IPv4 or IPv6 address 
     */

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
    {
        if (ifa->ifa_addr == NULL)
            continue;

        if ((ifa->ifa_addr->sa_family == AF_INET || ifa->ifa_addr->sa_family == AF_INET6) && !(ifa->ifa_flags & IFF_LOOPBACK))
        {
            connected = 1;
            break;
        }
    }

    // freeing the ifaddr

    freeifaddrs(ifaddr);

    return connected;
}
