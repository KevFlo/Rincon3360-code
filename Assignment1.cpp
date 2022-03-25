#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <math.h>
#include <pthread.h>

using namespace std;
//This is a struct that will hold the initial information for the first thread function that
struct arg_struct {
    /*** symbols will be the string input
     *  The bit length is the largest amount of bits needed to represent the largest decimal in input
     *  TruncatedMSg will be a vector that holds the binary message but truncated byt the bit length and saved in its segmented form
     *  frequency will be the frequency of the binary representation of the decimal from the input vector
     *  binrep will be the binary representation of the decimal values from the input
    ***/
    string symbols;
    int bitLength;
    vector<string> truncatedMsg;
    int frequency;
    string binrep;
};

// output struct will contain the information for the decompression function
struct output_{
    /*
    * Output is a map with the key being the binary representation and the value being the character it is matched to from the input string
    * Sol is the string returned from the map ex : "a"
    * symbol will be the binary to be decompressed and matched to the mapping
    */
    vector< tuple <char, string> > Alphabet;
    map<string,string> outputmap;
    string sol;
    string symbol;
};

// To binary just converts decimal number to binary based on the bit length
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
//findLargestDecimal will find the largest decimal from the input by passing in the vector containing the
// inputs and checking the [2] index i.e. where the decimal values are found
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
/*** 
 * Will take in a struct with the information populated prior to thread creation and
 * each thread will take a symbol from the symbols vector and find its binary representation
 * and frequency in the binary compressedMessage. 
 * ***/
void *binaryRepfrequencyOfsymbol (void *arguments){
    //the struct is being pointed to
    struct arg_struct *args = (struct arg_struct *)arguments;
    // pointing to data in the structure so the thread can access it from memory and do work 
    string symbol = args->symbols;
    int bitlength = args->bitLength;
    vector<string> trunkMsg = args->truncatedMsg;
    int value = stoi(symbol.substr(2));
    //get the binary representation of the decimal for this specific symbol
    // cout << symbol << " symbol"  << endl;
    // cout << value << "value of the symbol " << symbol << '\n';
    string  binaryRep = toBinary(value,bitlength);
    args->binrep=binaryRep;
    int freq = 0;
    //Look for the frequency of this binary represantation in the truncated representation of the binary message
    for (int i = 0; i < trunkMsg.size(); i++){
        if (trunkMsg[i]==binaryRep){
            freq++;
        }
    }
    //each struct in the vector will contain the relevant information for that input 
    // each thread points to a specific struct in the vecotr and fills its data in by pointing to it here
    args->frequency=freq;
    // cout << binaryRep << "binaryRep" << endl;
    
    return NULL;
}

void *decompressMsg (void *Args){
    struct output_ *args = (struct output_*)Args;
    vector< tuple <char, string> > pseudoAlphabet = args->Alphabet;
    map<string,string> hehe = args->outputmap;
    string symboltemp = args->symbol;
    string sol = args->sol;
//save the value of the binarry rep to the sol string and save it back to the struct
    sol = args->outputmap[symboltemp];
    for (int alphaIndex = 0 ; alphaIndex < pseudoAlphabet.size(); alphaIndex++){
        std::string tempstr,tempOstr;
        tempstr = get<0>(pseudoAlphabet[alphaIndex]);
        tempOstr = get<1>(pseudoAlphabet[alphaIndex]);
        if ( (symboltemp) == (tempstr) ){
            sol = sol + tempOstr;
        }
    }

    args->sol = sol;

    return NULL;
}

// This function will truncate the binary message based on the bit length to get the correct vecotr representaiton of the message
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
    //Initiallize the variables to hold the data to be later used in struct creation and pthread creation
  int numberOfSymbolsinAlphabet, largestDecimal, numberOfbits = 0;
  std::string tempinput, symbolAndvalue, compressedMessage, output;
  vector<arg_struct> argVector;
  vector<output_> OutputV;
  vector<string> Symbols;
  vector< tuple <char, string> > Alphabet;
  std::vector<string> decompressArgs;

  std::getline (std::cin,tempinput);
  numberOfSymbolsinAlphabet = stoi(tempinput);
// create the number of threads based on number of symbols in the input for the inital threaded function
  pthread_t th[numberOfSymbolsinAlphabet];

  Symbols.reserve(numberOfSymbolsinAlphabet);

    //get the input and populate the vector with that input 
  for (int i = 0; i < numberOfSymbolsinAlphabet; i++){

      std::getline(std::cin,tempinput);
      Symbols.push_back(tempinput);
  }

  std::getline (std::cin,compressedMessage);
  //finds the largest decimal
  largestDecimal = findLargestDecimal(Symbols);
  //gets the largest bit representation
  numberOfbits = ceil(log2(largestDecimal));
    //truncates the message based on the largest bit representation
  decompressArgs = binaryMessagePerBitLen(compressedMessage, numberOfbits);
  //creates the map for the binary representation and the corresponding char
  map<string,string> outmap;
  
  for (int x =0; x < numberOfSymbolsinAlphabet; x++){
      char tempC;
      string tempV = "";
      tempC= Symbols[x][0];
      tempV= (toBinary(stoi(Symbols[x].substr(2)),numberOfbits));
      Alphabet.push_back(make_tuple(tempC,tempV));
    
      //map key = binary representation value is the character 
    //   outmap.insert(make_pair(tempK, tempC));
  }  
  
    //creates the struct for each symbol 'a 2',..., and pushes back the data to the vector of structs 
  for (int k = 0; k < Symbols.size(); k++){
        struct arg_struct args;
        args.bitLength = numberOfbits;
        args.truncatedMsg = decompressArgs;
        args.symbols = Symbols[k];
        
        int val = stoi(args.symbols.substr(2));
        string  binaryRep = toBinary(val,args.bitLength);
        args.binrep = binaryRep;
        argVector.push_back(args);
    }
    //will save the memory required based on how big the vector needs to be based on the amount of decompressed arg sturctrs
//   OutputV.reserve(decompressArgs.size());
  for (int a = 0; a < decompressArgs.size(); a++){
      //the oputput struct is created and populated for each part of the truncated message
        struct output_ outPut;
        outPut.sol = "";
        outPut.outputmap = outmap;
        outPut.symbol = "";
        OutputV.push_back(outPut);
    }
  //Here the first thread function is created and so are the number of threads required 
   for(int j = 0; j < Symbols.size(); j++) {

      if (pthread_create(&th[j], NULL, binaryRepfrequencyOfsymbol, (void *) &argVector[j])) {
         cout << "Error:unable to create thread," << endl;
         return 1;
      }
   }
   //Join the threads 
   for(int i = 0; i < numberOfSymbolsinAlphabet; i++) {
       pthread_join(th[i], NULL);
   }
    //create thread for each part of the vector for the truncated message 
   pthread_t ths[decompressArgs.size()];
    //second threaded function gets the struct based on its index for the vector of output struct 
   for (int i = 0; i < decompressArgs.size(); i++){

       OutputV[i].symbol=decompressArgs[i];
    //    cout << OutputV[i] << "ouptut Vector" << endl;

       //should take output struct which contains binary rep and the map will return the char to the struct
        if (pthread_create(&ths[i], NULL, decompressMsg, (void *) &OutputV[i])) {
         cout << "Error:unable to create thread," << endl;
         return 1;
      }   
   }
   //join the threads
   for (int j = 0; j < decompressArgs.size(); j++){
       pthread_join(th[j], NULL);
   }
   // output the alphabet as per requirements
    cout << "Alphabet: " << endl;
    for (int i = 0; i < argVector.size(); i++){
        
        cout << "Character: " << argVector[i].symbols[0] << ", " << "Code: "<< argVector[i].binrep << ", " << "Frequency: " << argVector[i].frequency << "\n";
    }
    //iterate through the output vector of structs and get the string from sol
    for(int f = 0;f < OutputV.size(); f++){
        // cout << "output for struct: " << f <<  OutputV[f].sol<< "\n";
        output = output+OutputV[f].sol;
    }
    cout << "Decompressed message: " << output << endl;

  return 0;
}

