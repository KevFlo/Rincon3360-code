#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <netdb.h>
#include <iostream>
#include <string>
#include <string.h>
#include <map>
#include <algorithm>
#include <vector>
#include <math.h>
#include <sstream>
#include <pthread.h>
#define d 10


struct foo
{   
    int port;
    std::string  pattern, TextLine_s, server;
    std::vector<std::pair<std::string, std::string>> output_string;
    struct hostent *hostname;
};

void fireman(int)
{
    while (waitpid(-1, NULL, WNOHANG) > 0){
        std::cout << "A child process ended" << std::endl;
    }    
}

std::vector<std::pair<std::string, std::string>> rabinKarp(std::string pattern, std::string TextLine, int q, std::vector<std::pair<std::string, std::string>> output_string ) {
  std::string output;
  std::stringstream tmp_stream;
  std::vector<std::pair<std::string, std::string>> output_string_f = output_string;
  int m = size(pattern);
  int n = size(TextLine);
  int i, j;
  int p = 0;
  int t = 0;
  int h = 1;

  for (i = 0; i < m - 1; i++)
    h = (h * d) % q;

  // Calculate hash value for pattern and TextLine
  for (i = 0; i < m; i++) {
    p = (d * p + pattern[i]) % q;
    t = (d * t + TextLine[i]) % q;
  }

  // Find the match
  for (i = 0; i <= n - m; i++) {
    if (p == t) {
      for (j = 0; j < m; j++) {
        if (TextLine[i + j] != pattern[j])
          break;
      }
      if (j == m){
        //makes the output string if the patter was matched
        output = ("Pattern \""+ pattern +"\" in the input TextLine at position " + std::to_string(i));
        output_string_f.push_back(make_pair(output,pattern));
      }
    }

    if (i < n - m) {
      t = (d * (t - TextLine[i] * h) + TextLine[i + m]) % q;
      if (t < 0)
        t = (t + q);
    }
  }
  // makes the output string if the pattern was not found
  if (output_string_f.size() == 0) {
    output = ("Pattern \""+ pattern +"\" not found");
    output_string_f.push_back(make_pair(output,pattern));
  }
  return output_string_f;
}



int main(int argc, char const *argv[]){
    //all the variables needed to initialize the server and the alphabet
    std::vector<foo> argVector;
    int sockfd, newsockfd, port, clilen;   
    std::string TextLine;
    std::string pattern;
    std::vector<std::pair<std::string, std::string>> outputs_S;

    std::string tempinput, compressedMessage;

    //getting the number of symbols in the inputfile
    std::getline (std::cin,tempinput);
    TextLine = tempinput;

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
            //the truncaktion of the pattern string is read here
            int r = read(newsockfd, buffer, 256);
            if (r < 0)
                std::cout << "ERR - Can't read" << std::endl;
            //saving it to struct 
            value.pattern = atol(buffer);
            bzero(buffer, 256);

            //using outmap we find the corresponding symbol to value.pattern
            value.TextLine_s = TextLine;
            value.output_string = outputs_S;
            value.output_string = rabinKarp(value.pattern, TextLine, q, value.output_string);
            argVector.push_back(value);
            //send the value to client
            // strcpy(buffer, value.output_string);
            // w = write(newsockfd,value.output_string,sizeof(value.));
            // if (w < 0)
            //     std::cout << "ERR - Can't write" << std::endl;
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