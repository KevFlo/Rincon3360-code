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
    std::string binary, decompressed, val;
};

void fireman(int)
{
    while (waitpid(-1, NULL, WNOHANG) > 0){
        std::cout << "A child process ended" << std::endl;
    }    
}


std::string toBinary (int value, int bitlength){
    int decimal = value;
    std::string code;
    while(decimal){
        decimal%2==0?code='0'+code:code='1'+code;
        decimal/=2;
    }
    if (code.length() < bitlength){
        while( code.length() < bitlength){
            code='0'+code;
        }
    }
    return code;
}

int findLargestDecimal(std::vector<std::string> arg){
    std::string symbolValue;
    int largestDecimal = 0;
    int temp = 0;
    for (int i = 0; i < arg.size(); i++){
        symbolValue = arg[i].substr(2);
        temp = stoi(symbolValue);
        if (largestDecimal < temp) {
            largestDecimal = temp;
        }
    }
    return largestDecimal;
}

int main(int argc, char const *argv[]){
    int sockfd, newsockfd, port, clilen, decimalberOfSymbolsinAlphabet, largestDecimal, decimalberOfbits = 0;   
    std::string tempinput, symbolAndvalue, compressedMessage, output;
    std::vector<std::string> Symbols;
    std::vector<std::string> Alphabet;

    std::getline (std::cin,tempinput);
    decimalberOfSymbolsinAlphabet = stoi(tempinput);

    //get the input and populate the vector with that input 
    for (int i = 0; i < decimalberOfSymbolsinAlphabet; i++){
      std::getline(std::cin,tempinput);
      Symbols.push_back(tempinput);
    }
    
    largestDecimal = findLargestDecimal(Symbols);
    decimalberOfbits = ceil(log2(largestDecimal));
    //for every symbol in the input alphabet create the corresponding translation alphabet and map
    std::map<std::string,std::string> outmap;
    for (int k = 0; k < Symbols.size(); k++){
        std::string binaryRep = toBinary(stoi(Symbols[k].substr(2)),decimalberOfbits);
        std::string symbol = Symbols[k].substr(0);
        outmap[binaryRep] = symbol;
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
    
    char buffer[1024];
    bzero(buffer, 1024);

    struct foo bitlen;

    bitlen.val = atol(buffer);
    bzero(buffer, 1024);

    bitlen.val = std::to_string(decimalberOfbits);

    //send the bit length to client
    strcpy(buffer, bitlen.val.c_str());
    int n = write(newsockfd,bitlen.val.c_str(),sizeof(bitlen.val));
    if (n < 0)
        std::cout << "ERR - Can't write" << std::endl;
    bzero(buffer, 1024);

    //read the data from client hopefully the compressed message bits from the THREADS
    signal(SIGCHLD, fireman);
    while (true) {
        //listen for connection
        listen(sockfd, 5);
        pid_t p;
        p = fork();

        if (p == 0){
            clilen = sizeof(cli_addr);
            newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr,(socklen_t *)&clilen);
            if (newsockfd < 0)
                std::cout << "ERR - Can't accept" << std::endl;
            char buffer[1024];
            bzero(buffer, 1024);
            int w;

            struct foo value;
            //the truncaktion of the binary string is read here
            int r = read(newsockfd, buffer, 1024);
            if (r < 0)
                std::cout << "ERR - Can't read" << std::endl;
            //saving it to struct 
            value.binary = atol(buffer);
            bzero(buffer, 1024);

            //using outmap we find the corresponding symbol to value.binary
            value.decompressed = outmap[value.binary];
            //send the value to client
            strcpy(buffer, value.decompressed.c_str());
            w = write(newsockfd,value.decompressed.c_str(),sizeof(value.val));
            if (w < 0)
                std::cout << "ERR - Can't write" << std::endl;
            bzero(buffer, 1024);
            close(newsockfd);
            _exit(0);
        }
        else{
            wait(0);
        }
    }
    return 0;
}