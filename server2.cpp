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

std::string toBinary (int value, int bitlength){
    int decimal = value;
    std::string binary = "";
        while (decimal != 0){
            binary = ( decimal % 2 == 0 ? "0" : "1" )+binary;
            decimal /= 2;
        }
        while(binary.length()!=bitlength)
            binary = "0" + binary;
    return binary;
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
    int sockfd, newsockfd, port, clilen, numberOfSymbolsinAlphabet, largestDecimal, numberOfbits = 0;   
    std::string tempinput, symbolAndvalue, compressedMessage, output;
    std::vector<std::string> Symbols;
    std::vector<std::string> Alphabet;

    std::getline (std::cin,tempinput);
    numberOfSymbolsinAlphabet = stoi(tempinput);

      //get the input and populate the vector with that input 
    for (int i = 0; i < numberOfSymbolsinAlphabet; i++){
      std::getline(std::cin,tempinput);
      Symbols.push_back(tempinput);
    }

    largestDecimal = findLargestDecimal(Symbols);
    numberOfbits = ceil(log2(largestDecimal));


  // Compressed message comes here

    //send the numberOfbits to client via sockets

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
    

    // bitlen.val = numberOfbits;

    //gettign ready to send number of symbols to client
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr,(socklen_t *)&clilen);
    if (newsockfd < 0)
        std::cout << "ERR - Can't accept" << std::endl;
    
    char buffer[sizeof(numberOfbits) + 1];
    bzero(buffer, sizeof(numberOfbits) + 1);

    struct foo bitlen;

    bitlen.val = atol(buffer);
    bzero(buffer, sizeof(numberOfbits) + 1);

    bitlen.val = std::to_string(numberOfbits);



    strcpy(buffer, bitlen.val.c_str());
    int n = write(newsockfd,bitlen.val.c_str(),sizeof(bitlen.val));
}