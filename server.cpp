#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <netdb.h>
#include <iostream>

struct foo
{
    long val;
    std::string binary, decompressed;
};

void fireman(int)
{
    while (waitpid(-1, NULL, WNOHANG) > 0){
        std::cout << "A child process ended" << std::endl;
    }
    
}

int main(int argc, char *argv[])
{
    int numberOfSymbolsinAlphabet, largestDecimal, numberOfbits = 0;
    std::string tempinput, symbolAndvalue;

    int sockfd, newsockfd, port, clilen;
    int numberofSymbols = atoi(argv[2]); // from file input 


    struct sockaddr_in serv_addr, cli_addr;

    // open socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        std::cout << "ERR - Can't open socket" << std::endl;

    // connection data
    bzero((char *)&serv_addr, sizeof(serv_addr));
    port = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    // binding
    int b = bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (b < 0)
        std::cout << "ERR - Can't bind" << std::endl;
    
    //gettign ready to send number of symbols to client
    
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, (socklen_t *)&clilen);
    if (newsockfd < 0)
        std::cout << "ERR - Can't accept" << std::endl;
    int n = send(sockfd, &numberofSymbols, sizeof(numberofSymbols), 0);



    // read data
    signal(SIGCHLD, fireman); 
    while (true)
    {
        // listen
        listen(sockfd, 5);
        pid_t p;
        p = fork();

        if (p == 0)
        {
            clilen = sizeof(cli_addr);
            newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, (socklen_t *)&clilen);
            if (newsockfd < 0)
                std::cout << "ERR - Can't accept" << std::endl;

            char buffer[sizeof(long) + 1];
            bzero(buffer, sizeof(long) + 1);
            int w;

            struct foo value;

            int r = read(newsockfd, buffer, sizeof(long));
            if (r < 0)
                std::cout << "ERR - Can't read from socket" << std::endl;

            value.val = atol(buffer);
            bzero(buffer, sizeof(long) + 1);

            // binary conversion
            long temp = value.val;
            while (temp > 0)
            {
                value.binary = std::to_string(temp % 2) + value.binary;
                temp /= 2;
            }
            strcpy(buffer, value.binary.c_str());
            w = write(newsockfd, value.binary.c_str(), sizeof(value.binary));
            if (w < 0)
                std::cout << "ERR - Can't send data via socket" << std::endl;
            bzero(buffer, sizeof(long));

            // elias gamma
            int counter = 0;
            while (counter < strlen(value.binary.c_str()) - 1)
            {
                value.decompressed = "0" + value.decompressed;
                counter++;
            }
            value.decompressed;
            strcpy(buffer, value.decompressed.c_str());
            w = write(newsockfd, buffer, sizeof(long));
            if (w < 0)
                std::cout << "ERR - Can't send data via socket" << std::endl;
            bzero(buffer, sizeof(long));
            
            _exit(0);
        }
        else
        {
            wait(0);
        }
    }

    return 0;
}