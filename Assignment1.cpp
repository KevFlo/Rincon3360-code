#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <thread>

using namespace std;

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

int binaryRepfrequencyOfsymbol (string symbol, string compressedMessage ){


return 0;
}

int main ()
{
  int numberOfSymbolsinAlphabet, largestDecimal, numberOfbits = 0;
  std::string tempinput, symbolAndvalue, compressedMessage, symbol, thread;
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

  std::cout << "Enter compressed message (sequence of bits)" << "\n";
  std::getline (std::cin,compressedMessage);
  std::cout << "Compressed message: " << compressedMessage << "\n";

  largestDecimal = findLargestDecimal(Symbols);
  numberOfbits = ceil(log2(largestDecimal));
  cout << "largestDecimal: " << largestDecimal << "\n";
  cout << "numberOfbits: " << numberOfbits ;

  for (int i = 0; i < Symbols.size(); i++){
        symbol = Symbols[i];
        thread = thread+ to_string(i);
        threads.push_back( move ((binaryRepfrequencyOfsymbol(symbol ,compressedMessage))));

    }

  
  




  return 0;
}