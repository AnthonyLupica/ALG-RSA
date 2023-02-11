/***
   prepared for CS435 Project 1 part 2
**/

#include <string.h>
#include <iostream>
#include <fstream>
#include "sha256.h"
#include "BigIntegerLibrary.hh"

std::string hexString_to_decimalString(const std::string &hex_string);

int main(int argc, char *argv[])
{
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

      // generate a SHA-256 hash of the content of the file to be signed and send to 
      // hexString_to_decimalString() to be converted to a decimal string
      std::string decimal_string = hexString_to_decimalString(sha256(memblock));

      // convert decimal_string to a BigInteger to form a hash_code
      BigInteger hash_code = stringToBigInteger(decimal_string);

      std::ifstream public_private_in; // filestream object for reading public/private key 
      BigInteger n = BigInteger(0);    // BigInteger to store n from public and private keys 
      std::string temp_value1;         // temp variables for reading in values as strings 
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

// pre:  A hexidecimal string 
// post: A decimal string representation of the hex string  
std::string hexString_to_decimalString(const std::string &hex_string)
{
   std::string output = "";

   // take chunks of four characters from hex_string at a time
   for (size_t i = 0; i < hex_string.length(); i += 4) 
   {
      // store the chunk and convert the chunk to an integer using stoi and the base 16 specifier
      std::string hex_chunk = hex_string.substr(i, 4);
      int decimal_chunk = std::stoi(hex_chunk, nullptr, 16);

      // then convert the decimal_chunk to a string to form a decimal representation as a string 
      output += std::to_string(decimal_chunk);
   }
   
   return output;
}