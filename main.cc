#include <memory>  // std::unique_ptr
#include <iostream> //std::cout
#include <utility>  // std::pair
#include <functional> //std::less
#include <vector>
#include <algorithm>  // std::copy
#include "bst.hpp"


int main (){

  std::cout << "creating a tree<int, double> and a pair<int, double>"<<std::endl;
  bst<int, double>  tree; 
  std::pair<int, double>  pair(5, 7.2);

  std::cout << "Inserting the pair into the tree using auto iter = tree.insert(pair). Iter will be a pair containing an iterator to the node inserted and a bool."<<std::endl;
  auto iter = tree.insert(pair);  //in this way iter is a pair (iterator, bool)
  std::cout << "key of the node inserted: iter.first->first:  " << iter.first->first << std::endl;
  std::cout << "value fo the node inserted: iter.first->second:  " << iter.first->second << std::endl;
  std::cout << "iter.second:  " << iter.second << std::endl;

  std::cout<< "clearing the tree, emplacing new nodes and printing the whole tree" << std::endl;
  tree.clear();
  
  iter = tree.emplace(5, 0.55555);
  iter = tree.emplace(1,0.111);
  iter = tree.emplace(2,0.222);
  iter = tree.emplace(7,0.777);
  iter = tree.emplace(-3,1.3333);
  iter = tree.emplace(6, 0.6666);



  for(int i=0; i<30; i++){
    iter = tree.emplace(i, i*0.999);
  }

  std::cout <<"tree:\n" << tree << std::endl;


  iter.first = tree.begin();
  std::cout <<"printing the value of the node found with the function begin():    " << iter.first->second << std::endl;
  auto ite = tree.end();
  std::cout <<"printing the key of the node found with function end():  " << ite->first << std::endl;

  

  iter.first = tree.find(2);
  std::cout <<"printing the value of the node found with the function find(2):   " << iter.first->second << std::endl;

  
  iter.first = tree.take_head();
  std::cout<<"printing the key of the head node, found with function take_head():   "<< iter.first->first<<std::endl;

  std::cout<<"Using the function balance() on the tree. What happens is that a second tree is constructed and then tree take his head"<<std::endl;
  tree.balance();
  iter.first = tree.take_head();
  std::cout<<"reprinting the key of the head node, after having used balance() function on the tree:   "<< iter.first->first<<std::endl;

  std::cout<< "printing tree[28]:  "<< tree[28] << std::endl;
  std::cout <<"printing tree[-7], where node with key -7 didn't exist before:\n " << tree[-7] << std::endl;
  tree[77] = 37.45;
  std::cout <<"defining tree[77]=37.45 and printing tree[77]:  "<< tree[77] << std::endl;

  std::cout<<"creating newTree trough a depp copy of tree"<<std::endl;
  bst<int, double> newTree(tree);  //call copy ctor
  std::cout <<"printing newTree:\n"<< newTree << std::endl;
  
  tree.clear();
  std::cout<<"clearing tree and ricreate it with the copy assignement tree=newTree"<<std::endl;  
  tree = newTree;  //call copy assignement
  std::cout<<"tree:\n" << tree << std::endl;  

  std::cout<<"creating oak with a move ctor std::move(newTree), in this way no node ctor are called"<<std::endl;
  bst<int, double> oak(std::move(newTree));

  //iter = newTree.emplace(5, 555.5);  //call move ctor
  std::cout<<"printing newTree(should no longer exist):\n" << newTree << std::endl;
  std::cout<<"printing oak:\n" << oak << std::endl;


  bst<int, double> fir;
  std::cout<<"creating fir with move assignement fir=std::move(oak)"<<std::endl;
  fir = std::move(oak);  //call move assignement
  std::cout<<"printing fir:\n"<< fir << std::endl;
  std::cout <<"printing oak:\n"<< oak << std::endl;

  fir.erase(21);
  std::cout<<"printing fir after using erase(21):\n"<< fir << std::endl;


  fir.erase(14);
  iter.first = fir.take_head();
  std::cout<<"printing fir new head after using erase(14):  "<< iter.first->first<<std::endl;
  std::cout<<"printing fir:\n"<<fir<<std::endl;
 
}

