#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <pthread.h>

using namespace std;

struct arg_struct {
    string arg1;
    string arg2;
    int arg3;
} *args;

string toBinary (int value, int bitlength){
        int decimal = value;
        string binary = "";
        
        for (int i = bitlength; i >= 0; i--){
            int place = pow (2, i);
            if (decimal-place >= 0){
                decimal -= place;
                binary.append("1");
            }else{
                binary.append("0");
            }
        }
        return binary;
    }

int findFreq (string binary, string message){
    int M = binary.length();
    int N = message.length();
    int freq = 0;
   
    /* A loop to slide binary[] one by one */
    for (int i = 0; i <= N - M; i++)
    {
        /* For current index i, check for
           binarytern match */
        int j;
        for (j = 0; j < M; j++){
            if (message[i+j] != binary[j])
                break;
        }
  
        if (j == M) 
        {
           freq++;
        }
    }
    return freq;
}

int findLargestDecimal(vector<string> arg){
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
    //std::cout << "largestDecimal: " << largestDecimal << "\n";
    return largestDecimal;
}

void *binaryRepfrequencyOfsymbol (void *arguments){
    struct arg_struct *args = (struct arg_struct *)arguments;
    //struct arg_struct *args = (struct arg_struct *)args;
    string symbol = args->arg1;
    string message = args->arg2;
    int bitlength = args->arg3;
    int value = stoi(symbol.substr(2));
    long freq = 0;
    string  binaryRep = toBinary(value,bitlength);



    

    pthread_exit(NULL);

return 0;
}

int main ()
{
  int numberOfSymbolsinAlphabet, largestDecimal, numberOfbits = 0;
  std::string tempinput, symbolAndvalue, compfreqsedMessage, symbol, thread;
  pthread_t thread_id;
  vector<string> Symbols;
  std::vector<std::thread> threads;

  std::cout << "Enter number of symbols in the alphabet: ";
  std::getline (std::cin,tempinput);
  numberOfSymbolsinAlphabet = stoi(tempinput);
  std::cout << "Number of symbols in the alphabet: " << numberOfSymbolsinAlphabet << "\n";
  for (int i = 0; i < numberOfSymbolsinAlphabet; i++){
      std::cout << "Enter symbol " << i << " and symbols code " << "\n" << "use form of symbol value and code : a 2 " << "\n";
      std::getline(std::cin,tempinput);
      Symbols.push_back(tempinput);
  }
  cout << "\n\nSymbols: \n";
  for (int i = 0; i < Symbols.size(); i++){
        cout << Symbols[i] << "\n";
  }

  std::cout << "Enter compfreqsed message (sequence of bits)" << "\n";
  std::getline (std::cin,compfreqsedMessage);
  std::cout << "Compfreqsed message: " << compfreqsedMessage << "\n";

  largestDecimal = findLargestDecimal(Symbols);
  numberOfbits = ceil(log2(largestDecimal));
  cout << "largestDecimal: " << largestDecimal << "\n";
  cout << "numberOfbits: " << numberOfbits ;

  struct arg_struct args;
  args.arg2 = compfreqsedMessage;
  args.arg3 = numberOfbits;
  

  for(int j = 0; j < Symbols.size(); j++) {
      args.arg1 = Symbols[j];
      thread_id += j;
      cout << "main() : creating thread, " << j << endl;
      int rc;
      rc = pthread_create(&thread_id, NULL, binaryRepfrequencyOfsymbol, (void *) &args);
      
      if (rc) {
         cout << "Error:unable to create thread," << rc << endl;
         exit(-1);
      }

   }


   pthread_exit(NULL);

//   for (int i = 0; i < Symbols.size(); i++){
//         symbol = Symbols[i];
//         thread = thread+ to_string(i);
//         // threads.push_back( move ((binaryRepfrequencyOfsymbol(symbol ,compfreqsedMessage))));

//     }

  
  




  return 0;
}