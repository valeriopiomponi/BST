#ifndef __iterator_HPP__
#define __iterator_HPP


#include <iostream> //std::cout
#include <utility>  // std::pair






template <typename node,typename O>
class __iterator{
	  
	  node* current;
	
	  public:
          explicit __iterator(node* x) noexcept : current{x} {}  //iterator ctor, ecplicit in order to avoid implicit conversion. 

	  using value_type = O;
	  using reference = value_type&; 
	  using pointer = value_type*;

          reference operator*() const noexcept { return current -> value; } //so one can do value = *iter

          pointer operator->() const noexcept { return &(current -> value); } //return pointer to the value of the node pointed by iterator

	  __iterator& operator++() noexcept{
		// allocate a pointer to a temporary node
		node* p;		  	

		if (current -> right != nullptr){
		 
		  current = current->right.get();

		  while (current->left != nullptr) {
		    current = current->left.get();
		  }
		}
		else{
		  p = current->upper;
		  while (p != nullptr && current == p->right.get()){
		    current = p;
		    p = p->upper;
		  }
		  current = p;
		}

		return *this;
	  }


  	  
          friend bool operator==(const __iterator& a, const __iterator& b) {  //if I dont put friend, it cant access class members
		    return a.current == b.current;
	  }
	  
	  friend bool operator!=(const __iterator& a, const __iterator& b) {
		    return !(a == b);
	  }

	  //we set as friend class the bst class in order to be able to access the data member
	  template<typename key, typename value, typename cmp>
	  friend class bst;


};
#endif
