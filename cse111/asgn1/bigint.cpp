// $Id: bigint.cpp,v 1.3 2020-10-11 12:47:51-07 - - $

#include <cstdlib>
#include <exception>
#include <stack>
#include <stdexcept>
using namespace std;

#include "bigint.h"
#include "ubigint.h"

bigint::bigint (long that): uvalue (that), is_negative (that < 0) {
   DEBUGF ('~', this << " -> " << uvalue)
}

bigint::bigint (const ubigint& uvalue_, bool is_negative_):
                uvalue(uvalue_), is_negative(is_negative_) {
}

bigint::bigint (const string& that) {
   is_negative = that.size() > 0 and that[0] == '_';
   uvalue = ubigint (that.substr (is_negative ? 1 : 0));
}

bigint bigint::operator+ () const {
   return *this;
}

bigint bigint::operator- () const {
   return {uvalue, not is_negative};
}

bigint bigint::operator+ (const bigint& that) const {
   if (is_negative and that.is_negative)	{
			return {that.uvalue + uvalue, true}; 
   }
   if (not(is_negative) and not(that.is_negative))  {
      return {that.uvalue + uvalue, false}; 
   }
   if (is_negative and not (that.is_negative))  {
			if (uvalue == that.uvalue)	{
         return {uvalue - that.uvalue, false}; 
      } 
      if (uvalue > that.uvalue)	{
         return {uvalue - that.uvalue, true}; 
      }else  {
         return {that.uvalue - uvalue, false}; 
      }
   }
   if (not(is_negative) and that.is_negative)  {
      if (uvalue == that.uvalue)  {
         return {uvalue - that.uvalue, false};
      }
      if (uvalue > that.uvalue) {
         return {uvalue - that.uvalue, false}; 
      }else  {
         return {that.uvalue - uvalue, true};
      }
   }
   cout << "return empty" << endl;
	 bigint empty;
	 return empty;
}

bigint bigint::operator- (const bigint& that) const {
   if (is_negative and not(that.is_negative))  {
      return {uvalue + that.uvalue, true};
   }
   if (not(is_negative) and not(that.is_negative))  {
      if (uvalue == that.uvalue)	{
         return {that.uvalue - uvalue, false};
      }
      if (uvalue > that.uvalue) {
         return {uvalue - that.uvalue, false};
      }else  {
         return {that.uvalue - uvalue, true};
      }
   }
   if (is_negative and that.is_negative)  {
      if (uvalue == that.uvalue)  {
         return {uvalue - that.uvalue, false};
      }
      if (uvalue > that.uvalue) {
         return {uvalue - that.uvalue, true};
      }else  {
         return {that.uvalue - uvalue, false};
      }
   }
   if (not(is_negative) and that.is_negative)  {
      return {uvalue + that.uvalue, false};
   }
   cout << "FAIL";
   bigint empty;
   return empty;
}

bigint bigint::operator* (const bigint& that) const {
   if	(is_negative and that.is_negative)	{
      return{uvalue * that.uvalue, false};   
   }
   if (not (is_negative) and not (that.is_negative))  {
      return{uvalue * that.uvalue, false};
   }
   if (is_negative and not(that.is_negative))  {
      return{uvalue * that.uvalue, true};
   }
   if (not(is_negative) and that.is_negative)  {
      return{uvalue * that.uvalue, true};
   }
   cout << "FAIL";
   bigint result;
   return result;
}

bigint bigint::operator/ (const bigint& that) const {
   if (is_negative and that.is_negative)  {
      return{uvalue / that.uvalue, false};
   }
   if (not (is_negative) and not (that.is_negative))  {
      return{uvalue / that.uvalue, false};
   }
   if (is_negative and not(that.is_negative))  {
      return{uvalue / that.uvalue, true};
   }
   if (not(is_negative) and that.is_negative)  {
      return{uvalue / that.uvalue, true};
   }
   cout << "FAIL";
   bigint result;
   return result; 
}

bigint bigint::operator% (const bigint& that) const {
   if (is_negative and that.is_negative)  {
      return{uvalue % that.uvalue, false};
   }
   if (not (is_negative) and not (that.is_negative))  {
      return{uvalue % that.uvalue, false};
   }
   if (is_negative and not(that.is_negative))  {
      return{uvalue % that.uvalue, true};
   }
   if (not(is_negative) and that.is_negative)  {
      return{uvalue % that.uvalue, true};
   }
   cout << "FAIL";
   bigint result;
   return result;
}

bool bigint::operator== (const bigint& that) const {
   return is_negative == that.is_negative and uvalue == that.uvalue;
}

bool bigint::operator< (const bigint& that) const {
   if (is_negative != that.is_negative) return is_negative;
   return is_negative ? uvalue > that.uvalue
                      : uvalue < that.uvalue;
}

ostream& operator<< (ostream& out, const bigint& that) {
   return out << (that.is_negative ? "-" : "") << that.uvalue;
}

