/***
   prepared for CS435 Project 1 part 2
**/

#include <string.h>
#include <iostream>
#include <fstream>
#include "sha256.h"
#include "BigIntegerLibrary.hh"

int main(int argc, char *argv[])
{
   // //demonstrating how sha256 works
   // std::string input = "testing";
   // std::string output1 = sha256(input);
   // std::cout << "sha256('" << input << "'):" << output1 << "\n";
   
   // //demo bigInt works here
   // BigUnsigned a = stringToBigUnsigned("124338907642982722929222542626327282");
   // BigUnsigned b = stringToBigUnsigned("124338907642977775546469426263278643");
   // std::cout << "big a = " << a << "\n";
   // std::cout << "big b = " << b << "\n";
   // std::cout << "big a*b = " << a * b << "\n";

   // Second part of your project starts here
   if (argc != 3 || (argv[1][0] != 's' && argv[1][0] != 'v')) 
   {
      std::cerr << "wrong format! should be \"./messageDigest435 [s(sign) or v(verify)] [filename]\"\n";
      exit(1);
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
      
      // // write a copy of the file
      // std::string copyOFfile = filename+".Copy"; 
      // std::ofstream myfile2 (copyOFfile.c_str(), std::ios::binary);
      // myfile2.write (memblock, size); 
      // myfile2.close();
      
      if (argv[1][0]=='s')
      {
         std::cout << "file to sign >>\n\n```\n" << memblock << "\n```\n";
      }
      else
      {
         std::cout << "file to verify >>\n\n```\n" << memblock << "\n```\n";
      }
      
      std::cout << "file name: \"" << filename.c_str() << "\"\n" ;
      std::cout << "size of the file: " << size << " bytes.\n\n";

      // filestream object for reading public/private key 
      std::ifstream public_private_in;

      // BigInteger to store n from public and private keys 
      BigInteger n = BigInteger(0);

      // temp variables for reading in values as strings 
      std::string temp_value1;
      std::string temp_value2;
      
      // document signing 
      if (argv[1][0]=='s') 
      {
         public_private_in.open("../bigInt435/d_n.txt");

         if (!public_private_in)
         {
            std::cerr << "Error connecting to file containing private key for document signing...\n"
                      << "Make sure it is reachable through \"../bigInt435/d_n.txt\"\n";
            exit(1);
         }

         BigInteger d = BigInteger(0);
         
         // read d into value 1 and n into value 2
         public_private_in >> temp_value1 >> temp_value2;

         // convert strings to BigIntegers
         d = stringToBigInteger(temp_value1);
         n = stringToBigInteger(temp_value2);


         public_private_in.close();
      }

      // document verifying 
      else 
      {
         public_private_in.open("../bigInt435/e_n.txt");

         if (!public_private_in)
         {
            std::cerr << "Error connecting to file containing public key for document verification...\n"
                      << "Make sure it is reachable through \"../bigInt435/e_n.txt\"\n";
            exit(1);
         }

         BigInteger e = BigInteger(0);

         // read e into value 1 and n into value 2
         public_private_in >> temp_value1 >> temp_value2;

         // convert strings to BigIntegers
         e = stringToBigInteger(temp_value1);
         n = stringToBigInteger(temp_value2);

         public_private_in.close();
      }
      delete[] memblock;
    }
    return 0;
}