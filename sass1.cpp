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

struct arg_struct {
    std::string Textline;
    std::map<std::string,int> outputs_t;
    std::string pattern;
    std::vector<std::string> outputs_S;
};

// yoinked from G4G<geeks4geeks>
std::map<std::string,int> rabinKarp(std::string pattern, std::string text, int q, std::map<std::string,int> Outputs, std::vector<std::string> output_string ) {
  std::string output;
  std::stringstream tmp_stream;
  std::map<std::string,int> outputs_f = Outputs;
  std::vector<std::string> output_string_f = output_string;
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
        
        
        output = ("Pattern \""+ pattern +"\" in the input text at position " + std::to_string(i));
        output_string_f.push_back(output);
        // std::cout << output << std::endl;
        
        // std::cout << "Pattern " << '"'<< pattern <<'"' << " in the input text at position " << i  << std::endl;
        outputs_f.insert({pattern,(i)});
      }
    }

    if (i < n - m) {
      t = (d * (t - text[i] * h) + text[i + m]) % q;

      if (t < 0)
        t = (t + q);
    }
  }
  return outputs_f;
}


void *multiRK (void *arguments){
    struct arg_struct *args = (struct arg_struct *) arguments;
    std::string pattern_t = args->pattern;
    std::string text_t = args->Textline;
    std::vector<std::string> output_string = args->outputs_S;
    std::map<std::string,int> outs = args->outputs_t;
    std::map<std::string,int> tempmap;
    int q = 13;

    
    tempmap = rabinKarp(pattern_t, text_t, q, outs , output_string );
    
    std::cout << outs.size() << "afterassignment" << std::endl;

    return NULL;
}

int main () {

    std::vector<std::string> patterns;
    std::map<std::string,int> outputs;
    std::vector<std::string> output_s;
    std::vector<arg_struct> argVector;
    
    std::string temp ="";
    std::getline(std::cin, temp);
    std::string text = temp;
    std::getline(std::cin, temp);
    int n_patterns = stoi(temp);
    int n_threads = stoi(temp);

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
        args.outputs_t = outputs;
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
   
   for(int i = 0; i < patterns.size(); i++)
    
    {
      std::cout << outputs.size() << std::endl;
        for (auto j: outputs){
          
          
          std::cout << outputs.at(patterns[i]) << std::endl;
          if (outputs.count(patterns[i]) == 0){
            std::cout << "Pattern \"" << patterns[i] << "\" not found" << std::endl;
          } else if ( j.first == patterns[i] ){
            std::cout << "Pattern \"" << patterns[i] << "\" in the input text at position " << j.second << std::endl;
          }

        }

    }

    return 0;
}
