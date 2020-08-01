#ifndef __node_HPP__
#define __node_HPP__

#include <memory>  // std::unique_ptr
#include <iostream> //std::cout
#include <utility>  // std::pair
#include <functional> //std::less
#include <vector>



template<class T>
struct node{

  T value;

        		 
  std::unique_ptr<node<T>> left;
        		
  std::unique_ptr<node<T>> right;
        		
  node<T>* upper;

  node() noexcept : value{}, left{nullptr}, right{nullptr}, upper{nullptr} {
    std::cout << "default ctor"<< std::endl;
  } 

  
  node(const T& v, node<T>* pu) noexcept : value{v}, left{nullptr}, right{nullptr}, upper{pu}{ 
    std::cout << "CALL: custom ctor node" << std::endl;
  }

		/**
		 * custom move constructor, moving the value to the node and setting only the upper pointer.
		 * Used whenever a right-value is given for the insertion in the tree
		 */
  node(T&& v, node<T>* pu) noexcept : value{std::move(v)}, left{nullptr}, right{nullptr}, upper{pu}{
    std::cout << "CALL: custom mctor node" << std::endl;
  }
};

#endif
