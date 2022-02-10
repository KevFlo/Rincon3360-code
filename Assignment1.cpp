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
    vector<string> arg4;
    int frequency;
    string binrep;

} *args;

struct decompress{
    string bincode;
    string compressed;
    int freq;
    vector<string> truncatedMessage;
    vector<string> solution;
} *decompress;

string toBinary (int value, int bitlength){
    int decimal = value;
    string binary = "";

        while (decimal != 0){
            binary += ( decimal % 2 == 0 ? "0" : "1" );
            decimal /= 2;
        }

    return binary;
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
    return largestDecimal;
}

void *binaryRepfrequencyOfsymbol (void *arguments){
    struct arg_struct *args = (struct arg_struct *)arguments;
    
    string symbol = args->arg1;
    int bitlength = args->arg3;
    cout << "i got to here" << endl;
    vector<string> trunkMsg = args->arg4;
    int value = stoi(symbol.substr(2));
    string  binaryRep = toBinary(value,bitlength);

    int freq = 0;

    for (int i = 0; i < trunkMsg.size(); i++){
        if (trunkMsg[i]==binaryRep){
            freq++;
        }
    }
    args->frequency=freq;
    args->binrep=binaryRep;

    return NULL;
}

void *decompressMsg (void *decompressArgs){

    struct decompress *decArgs= (struct decompress *)decompressArgs;
    vector<string> sol = decArgs->solution;
    string binaryRep = decArgs->bincode;
    int count = decArgs->freq;
    vector<string> trunkMsg = decArgs->truncatedMessage;

    for (int i = 0; i < trunkMsg.size(); i++) {
        break;
    }

    return 0;
}

vector<string> binaryMessagePerBitLen(string message, int bitLen){
    vector<string> truncatedMessage;

    while (message.length()>0){
        string bin = message.substr(0, bitLen);
        truncatedMessage.push_back(bin);
        message = message.substr(bitLen);
    }
return truncatedMessage;
}

int main ()
{
  int numberOfSymbolsinAlphabet, largestDecimal, numberOfbits = 0;
  std::string tempinput, symbolAndvalue, compressedMessage, symbol;
  vector<arg_struct> argVector;
  vector<string> Symbols;
  std::vector<string> decompressArgs;
  std::vector<string> output;

  std::vector<pthread_t> threads;
  

  std::cout << "Enter number of symbols in the alphabet: ";
  std::getline (std::cin,tempinput);
  numberOfSymbolsinAlphabet = stoi(tempinput);

  pthread_t th[numberOfSymbolsinAlphabet];
  threads.reserve(numberOfSymbolsinAlphabet);
  Symbols.reserve(numberOfSymbolsinAlphabet);

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

  std::cout << "Enter compressed message (sequence of bits)" << "\n";
  std::getline (std::cin,compressedMessage);
  std::cout << "Compressed message: " << compressedMessage << "\n";

  largestDecimal = findLargestDecimal(Symbols);
  numberOfbits = ceil(log2(largestDecimal));
  cout << "largestDecimal: " << largestDecimal << "\n";
  cout << "numberOfbits: " << numberOfbits << "\n";

  decompressArgs = binaryMessagePerBitLen(compressedMessage, numberOfbits);
  
    struct decompress deargs;
  
  
  

   for(int j = 0; j < Symbols.size(); j++) {
        struct arg_struct args;
        args.arg2 = compressedMessage;
        args.arg3 = numberOfbits;
        args.arg4 = decompressArgs;
        args.arg1 = Symbols[j];

        argVector.push_back(args);


      
      cout << "main() : creating thread, " << j << endl;
      if (pthread_create(&th[j], NULL, binaryRepfrequencyOfsymbol, (void *) &argVector[j])) {
         cout << "Error:unable to create thread," << endl;
         return 1;
      }

   }
   
   for(int i = 0; i < numberOfSymbolsinAlphabet; i++) {
       pthread_join(th[i], NULL);
    //here i need to save the frequency and the binary representation somehwere so i can use it later
    //Im thinking of just storing it in some struct and then  calling that information somewhere but i would need to creat new structs per thread
    //maybe use Pthread_exit(String<vector>"string binary int freq") but it only returns a number so i am not sure

       deargs.bincode=args->binrep;
       cout<< "i got to here in main joining threads" << endl;
       //output.push_back(bincode);
       deargs.freq=args->frequency;
       cout << "binary representation: " << deargs.bincode<< endl;  
       cout << "binary frequency: "<< deargs.freq << " " << endl;
       return 0;
   }


   pthread_exit(NULL);
   pthread_t ths[decompressArgs.size()];

   for (int i = 0; i < decompressArgs.size(); i++){
       cout << "main() : creating thread, " << i << endl;
        if (pthread_create(&ths[i], NULL, decompressMsg, (void *) &deargs)) {
         cout << "Error:unable to create thread," << endl;
         return 1;
      }
       
   }
   for (int j = 0; j < decompressArgs.size(); j++){
       pthread_join(th[j], NULL);

   }

//    for(int l = 0; l < compressedMessage.size(); l++) {
//        if(l%numberOfbits ==0){
//             deargs.bincode = compressedMessage.substr(l);

//             cout << "main() : creating thread, " << l << endl;
//             if (pthread_create(&th[l], NULL, binaryRepfrequencyOfsymbol, (void *) &args)) {
//                 cout << "Error:unable to create thread," << endl;
//                 return 1;
//             }
//         }

//    }

//   for (int i = 0; i < Symbols.size(); i++){
//         symbol = Symbols[i];
//         thread = thread+ to_string(i);
//         // threads.push_back( move ((binaryRepfrequencyOfsymbol(symbol ,compressedMessage))));

//     }

  
  




  return 0;
}