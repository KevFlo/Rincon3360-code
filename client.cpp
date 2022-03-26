#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iostream>
#include <pthread.h>

int port;
std::string hostname;

struct foo {
    long val;
    std::string binary, eliasgamma;
};

void *generateCode(void *foo_ptr)
{
    struct foo *value = (struct foo *)foo_ptr;

    int sockfd;

    struct sockaddr_in serv_addr;
    struct hostent *server;

    server = gethostbyname(hostname.c_str());
    if (server == NULL)
        std::cout << "ERR - No hostname found" << std::endl;

    // open socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        std::cout << "ERR - Can't open socket" << std::endl;

    // connection data
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);

    // connect to server
    int socket = connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (socket < 0)
        std::cout << "ERR - Can't connect" << std::endl;


    // send data to socket
    char buffer[sizeof(long) + 1];
    bzero(buffer, sizeof(long) + 1);
    strcpy(buffer, std::to_string(value->val).c_str());
    int n = write(sockfd, buffer, sizeof(long));
    if (n < 0)
        std::cout << "ERR - Can't send data via socket" << std::endl;
    bzero(buffer, sizeof(long) + 1);

    int r;
    
    // read binary from socket
    r = read(sockfd, buffer, sizeof(long));
    if (r < 0)
        std::cout << "ERR - Can't read from socket" << std::endl;
    value->binary = buffer;
    bzero(buffer, sizeof(long) + 1);
    
    // read eliasgamma from socket
    r = read(sockfd, buffer, sizeof(long));
    if (r < 0)
        std::cout << "ERR - Can't read from socket" << std::endl;
    value->eliasgamma = buffer;
    bzero(buffer, sizeof(long) + 1);


    return NULL;
}

int main(int argc, char *argv[])
{
    port = atoi(argv[2]);
    hostname = argv[1];

    int THREADS;
    std::cin >> THREADS;
    // THREADS = 1;
    pthread_t tid[THREADS];
    struct foo values[THREADS];

    // get inputs
    for (int i = 0; i < THREADS; i++)
        std::cin >> values[i].val;

    for (int i = 0; i < THREADS; i++)
        pthread_create(&tid[i], NULL, generateCode, &values[i]);

    // threads
    for (int i = 0; i < THREADS; i++)
        pthread_join(tid[i], NULL);

    // outputs
    for (int i = 0; i < THREADS; i++)
    {
        std::cout << "Value: " << values[i].val;
        std::cout << ", Binary Code: " << values[i].binary;
        std::cout << ", Elias-Gamma code: " << values[i].eliasgamma;
        std::cout << std::endl;
    }

    return 0;
}