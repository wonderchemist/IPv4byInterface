// Example code to get the local IP address based on the network interface (MacOS/Linux)

#include <string>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <vector>

using namespace std;

string getIPAddress(string interface)
{
    char hostname[NI_MAXHOST];
    struct ifaddrs* ifaddr;

    if (getifaddrs(&ifaddr) != -1) {
        for (struct ifaddrs* ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
            // Loop through the interfaces found
            if (ifa->ifa_addr != NULL && ifa->ifa_addr->sa_family == AF_INET) {
                // Exclude interface that do not have an address and ones tht do not match AF_INET (IPv4's)
                if (strncmp(ifa->ifa_name, interface.c_str(), interface.length()) == 0) {
                    // Exclude interfaces that do not match the interface name
                    if (getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in), hostname, sizeof(hostname), NULL, 0, NI_NUMERICHOST) == 0) {
                        // IP found and copied to hostname as NI_NUMERICHOST (numeric address)
                        freeifaddrs(ifaddr);
                        return std::string(hostname);
                    }
                }
            }
        }
    }

    freeifaddrs(ifaddr);
    return "Failed to get host address";
}

vector<string> listInterface()
{
    struct ifaddrs* ifaddr;
    vector<string> interfaceList;

    if (getifaddrs(&ifaddr) != -1) {
        for (struct ifaddrs* ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
            // Loop through the interfaces found
            if (ifa->ifa_addr != NULL && ifa->ifa_addr->sa_family == AF_INET) {
                // Exclude interface that do not have an address and ones tht do not match AF_INET (IPv4's)
                interfaceList.push_back(ifa->ifa_name);
            }
        }
    }

    freeifaddrs(ifaddr);
    return interfaceList;
}

int main(int argc, char *argv[])
{
    auto interfaces = listInterface();

    for (auto interface: interfaces) {
        printf("My Interface is %s with ",interface.c_str());
        printf("IP %s \n", getIPAddress(interface).c_str());
    }
    
    exit(EXIT_SUCCESS);
}
