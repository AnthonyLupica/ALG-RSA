/***
   prepared for CS435 Project 1 part 2
**/

#include <string.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "sha256.h"
#include "BigIntegerLibrary.hh"

int main(int argc, char *argv[])
{
   //demonstrating how sha256 works
   std::string input = "testing";
   std::string output1 = sha256(input);
   std::cout << "sha256('" << input << "'):" << output1 << "\n";
   
   //demo bigInt works here
   BigUnsigned a = stringToBigUnsigned("124338907642982722929222542626327282");
   BigUnsigned b = stringToBigUnsigned("124338907642977775546469426263278643");
   std::cout << "big a = " << a << "\n";
   std::cout << "big b = " << b << "\n";
   std::cout << "big a*b = " << a * b << "\n";

   //Second part of your project starts here
   if (argc != 3 || (argv[1][0] != 's' && argv[1][0] != 'v')) 
   {
      std::cout << "wrong format! should be \"a.exe s filename\"";
   }
   else 
   {
      std::string filename = argv[2];
      
      // read the file
      std::streampos begin,end;
      std::ifstream myfile (filename.c_str(), std::ios::binary);

      // verify connection to file
      if (!myfile)
      {
         std::cerr << "\nError connecting to file... check that the \"file to sign\" argument is correct and ensure it is present at the same directory level as messageDigest435.cpp\n";
         exit(1);
      }

      // find size of the file in bytes
      begin = myfile.tellg();
      myfile.seekg (0, std::ios::end);
      end = myfile.tellg();
      std::streampos size = end-begin; 
      
      // read file; save in the char array memblock
      myfile.seekg (0, std::ios::beg);
      char * memblock = new char[size];
      myfile.read (memblock, size); 
      myfile.close();
      
      // std::string copyOFfile = filename+".Copy"; 
      // std::ofstream myfile2 (copyOFfile.c_str(), std::ios::binary);
      // myfile2.write (memblock, size); // write to a file
      // myfile2.close();
      
      std::cout << "file to sign/verify >>\n\n```\n" << memblock << "\n```\n\n";
      
      std::cout << "file name: \"" << filename.c_str() << "\"\n" ;
      std::cout << "size of the file: " << size << " bytes.\n";
        
      if (argv[1][0]=='s') {
         std::cout << "\n"<<"Need to sign the doc.\n";
         //.....
         
      }
      else {
         std::cout << "\n"<<"Need to verify the doc.\n";
         //.....
         
      }
      delete[] memblock;
    }
    return 0;
}