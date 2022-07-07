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
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <math.h>
#include <pthread.h>


struct foo
{
    std::string pattern;
    std::string pattenr_location;
};

void fireman(int)
{
    while (waitpid(-1, NULL, WNOHANG) > 0){
    }    
}


int main(int argc, char const *argv[]){
    //all the variables needed to initialize the server and the alphabet
    int sockfd, newsockfd, port, clilen = 0;   
    std::string tempinput, pattern_s, pattern_sloc, output, textline;
    std::vector<std::string> Symbols;
    std::vector<std::string> Chars;
    std::vector<std::string> Alphabet;
    //getting the number of symbols in the inputfile
    std::getline (std::cin,tempinput);
    textline = stoi(tempinput);

    

    //make for loop to make alphabet from symbols  and tobinary
    for (int i = 0; i < Symbols.size(); i++){
        int val = stoi(Symbols[i].substr(2));
        std::string temp = toBinary(val, decimalberOfbits);
        Chars.push_back(Symbols[i].substr(0));
        Alphabet.push_back(temp);

    }
    
    

    //Now we have the map and the alphabet we can start the server
    //sending  the decimalberOfbits to client via sockets

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

    //gettign ready to send decimalber of symbols to client
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr,(socklen_t *)&clilen);
    if (newsockfd < 0)
        std::cout << "ERR - Can't accept" << std::endl;
    
    char buffer[256];
    bzero(buffer, 256);

    struct foo bitlen;

    bitlen.val = atol(buffer);
    bzero(buffer, 256);

    bitlen.val = std::to_string(decimalberOfbits);

    //send the bit length to client
    strcpy(buffer, bitlen.val.c_str());
    int n = write(newsockfd,bitlen.val.c_str(),sizeof(bitlen.val));
    if (n < 0)
        std::cout << "ERR - Can't write" << std::endl;
    bzero(buffer, 256);

    //read the data from client hopefully the compressed message bits from the THREADS
    signal(SIGCHLD, fireman);
    while (true) {

        //listen for connection
        listen(sockfd, 5);
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr,(socklen_t *)&clilen);
        pid_t p;
        
        p = fork();

        if (p == 0){
            clilen = sizeof(cli_addr);
            
            if (newsockfd < 0)
                std::cout << "ERR - Can't accept" << std::endl;

            char buffer[256];
            bzero(buffer, 256);
            int w;

            struct foo value;
            value.AlphabetBin =Alphabet;
            value.Charsmap = Chars;

            //the truncaktion of the binary string is read here
            int r = read(newsockfd, buffer, 256);
            if (r < 0)
                std::cout << "ERR - Can't read" << std::endl;
            //saving it to struct 
            value.binary = atol(buffer);
            bzero(buffer, 256);

            
            //using outmap we find the corresponding symbol to value.binary
            // value.decompressed = outmap[value.binary];

            //find value.binary in our alphabet vector
            for (int i = 0; i < Alphabet.size(); i++){
                if (value.AlphabetBin[i] == value.binary){
                    value.decompressed = value.Charsmap[i].substr(0);
                }
            } 

            
            //send the value to client
            strcpy(buffer, value.decompressed.c_str());
            w = write(newsockfd,buffer,256);
            if (w < 0)
                std::cout << "ERR - Can't write" << std::endl;
            bzero(buffer, 256);
            close(newsockfd);
            _exit(0);
        }
        else{
            std::cin.get();
        }
    }
    return 0;
}