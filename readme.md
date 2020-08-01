# Binary Search Tree by Valerio Piomponi

##How to compile and run
To compile and run the program just type "make" on the command line and then "./main.exe". To remove main.exe and recompile use "make clean".


##The code
The code is composed by a main.cc file and by three different headers file:
-In node.hpp the struct node is defined: The struct contains a templated value and three pointer to node (two unique pointer (left and right), and a normal pointer (upper)). Both a copy and a move ctor are defined in order to be able to construct a node with the value and the upper pointer. Left and right pointer are always set as nullptr when the node is constructed.

-In __iterator.hpp the templated class itarator is defined: The templates are the node and the value type owned by the node. The only member of the class (current) is a pointer to node. Then operators *() ; -> are defined in order to allow the access to the value of the node pointed by the iterator. Operator ++() allows to easily move inside the three in a forward manner, that means in an increasing order of the key values. This class is set friend of the class bst, in this way I can access the itarator class member (current) from the functions defined in the class bst.

-bst.hpp: it contains the binary search tree class. It is templated on the key type, value type associated to the nodes and the operation used to compare the nodes through its key values. In particular, the BST implemented is templated on std::less<Key_type> (< operator). BST class has two private member that are the head (a unique ptr to the top node of the three) and the comparator less. The move ctor and and the move assignements are set as default, while a deep copy semantics is implemented by the custom. All the functions and operator asked in the assignement are implemented in the public section of the class. Some functions (those with a bigger body) are just defined in the class and then declared outside it. As an extra, I also implemented a function take_head which return an iterator in the head position.

-main.cc: Here I wrote a simple code in order to test the funcionality of all the functions, operator and constructors implemented in the previous headers file. In order to better understand what the code does when run, and to check if the copy and move semantics is well written, each time a node or bst ctor/assignements is called, it is also printed on the screen. 


##Comments
In order to check that the program has no memory leaks, I used the command "valgrind --leak-check=full ./main.exe". No leaks were reported.

I wrote the codes alone but helping myself with the code written by my collegues Iuri Macocco and Mattia Ubertini. 
I used some of their functions trying to modify some parts and remove the parts I thought were useless. Sometimes this helped me out on uderstanding why this part I thought were useless were actually needed.
My goal was to exploit this exercise to learn and understand as much as possible about c++. I was able consolidate by knowledge about move and copy semantics usage, learn a lot of useful commands and keywords, and better understand on how to let classes talk to each other. 

