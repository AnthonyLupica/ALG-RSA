// Standard libraries
#include <string>
#include <iostream>
#include <stdlib.h> 
#include <time.h>

// 'BigIntegerLibrary.hh' includes all of the library headers.
#include "BigIntegerLibrary.hh"

bool primality(const BigUnsigned &n);
BigUnsigned modExp(const BigUnsigned &x, const BigUnsigned &y, const BigUnsigned &m);

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
      /* FINDING p */
      std::cout << "Finding p...\n";
      BigUnsigned p = BigUnsigned(0);
     
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

         // send this candidate off to be tested for primeness (twice to be extra sure)
         check1 = primality(p);
         std::cout << check1 << std::endl;

         if(check1)
         {
            std::cout << "check 2 runs\n";
            check2 = primality(p);
         }
      } while (!check1 || !check2); // while either call to primality returns false
      
      std::cout << "my p !!!\n";
      std::cout << p << std::endl;

      /* FINDING q */
      std::cout << "Finding q...\n";
      BigUnsigned q = BigUnsigned(0);
     
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

         // send this candidate off to be tested for primeness (twice to be extra sure)
         check1 = primality(q);
         std::cout << check1 << std::endl;

         if(check1)
         {
            std::cout << "check 2 runs\n";
            check2 = primality(q);
         }
      } while (!check1 || !check2); // while either call to primality returns false
      
      std::cout << "my q !!!\n";
      std::cout << q << std::endl;
	} 
   
   catch(char const* err) 
   {
		std::cout << "The library threw an exception:\n"
			       << err << std::endl;
	}

	return 0;
}

/*
   pre: positive (large) integer n
   post: returns true if prime, and false otherwise 
*/
bool primality(const BigUnsigned &n)
{
   // Fermat's Little Theorem states: 
   // "if n is prime, then for all a, such that 1 <= a < n, a^(n-1) == 1 mod n"

   /* generating a random integer for a */

   // LOOP_COUNT_RAND is in [1, LOOP_COUNT]
   // this ensures a is never greater than n because n was concatenated with 7 following its loop
   const int LOOP_COUNT_RAND = rand() % (LOOP_COUNT) + 1;
   
   BigUnsigned a = BigUnsigned(0);
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
   if (modExp(a, n - 1, n) == BigUnsigned(1) % n)
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

/*
   pre: receive three BigUnsigned for x^y mod m
   post: modular exponention used to recursively return the result of exponentiation for very large ints  
*/
BigUnsigned modExp(const BigUnsigned &x, const BigUnsigned &y, const BigUnsigned &m)
{
   if (y == 0)
   {
      return 1;
   }
   
   BigUnsigned z = modExp(x, y / 2, m);
   
   if (y % 2 == 0)
   {
      return (z * z) % m;
   }
   else 
   {
      return (x * z * z) % m;
   }
}