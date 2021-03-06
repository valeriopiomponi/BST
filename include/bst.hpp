#ifndef __bst__
#define __bst__




#include <memory>  // std::unique_ptr
#include <iostream> //std::cout
#include <utility>  // std::pair
#include <functional> //std::less
#include <vector>
#include <algorithm>  // std::copy
#include "node.hpp"
#include "__iterator.hpp"

template<class key, class value, class cmp=std::less<key> >  
class bst{

  using pair_type =  typename std::pair<const key, value>;
  using node_type =  node<pair_type>;
  using iterator  =  __iterator<node<pair_type>, pair_type >;
  using const_iterator = __iterator<node<pair_type>, const pair_type>;  

 private:

  std::unique_ptr<node_type> head;

  cmp less;

 public:

  explicit bst() noexcept :  head{nullptr}, less{}  { std::cout << "bst custom ctor\n";}

  explicit bst(bst&&) noexcept = default; //move ctor set as default, act if as r-value is given (for example bst newtree(std::move(oldtree))
  
  bst& operator=(bst&&) noexcept = default;  //move assignement set as default

  // copy semantic
  explicit bst(const bst& tree) noexcept :
    head{ std::unique_ptr<node_type>( new node_type(tree.head.get()->value, nullptr) ) },  less{tree.less} {
      std::cout<<"deep copy ctor"<<std::endl;
      if (tree.head.get()->left)
	__copy(tree, tree.head.get()->left);
          
      if (tree.head.get()->right) 
	__copy(tree, tree.head.get()->right);
   
    }

  void __copy(const bst& tree, std::unique_ptr<node_type>& a) {
  
    this->emplace(a.get()->value);

    if (a.get()->left)
      __copy(tree, a.get()->left);
            
    if (a.get()->right)
      __copy(tree, a.get()->right);
            
    return;
  }


  bst& operator=(const bst& tree){
    std::cout << "copy assignement" << std::endl;
    (*this).clear();
    bst tree_new(tree);
    *this = std::move(tree_new);
    return *this;
  }

    
  
  template<typename OT>
  std::pair<iterator, bool> insert(OT&& x); //&& so i can pass to it an rvalue


  template<class... Types>  // variadic templates
  std::pair<iterator,bool> emplace(Types&&... args) {        //function needed to correctly call insert (giving a pair_type as input)
    return insert(pair_type{std::forward<Types>(args)...});  //in this way, empace can bel called with just two arguments which must be consistent with key and value, i.e tree.emplace(5, 8.443)
  }


  void clear() {          //to clear up the tree i simply put the head pointer to nullptr, so I can restart inserting nodes with any key
    head.reset(nullptr);
  }


  iterator begin() noexcept { //same implementation for tail() in linked list, where noexcept is used but i dont know why
    auto t = head.get();  //t will be a ptr to a node
    while(t->left){
      t=t->left.get();  //go left as you can
	}
    return iterator{t}; //call iterator ctor and return the iterator
  }


  const_iterator begin() const  noexcept {
    auto t = head.get();  //t will be a ptr to a node
    while(t->left){
      t=t->left.get();  //go left as you can
	}
    return const_iterator{t}; //call iterator ctor and return the iterator
  }

 


  iterator end()  noexcept { //same implementation fo linked list
    auto t = head.get();  //t will be a ptr to a node
    while(t->right){
      t=t->right.get();  //go right as you can
	}
    return iterator{t}; //call iterator ctor and return the iterator
  }



  const_iterator end() const noexcept { //same implementation fo linked list
    auto t = head.get();  //t will be a ptr to a node
    while(t->right){
      t=t->right.get();  //go right as you can
	}
    return const_iterator{t}; //call iterator ctor and return the iterator
  }  
    

  const iterator take_head() const noexcept{
    auto t=head.get();
    return const_iterator{t};
  }

  iterator take_head()  noexcept{
    auto t = head.get();
    return iterator{t};
  }




  iterator find(const key& x);
		      
  const_iterator find(const key& x) const; //const at the end of funciton declaration make sure class member are not modified inside the function  

  void VectorToBst(std::vector<std::pair<const key,value>> data, int start, int end, bst &bal_tree);

  void balance();

  
  value& operator[](const key& x){
    auto itbool = insert(pair_type{x,0});  //I try to insert a node with key x, if it already exist, insert retrun (iterator in x, false). Otherwise it create a new node with value 0 or the rvalue if given (I dont know why it works with the r-value but actually it works)

    return itbool.first->second;
  }

  value& operator[](key&& x){
    auto itbool = insert(pair_type{std::forward<key>(x), 0});

    return itbool.first->second;
  }  


  friend         //friend specifier, allow acces to privare members
  std::ostream& operator<<(std::ostream& os, const bst& x){

    if(x.head.get() == nullptr){
       std::cout<<"The tree is empty"<<std::endl;
       return os;
    }
 
    for(auto i = x.begin(); i!=x.end(); ++i){     //in this way I'm not printing final node
      std::cout << "Key = " << i->first << ", Value = " << i->second  << std::endl;	
    }

    auto i = x.end();
    std::cout << "Key = " << i->first << ", Value = " << i->second  << std::endl;

    return os;
    
  }


  void erase(const key& x);
  void erase_generic(iterator it_erase);
  void erase_head();
};




template < class key, class value, class cmp >
template<class OT>
std::pair<__iterator<node<std::pair<const key,value>>, std::pair<const key,value> >, bool> bst<key,value,cmp>::insert(OT&& x){

  auto t = head.get();

  while(t){
    if(less(x.first,t->value.first)){ //if key of the node to insert is smaller than key of pointed node, go left if you can, otherwise add a node
        if(t->left)
          t=t->left.get();
        else{
          
          t->left.reset(new node_type {std::forward<OT>(x),t}); //move ctor assigning t as the upper pointer fo the new node and x as value, using new initializer to call ctor inside the function reset 
          return std::make_pair(iterator{t->left.get()},true) ;
        }
      }         
    else if(less(t->value.first,x.first)){
        if(t->right)
          t=t->right.get();
        else{
          t->right.reset(new node_type{std::forward<OT>(x),t}); //why std::move doesnt work here?
          return std::make_pair(iterator{t->right.get()},true) ;
        }
    }
    else return std::make_pair(iterator{t},false) ;
  }

  head.reset(new node_type{std::forward<OT>(x), nullptr}) ; //empty tree case
  return std::make_pair(iterator{head.get()},true) ;

}





template < class key, class value, class cmp >
__iterator<node<std::pair<const key, value>>, std::pair<const key,value>> bst<key,value,cmp>::find(const key& x) {

  auto t = head.get();  
  while(t){
    if(less(x,t->value.first)){
      if(t->left)
        t=t->left.get();
      else
        return end();
    }
    else if(less(t->value.first,x)){
      if(t->right)
        t=t->right.get();
      else
        return end();
    }
    else
      return iterator{t}; 
    }
  return end();
}


template < class key, class value, class cmp >
__iterator<node<std::pair<const key, value>>,const std::pair<const key,value>> bst<key,value,cmp>::find(const key& x) const{

  auto t = head.get();  
  while(t){
    if(less(x,t->value.first)){
      if(t->left)
        t=t->left.get();
      else
        return end();
    }
    else if(less(t->value.first,x)){
      if(t->right)
        t=t->right.get();
      else
        return end();
    }
    else
      return const_iterator{t}; 
    }
  return end();
}


template<class key, class value, class cmp>
void bst<key, value, cmp>::VectorToBst(std::vector<std::pair<const key,value>> data,int start,int end, bst<key,value,cmp> &bal_tree){
  if (start > end){
    return;
  }
  int mid;
  mid = int((start +end)/2);

  bal_tree.insert(data[mid]);
  VectorToBst(data,start,mid-1,bal_tree);
  VectorToBst(data,mid+1,end,bal_tree);
  return;
}


template<class key, class value, class cmp>
void bst<key,value,cmp>::balance() {
  std::vector<pair_type> pippo;        //create a vector to pusk back inside it the values(key and value)  of the nodes

  for(auto i=begin(); i!=end(); ++i){  // i will be the iterator, in this way I'm omitting final node
    pippo.push_back(*i);  //in this way I pass the value of the node I'm pointing (see how i defined operator *() in class __iterator.hpp
  }

  auto i=end();
  pippo.push_back(*i);

  bst bal_tree{};  //create e new empty tree

  VectorToBst(pippo, 0, pippo.size()-1, bal_tree);  //this new tree became the balanced tree
  clear();  //delete recent tree

  head.reset(bal_tree.head.release());  //The master tree head take what the new tree's head is pointing, so it became the balanced tree


} 

  
  
template < class key, class value, class cmp >
void bst<key,value,cmp>::erase(const key& x){

  auto it_erase = find(x);
  auto node_to_erase = it_erase.current;  //here I'm accessing a private member of the class iterator. In order to do that, classes iterator and bst must be friend!

  if( it_erase == end()){
    std::cout<< "The key is not present."<<std::endl; //or it is the end of the tree
    return;
  }

  else{
    
    //case 1) node_to_erase=head
    if(node_to_erase==head.get()){
      
      erase_head();
     
    }
    //case 2) node_to_erase is generic
    else
     
      erase_generic(it_erase);

    return;
  }
}

template < class key, class value, class cmp >
void bst<key,value,cmp>::erase_generic(__iterator<node<std::pair<const key,value>>,std::pair<const key,value>> it_erase){


  auto node_to_erase = it_erase.current;  
  
  auto father = node_to_erase->upper;   
      
  //node_to erase has no child reset to nullptr the father's pointer
  if(node_to_erase->left == nullptr  && node_to_erase->right == nullptr){
      
      node_to_erase->upper=nullptr;

      if(less(node_to_erase->value.first,father->value.first))
        father->left.reset();
       
      else
        father->right.reset();
  }
  
    
  //second case the node has just right child
  else if(node_to_erase->left.get() == nullptr && node_to_erase->right.get() != nullptr){
      
      (node_to_erase->right)->upper=father;
      
      if(less(node_to_erase->value.first,father->value.first)) 
        father->left.reset(node_to_erase->right.release());
      
      else
        father->right.reset(node_to_erase->right.release());

      node_to_erase->upper=nullptr;
      
  }

    //second case the node has just left child
  else if(node_to_erase->left.get() != nullptr && node_to_erase->right.get() == nullptr){
      
      (node_to_erase->left)->upper=father;
       
      if(less(node_to_erase->value.first,father->value.first))
        
        father->left.reset(node_to_erase->left.release());
     
      else
        father->right.reset(node_to_erase->left.release());
      
      node_to_erase->upper=nullptr;
      
  }

  //node to eliminate has two children

  else{
    
    auto node_successor=(++it_erase).current;
   
    (node_to_erase->left)->upper=node_successor;
    
    node_successor->left.reset(node_to_erase->left.release());

    //case 1) successor is the right child of the node to delete
    if(node_successor==node_to_erase->right.get()){
      
      node_successor->upper=father;
      node_to_erase->upper=nullptr;

      if(less(node_to_erase->value.first,father->value.first))
        father->left.reset(node_to_erase->right.release());

      else
        father->right.reset(node_to_erase->right.release());  

    }
    //case 2) general case successor is not the right child of the node to delete
    else{
      
      //if the successor has a child (which has to be a right child)
      if (node_successor->right) {
         
        (node_successor->right)->upper = node_successor->upper;
        (node_successor->upper)->left.reset(node_successor->right.release());
      
      }

      //set the right child of the node as the right child of the node_successor
       
      node_successor->right.reset(node_to_erase->right.release());
      (node_successor->right)->upper=node_successor;
                
      //set the father of the node successor to be the father of the node_to_erase
      node_successor->upper=father;
      node_to_erase->upper=nullptr;

  
      //set the child of the father to node successor
      if(less(node_to_erase->value.first,father->value.first))

        //this case is when the node to be eliminated was a left child
        //and we reset the ownership
        father->left.reset(node_to_erase->right.release());
       
      //in this case was the right child
      else
        father->right.reset(node_to_erase->right.release());  

    } 
  }
  return;
}


template < class key, class value, class cmp >
void bst<key,value,cmp>::erase_head(){

  using pair_type = typename std::pair<const key,value>;
  using iterator  =  __iterator<node<pair_type>,pair_type>; 

  if(head.get()->right==nullptr && head.get()->left==nullptr){
    head.reset();
  }
  

  else{

    //head has just left child
    if(head.get()->right==nullptr){
      (head.get()->left)->upper=nullptr;
      head.reset(head.get()->left.release());
    }

    //head has right child 
    else{

      iterator H{head.get()};
      auto node_successor = (++H).current;  //pointer to node that must become the head
 
      if(head.get()->left.get()){
        (head.get()->left)->upper=node_successor; //the upper of left child will point to node successor (that must become the head)
        node_successor->left.reset(head.get()->left.release()); //node successor gain is correct left child 
      }

      //case 1) successor is the right child of the head
      if(node_successor==head.get()->right.get()){
        head.reset(head.get()->right.release());
        node_successor->upper=nullptr;
      }
      //case 2) general case successor is not the right child of the head
      else{
	     auto father_succ = node_successor->upper;

	     if (node_successor->right.get()) {
	       auto node_app = head.get()->right.release();
	       (node_successor->right)->upper = father_succ; //right child of successor take his new correct father
	       head.reset(father_succ->left.release()); // head take the correct node, but father of successor loose his left child
	       father_succ->left.reset(node_successor->right.release());  //father_succ take his new left child
	       node_successor->right.reset(node_app);//giving to the new head his correct right child
	       
	     }

	     else{
	       node_successor->right.reset(head.get()->right.release()); //the new head take is correct right child from old head
	       head.reset(father_succ->left.release()); //set new head
	     }
	     
        
	     (node_successor->right)->upper=node_successor; //set remaining upper pointers correctly
	     node_successor->upper=nullptr;
	   
        
      } 
    }               
  }
  return;
}


#endif
