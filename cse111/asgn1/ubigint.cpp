// $Id: ubigint.cpp,v 1.12 2020-10-19 13:14:59-07 - - $

#include <cctype>
#include <cstdlib>
#include <exception>
#include <stack>
#include <stdexcept>
#include<string>
using namespace std;

#include "debug.h"
#include "relops.h"
#include "ubigint.h"

ubigint::ubigint (unsigned long that) {
   if (that < 10)	{
      ubigvalue.push_back(that);
   }else	{
      uint8_t digit = 0;
      while(that != 0)	{
			   digit = that % 10;
         ubigvalue.push_back(digit);
         that = that / 10;
      }
   }
}

ubigint::ubigint (const string& that) {
   for (reverse_iterator digit=that.rbegin(); digit!=that.rend(); ++digit)	{
      if (not isdigit (*digit)) {
         throw invalid_argument ("ubigint::ubigint(" + that + ")");
      }
			uint8_t num = *digit - '0';
      ubigvalue.push_back(num);
   }
}

ubigint ubigint::operator+ (const ubigint& that) const {
   ubigint result(); 

   uint8_t carry = 0;
   uint8_t rem = 0;
   int sum = 0;
   long unsigned int i = 0;

   if (ubigvalue.size() != that.ubigvalue.size())	{
      int pad = ubigvalue.size() - that.ubigvalue.size();
      if (pad < 0)	{
         //Add common digits
         for (; i < that.ubigvalue.size(); i++)  {
            sum = ubigvalue.at(i) + that.ubigvalue.at(i) + carry;
            rem = sum % 10;
            carry = sum / 10;
            result.ubigvalue.push_back(rem);
         }
         //Loop through remaining digits
         for (; i < ubigvalue.size(); i++)  {
            sum = ubigvalue.at(i) + carry;
            rem = sum % 10;
            carry = sum / 10;
            result.ubigvalue.push_back(rem);
         }
         if (carry)  {
            result.ubigvalue.push_back(carry);
         }
      }else{	
		     for (; i < ubigvalue.size(); i++)  {
            sum = ubigvalue.at(i) + that.ubigvalue.at(i) + carry;
            rem = sum % 10;
            carry = sum / 10;
            result.ubigvalue.push_back(rem);
         }
         //Loop through remaining digits
         for (; i < that.ubigvalue.size(); i++)  {
            sum = that.ubigvalue.at(i) + carry;
            rem = sum % 10;
            carry = sum / 10;
            result.ubigvalue.push_back(rem);
         }
         if (carry)  {
            result.ubigvalue.push_back(carry);
         }
      }	
   }else	{
      for (; i < ubigvalue.size(); i++)	{
         sum = ubigvalue.at(i) + that.ubigvalue.at(i) + carry;
         rem = sum % 10;
         carry = sum / 10;
			   result.ubigvalue.push_back(rem);
      }
      if (carry)	{
         result.ubigvalue.push_back(carry);
      }
   }
   return result;
}

ubigint ubigint::operator- (const ubigint& that) const {
   return ubigint();
}

ubigint ubigint::operator* (const ubigint& that) const {
   return ubigint();
}

void ubigint::multiply_by_2() {
   return;
}

void ubigint::divide_by_2() {
   return;
}


struct quo_rem { ubigint quotient; ubigint remainder; };
quo_rem udivide (const ubigint& dividend, const ubigint& divisor_) {
   // NOTE: udivide is a non-member function.
   ubigint divisor {divisor_};
   ubigint zero {0};
   if (divisor == zero) throw domain_error ("udivide by zero");
   ubigint power_of_2 {1};
   ubigint quotient {0};
   ubigint remainder {dividend}; // left operand, dividend
   while (divisor < remainder) {
      divisor.multiply_by_2();
      power_of_2.multiply_by_2();
   }
   while (power_of_2 > zero) {
      if (divisor <= remainder) {
         remainder = remainder - divisor;
         quotient = quotient + power_of_2;
      }
      divisor.divide_by_2();
      power_of_2.divide_by_2();
   }
   DEBUGF ('/', "quotient = " << quotient);
   DEBUGF ('/', "remainder = " << remainder);
   return {.quotient = quotient, .remainder = remainder};
}

ubigint ubigint::operator/ (const ubigint& that) const {
	return ubigint();
}

ubigint ubigint::operator% (const ubigint& that) const {
	return ubigint();
}

bool ubigint::operator== (const ubigint& that) const {
   return true;
}

bool ubigint::operator< (const ubigint& that) const {
   return true;
}

ostream& operator<< (ostream& out, const ubigint& that) { 
   for (auto iter = that.ubigvalue.rbegin(); iter != that.ubigvalue.rend(); ++iter) {
        out << unsigned(*iter);
   }
   return out;
}

