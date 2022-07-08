#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sstream>
#include <algorithm>
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
#define d 10


struct foo
{
    std::string pattern;
    std::string pattenr_location;
    std::string found;
    std::string notfound;
};

void fireman(int)
{
    while (waitpid(-1, NULL, WNOHANG) > 0){
    }    
}

// yoinked from G4G<geeks4geeks> but the out put was modified as well as return value and arguments
std::vector<std::pair<std::string, std::string>> rabinKarp(std::string pattern, std::string text, int q, std::vector<std::pair<std::string, std::string>> output_string ) {
  std::string output;
  std::vector<std::pair<std::string, std::string>> output_string_f = output_string;
  int m = pattern.size();
  int n = text.size();
  int i, j;
  int p = 0;
  int t = 0;
  int h = 1;

  for (i = 0; i < m - 1; i++)
    h = (h * d) % q;

  // Calculate hash value for pattern and text
  for (i = 0; i < m; i++) {
    p = (d * p + pattern[i]) % q;
    t = (d * t + text[i]) % q;
  }

  // Find the match
  for (i = 0; i <= n - m; i++) {
    if (p == t) {
      for (j = 0; j < m; j++) {
        if (text[i + j] != pattern[j])
          break;
      }
      if (j == m){
        //makes the output string if the patter was matched
        output = ("Pattern \""+ pattern +"\" in the input text at position " + std::to_string(i));
        output_string_f.push_back(make_pair(output,pattern));
      }
    }

    if (i < n - m) {
      t = (d * (t - text[i] * h) + text[i + m]) % q;
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
    int sockfd, newsockfd, port, clilen = 0;   
    std::string tempinput, pattern_s, pattern_sloc, output, textline;
    std::vector<std::pair<std::string, std::string>> output_string ;
    std::getline (std::cin,tempinput);
    textline = tempinput;

    struct sockaddr_in serv_addr, cli_addr;

    char buffer[256];
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


    clilen = sizeof(cli_addr);

    //read the data from client hopefully the pattern message bits from the THREADS
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


            bzero(buffer, 256);
            int n = read(newsockfd, buffer, 255);
            if (n < 0)
                std::cout << "ERROR reading from socket" << std::endl;

            std::string temp = buffer;

            std::vector<std::pair<std::string, std::string>> output_string;
            //find patterns using rabin karp
            output_string = rabinKarp(temp, textline, d, output_string);

             for (std::pair<std::string, std::string> it: output_string) {
                std::vector<std::string> found;
                std::vector<std::string> notfound;
                if(it.first == ("Pattern \""+ pattern_s +"\" not found")){
                    notfound.push_back(it.first);
                    for(int j = 0 ; j < notfound.size(); j++){
                        strcpy(buffer, notfound[j].c_str());
                        int w = write(newsockfd, buffer, strlen(buffer));
                        if (w < 0)
                            std::cout << "ERROR writing to socket not foun" << std::endl;
                    }
                    // // value.notfound += it.first;
                    // strcpy(buffer, it.first.c_str());
                    // int w = write(newsockfd, buffer, strlen(buffer));
                    // if (w < 0){
                    //     std::cout << "ERR - Can't write" << std::endl;
                    // }

                    // std::cout << it.first << std::endl;
                }
                else{
                    found.push_back(it.first);
                    for(int j = 0 ; j < found.size(); j++){
                        strcpy(buffer, found[j].c_str());
                        int w = write(newsockfd, buffer, strlen(buffer));
                        if (w < 0)
                            std::cout << "ERROR writing to socket found" << std::endl;
                    }




                    // value.found += it.first;
                    // strcpy(buffer, it.first.c_str());
                    // int w = write(newsockfd, buffer, strlen(buffer));
                    // if (w < 0){
                    //     std::cout << "ERR - Can't write" << std::endl;
                    // }
                    // std::cout << it.first << std::endl;

                }

            }
            
            // //send the found strings to client
            // strcpy(buffer, value.found.c_str());
            // w = write(newsockfd,buffer,256);
            // if (w < 0)
            //     std::cout << "ERR - Can't write" << std::endl;
            // bzero(buffer, 256);

            // //send the not found strings to client
            // strcpy(buffer, value.notfound.c_str());
            // w = write(newsockfd,buffer,256);
            // if (w < 0)
            //     std::cout << "ERR - Can't write" << std::endl;


            close(newsockfd);
            _exit(0);
        }
        else{
            std::cin.get();
        }
    }
    return 0;
}