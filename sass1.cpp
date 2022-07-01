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

// structure to pass into Pthread function
struct arg_struct {
    std::string Textline;
    std::string pattern;
    std::vector<std::pair<std::string, std::string>> outputs_S;
};

// yoinked from G4G<geeks4geeks> but the out put was modified as well as return value and arguments
std::vector<std::pair<std::string, std::string>> rabinKarp(std::string pattern, std::string text, int q, std::vector<std::pair<std::string, std::string>> output_string ) {
  std::string output;
  std::stringstream tmp_stream;
  std::vector<std::pair<std::string, std::string>> output_string_f = output_string;
  int m = size(pattern);
  int n = size(text);
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


void *multiRK (void *arguments){
    struct arg_struct *args = (struct arg_struct *) arguments;
    std::string pattern_t = args->pattern;
    std::string text_t = args->Textline;
    std::vector<std::pair<std::string, std::string>> output_string = args->outputs_S;
    std::vector<std::pair<std::string, std::string>> tempvec;
    int q = 13;
    
    args->outputs_S = rabinKarp(pattern_t, text_t, q, output_string );;

    return NULL;
}

int main () {
    std::vector<std::string> patterns;
    std::vector<std::pair<std::string, std::string>> output_s;
    std::vector<arg_struct> argVector;
    std::string temp ="";
    std::getline(std::cin, temp);
    std::string text = temp;
    std::getline(std::cin, temp);
    int n_patterns = stoi(temp);
    int n_threads = stoi(temp);

    //MAkes the patterns vector get populated with the Char patterns we want to search for
    for (int i = 0; i < n_patterns; i++)
    {
        std::getline(std::cin, temp);
        patterns.push_back(temp);
    }
// need to figure out how to send the pattern, text, and a vector to hold the output text to the thread
    for (int i = 0; i < patterns.size(); i++)
    {
        struct arg_struct args;
        std::getline(std::cin, temp);
        args.Textline = text;
        args.pattern = patterns[i];
        args.outputs_S = output_s;
        argVector.push_back(args);
    }

    pthread_t th[n_threads];
    for (int i = 0; i < n_threads; i++)
    {
       if ( pthread_create(&th[i], NULL, multiRK, (void *)&argVector[i])){
              std::cout << "Error creating thread" << std::endl;
              return 1;
       }
    }

    //Join the threads 
   for(int i = 0; i < n_threads; i++) {
       pthread_join(th[i], NULL);
   }
   
   //This reaches into the vector of structs that hold the data from the threads and tries to print out the data ass specified
  
   for(int i = 0; i < patterns.size(); i++)
    {
      std::cout << "SEARCH RESULTS:" << std::endl;
        for (std::pair<std::string, std::string> it: argVector[i].outputs_S){
          if(it.first == ("Pattern \""+ patterns[i] +"\" not found")){
              std::cout << it.first << std::endl;
          }
          else{
            std::cout << it.first << std::endl;
          }
        }
        std::cout << std::endl;
    }
    return 0;
}