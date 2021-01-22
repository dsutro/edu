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
   ubigint result; 

   uint8_t carry = 0;
   uint8_t rem = 0;
   int sum = 0;
   long unsigned int i = 0;

   if (ubigvalue.size() != that.ubigvalue.size())	{
      int pad = ubigvalue.size() - that.ubigvalue.size();
      if (pad > 0)	{
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
   while (result.ubigvalue.size() > 0 and result.ubigvalue.back() == 0) result.ubigvalue.pop_back();
   return result;
}

ubigint ubigint::operator- (const ubigint& that) const {
   ubigint result;

   if (*this == that)	{
      result.ubigvalue.clear();
      result.ubigvalue.push_back(0);
      return result;
   }

   int carry = 0;
   uint8_t rem = 0;
   int sum = 0;
   long unsigned int i = 0;

   if (ubigvalue.size() != that.ubigvalue.size()) {
      for (; i < that.ubigvalue.size(); i++)  {
         sum = ubigvalue.at(i) - that.ubigvalue.at(i) + carry;
         if (sum < 0) {
            rem = sum + 10;
            carry = -1;
         }else  {
            carry = 0;
            rem = sum;
         }
         result.ubigvalue.push_back(rem);
      }
      //Loop through remaining digits
      for (; i < ubigvalue.size(); i++)  {
         sum = ubigvalue.at(i) + carry;
         if (sum < 0) {
            rem = sum + 10;
            carry = -1;
         }else  {
            carry = 0;
            rem = sum;
         }
         result.ubigvalue.push_back(rem);
      }
   }else  {
      for (; i < ubigvalue.size(); i++) {
         sum = ubigvalue.at(i) - that.ubigvalue.at(i) + carry;
         if (sum < 0)	{
            rem = sum + 10;
            carry = -1;
         }else	{
            carry = 0;
            rem = sum;
         }
         result.ubigvalue.push_back(rem);
      }
   }
   while (result.ubigvalue.size() > 0 and result.ubigvalue.back() == 0) result.ubigvalue.pop_back();
   return result;
}

ubigint ubigint::operator* (const ubigint& that) const {
   ubigint result;
   int alloc = ubigvalue.size() + that.ubigvalue.size();
   uint8_t zero = 0;

   for (int i = 0; i < alloc; i++)	{
      result.ubigvalue.push_back(zero);
   }

   for (long unsigned int outer = 0; outer < ubigvalue.size(); outer++)	{
      int carry = 0;
      for (long unsigned int inner = 0; inner < that.ubigvalue.size(); inner++)	{
         int product = int(result.ubigvalue.at(inner + outer)) + 
                       int(ubigvalue.at(outer)) * int(that.ubigvalue.at(inner)) + carry;
         result.ubigvalue.at(inner + outer) = uint8_t(product % 10);
         carry = product / 10;
      }
      result.ubigvalue.at(outer + that.ubigvalue.size()) = uint8_t(carry);
   }
   while (result.ubigvalue.size() > 0 and result.ubigvalue.back() == 0) result.ubigvalue.pop_back();
   return result;
}

void ubigint::multiply_by_2() {
   *this = *this + *this;
}

void ubigint::divide_by_2() {
   if (ubigint(0) == *this)	{
      return;
   }
   ubigint search;
   int size = ubigvalue.size() - 1;
   int index = size;
   int rem = 0;

   while (1)	{
     uint8_t quo = 0;

     if (rem)	{
        quo = quo + rem;
        rem =0;
     }

     quo = quo + (int(ubigvalue.at(index)) / 2);
 
     if (ubigvalue.at(index) % 2)	{
        rem = 5;
     }

     ubigvalue.at(index) = quo;
		
     index--;      

     if (index == 0)	{
        break;
     } 
	 }
   while (ubigvalue.size() > 0 and ubigvalue.back() == 0) ubigvalue.pop_back();
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
	return udivide(*this, that).quotient;
}

ubigint ubigint::operator% (const ubigint& that) const {
	return udivide(*this, that).remainder;
}

bool ubigint::operator== (const ubigint& that) const {
   if (ubigvalue.size() != that.ubigvalue.size()) {
      return false;
   }else	{
      for (long unsigned int i =0; i < ubigvalue.size(); i++)	{
         if	(ubigvalue.at(i) != that.ubigvalue.at(i))	{
            return false;
         }
      }
   }
   return true;
}

bool ubigint::operator< (const ubigint& that) const {
   if (ubigvalue.size() == that.ubigvalue.size())	{
      for (long unsigned int i =0; i < ubigvalue.size(); i++) {
         if (ubigvalue.at(i) != that.ubigvalue.at(i)) {
            if (ubigvalue.at(i) < ubigvalue.at(i))	{
               return true;
            }else	{
               return false;
            }
         }
      }
      return false;
   }else	{
      if ((ubigvalue.size() - that.ubigvalue.size()) > 0)	{
         return false;
      }else	{
         return true;
      }
   }
}

ostream& operator<< (ostream& out, const ubigint& that) { 
   for (auto iter = that.ubigvalue.rbegin(); iter != that.ubigvalue.rend(); ++iter) {
        out << unsigned(*iter);
   }
   return out;
}

