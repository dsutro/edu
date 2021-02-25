// $Id: listmap.tcc,v 1.15 2019-10-30 12:44:53-07 - - $

#include "listmap.h"
#include "debug.h"

//
/////////////////////////////////////////////////////////////////
// Operations on listmap.
/////////////////////////////////////////////////////////////////
//

//
// listmap::~listmap()
//
template <typename key_t, typename mapped_t, class less_t>
listmap<key_t,mapped_t,less_t>::~listmap() {
   DEBUGF ('l', reinterpret_cast<const void*> (this));

   while(not empty()) {
      erase(begin());
   }
   //erase anchor
}

//
// iterator listmap::insert (const value_type&)
//
template <typename key_t, typename mapped_t, class less_t>
typename listmap<key_t,mapped_t,less_t>::iterator
listmap<key_t,mapped_t,less_t>::insert (const value_type& pair) {
   DEBUGF ('l', &pair << "->" << pair);

   auto iter = find(pair.first);
   if (iter != end()) {
      iter.where->value.second = pair.second;
      return iter;
   }
   
   iter = begin();
   while (iter != end() and less(iter.where->value.first, pair.first)) {
      ++iter;
   }
   node *insertNode = new node(iter.where, iter.where->prev, pair);
   insertNode->prev->next = insertNode;
   insertNode->next->prev = insertNode;
   return iterator(insertNode);
}
/*
   if(empty()) {
      node *insertNode = new node(anchor(), anchor(), pair);
      anchor()->next = insertNode;
      anchor()->prev = insertNode;
      return insertNode;
   }else {
      iterator iter = find(pair.first); 
      if(iter != end()) {
         iter.where->value.second = pair.second;
         return iter;
      }else {
         iter = begin();
         while(less(iter.where->value.first, pair.first) and iter != end()) {
            ++iter; 
         }
         if (iter != end()) {
            node *insertNode = new node(iter.where, iter.where->prev, pair);  
            insertNode->next->prev = insertNode;
            insertNode->prev->next = insertNode;
            return insertNode;
         }
         


         node *iterNode = anchor()->next;
         while(less(iterNode->value.first, pair.first) and iterNode->next != anchor()) {
            iterNode = iterNode->next;
         }
         node *insertNode = new node(iterNode, iterNode->prev, pair);
         iterNode->prev->next = insertNode;
         iterNode->prev = insertNode;
         return insertNode;

      }
   }
   return iterator();
}
*/

//
// listmap::find(const key_type&)
//
template <typename key_t, typename mapped_t, class less_t>
typename listmap<key_t,mapped_t,less_t>::iterator
listmap<key_t,mapped_t,less_t>::find (const key_type& that) {
   DEBUGF ('l', that);

   node *iterNode = anchor()->next;
   while(iterNode != anchor() and iterNode->value.first != that) {
      iterNode = iterNode->next;
   }
   return iterNode;
}

//
// iterator listmap::erase (iterator position)
//
template <typename key_t, typename mapped_t, class less_t>
typename listmap<key_t,mapped_t,less_t>::iterator
listmap<key_t,mapped_t,less_t>::erase (iterator position) {
   DEBUGF ('l', &*position);

   node *iter = anchor()->next;
   while(iter->value.first != position->first && iter != anchor()) {
      iter = iter->next;
   }

   if (iter != anchor()) {
      iter->next->prev = iter->prev;
      iter->prev->next = iter->next;
      iter->next = nullptr;
      iter->prev = nullptr;
      delete iter;
      position = nullptr;
   }

   return iterator();
}
