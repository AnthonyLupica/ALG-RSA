// Standard libraries
#include <string>
#include <fstream>
#include <iostream>
#include <stdlib.h> 
#include <time.h>

// 'BigIntegerLibrary.hh' includes all of the library headers.
#include "BigIntegerLibrary.hh"
#include "BigIntegerAlgorithms.hh"

bool primality(const BigUnsigned &n);

// number of times looped to produce p and q
const int LOOP_COUNT = 200;

int main()
{
	/* The library throws 'const char *' error messages when things go
	 * wrong.  It's a good idea to catch them using a 'try' block like this.
	 * Your C++ compiler might need a command-line option to compile
	 * code that uses exceptions. */

   srand(time(0));

   // booleans for the two required primality tests
   bool check1 = false;
   bool check2 = false;

	try 
   {  
      /* finding p */
      std::cout << "finding p...\n";
      BigUnsigned p = BigUnsigned(0);
      int primeCandidate = 0;
     
      do
      {
         // reset checks to false
         check1 = false;
         check2 = false;

         // assign p back to 0
         p = 0;

         // in a loop, left shift by multiplying by ten, and concatenate a random digit [0, 9]
         for (int i = 0; i < LOOP_COUNT; ++i) 
         {
            p = (p * 10) + (rand() % 10);
         }
         p = (p * 10) + 7; // concatenating a 7 to increase odds that this number is prime

         ++primeCandidate;
         if (primeCandidate % 10 == 0)
         {
            std::cout << "p candidate number " << primeCandidate << std::endl;
         }

         // send this candidate off to be tested for primeness (twice to be extra sure)
         check1 = primality(p);
         
         if(check1)
         {
            check2 = primality(p);
         }
      } while (!check1 || !check2); // while either call to primality returns false
      
      std::cout << "success on candidate " << primeCandidate << "!!\n\n";

      /* finding q */
      std::cout << "finding q...\n";
      BigUnsigned q = BigUnsigned(0);
      primeCandidate = 0;
     
      do
      {
         // reset checks to false
         check1 = false;
         check2 = false;

         // assign q back to 0
         q = 0;

         // in a loop, left shift by multiplying by ten, and concatenate a random digit [0, 9]
         for (int i = 0; i < LOOP_COUNT; ++i) 
         {
            q = (q * 10) + (rand() % 10);
         }
         q = (q * 10) + 7; // concatenating a 7 to increase odds that this number is prime

         ++primeCandidate;
         if (primeCandidate % 10 == 0)
         {
            std::cout << "q candidate number " << primeCandidate << std::endl;
         }

         // send this candidate off to be tested for primeness (twice to be extra sure)
         check1 = primality(q);

         if(check1)
         {
            check2 = primality(q);
         }
      } while (!check1 || !check2); // while either call to primality returns false
      
      std::cout << "success on candidate " << primeCandidate << "!!\n\n";
      
      /* file stream for writing p and q */
      std::ofstream outFile("p_q.txt");
      
      if (!outFile)
      {
         std::cerr << "Error connecting to file for streaming of p and q, displaying to console instead\n";

         std::cout << "p: " << p << "\nq: " << q << std::endl;
      }

      else 
      {
         std::cout << "results for p and q written to \"p_q.txt\"\n";
         
         outFile << p << std::endl << q << std::endl;
      }

      // now that p and q have been discovered, we can store n
      BigInteger n = p * q;

      /* computing e and d */

      // choose an initial candidate for e to be relatively prime to (p - 1)(q - 1)
      BigInteger e = BigInteger(1);

      // compute phi 
      BigInteger phi = (p - 1) * (q - 1);

      BigUnsigned GCD;
      
      do 
      {
         e += 2;
         GCD = gcd(phi.getMagnitude(), e.getMagnitude());   
      } while (GCD != 1);

      BigUnsigned d = modinv(e, phi.getMagnitude());

      /* check that e and d are modular inverses */
      BigInteger base = BigInteger(1);

      // encrypt with e
      base = modexp(1, e.getMagnitude(), phi.getMagnitude());

      // decript with d
      base = modexp(base, d, phi.getMagnitude());
      
      // base should return to its intial state
      if (base != 1)
      {
         std::cerr << "e and d broke a test that concluded they aren't modular inverses\n";

         exit(1);
      }
	} 
   
   catch(char const* err) 
   { 
		std::cout << "The library threw an exception:\n"
			       << err << std::endl;
	}

	return 0;
}

// Input: Positive integer n
// Output: is prime? (yes/no)
bool primality(const BigUnsigned &n)
{
   // Fermat's Little Theorem states: 
   // "if n is prime, then for all a, such that 1 <= a < n, a^(n-1) == 1 mod n"

   /* generating a random integer for a */

   // LOOP_COUNT_RAND is in [1, LOOP_COUNT]
   // this ensures a is never greater than n because n was concatenated with 7 following its loop
   const int LOOP_COUNT_RAND = rand() % (LOOP_COUNT) + 1;
   
   BigInteger a = BigInteger(0);
   for (int i = 0; i < LOOP_COUNT_RAND; ++i) 
   {
      a = (a * 10) + (rand() % 10);
   } 

   // in the unlikely, but possible event that a resolved to 0, we assign 1
   if (a < 1)
   {
      a = 1;
   }

   // finally, do a check to ensure we have a valid integer that is not greater than n
   if (a >= n)
   {
      a = n - 1;
   }

   // with this valid number for a, run test for a^(n-1) == 1 mod n
   if (modexp(a, n - 1, n) == BigUnsigned(1) % n)
   {
      // passes FLT, is *very* likely prime
      return true;
   }
   else
   {
      // not prime 
      return false;
   }
}
