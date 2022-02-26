#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <math.h>
#include <pthread.h>

using namespace std;

struct arg_struct {
    string symbols;
    string compressedbinaryMsg;
    int bitLength;
    vector<string> truncatedMsg;
    int frequency;
    string binrep;
    string sol;
    vector<string>output;
};
struct output_{
    map<string,string> output;
    string sol;
    string symbol;
};

// binary is fine 
string toBinary (int value, int bitlength){
    int decimal = value;
    string binary = "";
        while (decimal != 0){
            binary = ( decimal % 2 == 0 ? "0" : "1" )+binary;
            decimal /= 2;
        }
        while(binary.length()!=bitlength)
            binary = "0" + binary;
    return binary;
}
//this works dont touch
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
    string symbol = args->symbols;
    int bitlength = args->bitLength;
    vector<string> trunkMsg = args->truncatedMsg;
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

void *decompressMsg (void *Args){
    struct output_ *args = (struct output_*)Args;
    map<string,string> hehe = args->output;
    string symbol = args->symbol;
    string sol = args->sol;

    // sol = hehe[symbol];
    sol = args->output[symbol];

    args->sol = sol;

    return NULL;
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
  std::string tempinput, symbolAndvalue, compressedMessage, output;
  vector<arg_struct> argVector;
  vector<output_> Output;
  vector<string> Symbols;
  std::vector<string> decompressArgs;

  std::getline (std::cin,tempinput);
  numberOfSymbolsinAlphabet = stoi(tempinput);

  pthread_t th[numberOfSymbolsinAlphabet];

  Symbols.reserve(numberOfSymbolsinAlphabet);

//create the vector of sctructs here so that its already populated with data and you dont need to create at the pthread_create part 
    
  for (int i = 0; i < numberOfSymbolsinAlphabet; i++){

      std::getline(std::cin,tempinput);
      Symbols.push_back(tempinput);
  }

  std::getline (std::cin,compressedMessage);
  
  largestDecimal = findLargestDecimal(Symbols);
  numberOfbits = ceil(log2(largestDecimal));
 
  decompressArgs = binaryMessagePerBitLen(compressedMessage, numberOfbits);
  
  map<string,string> outmap;
  
  for (int x =0; x < numberOfSymbolsinAlphabet; x++){
      string tempC = "";
      string tempK = "";
      tempC= Symbols[x][0];
      tempK= (toBinary(stoi(Symbols[x].substr(2)),numberOfbits));
      outmap.insert(std::pair<std::string,std::string>(tempK, tempC));
  }  
  
    //creates the struct for each symbol 'a 2',..., and pushes back the data to the vector of structs 
  for (int k = 0; k < Symbols.size(); k++){
        struct arg_struct args;
        args.compressedbinaryMsg = compressedMessage;
        args.bitLength = numberOfbits;
        args.truncatedMsg = decompressArgs;
        args.symbols = Symbols[k];
        args.sol = "";
        
        int val = stoi(args.symbols.substr(2));
        string  binaryRep = toBinary(val,args.bitLength);
        argVector.push_back(args);
    }
  Output.reserve(decompressArgs.size());
  for (int a = 0; a < decompressArgs.size(); a++){
        struct output_ outPut;
        outPut.sol = "";
        outPut.output = outmap;
        Output.push_back(outPut);
    }
  
   for(int j = 0; j < Symbols.size(); j++) {
      if (pthread_create(&th[j], NULL, binaryRepfrequencyOfsymbol, (void *) &argVector[j])) {
         cout << "Error:unable to create thread," << endl;
         return 1;
      }
   }
   
   for(int i = 0; i < numberOfSymbolsinAlphabet; i++) {
       pthread_join(th[i], NULL);
   }

   pthread_t ths[decompressArgs.size()];

   for (int i = 0; i < decompressArgs.size(); i++){

       Output[i].symbol=decompressArgs[i];
        if (pthread_create(&ths[i], NULL, decompressMsg, (void *) &Output[i])) {
         cout << "Error:unable to create thread," << endl;
         return 1;
      }   
   }
   for (int j = 0; j < decompressArgs.size(); j++){
       pthread_join(th[j], NULL);
   }
    cout << "Alphabet: " << endl;
    for (int i = 0; i < argVector.size(); i++){
        
        cout << "Character: " << argVector[i].symbols[0] << ", " << "Code: "<< argVector[i].binrep << ", " << "Frequency: " << argVector[i].frequency << "\n";
    }
    cout << Output.size() << endl;
    for(int f = 0;f < Output.size(); f++){
        output = output+Output[f].sol;
    }
    cout << "Decompressed message: " << output << endl;

  return 0;
}

