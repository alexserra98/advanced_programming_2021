#include <algorithm> 
#include <vector>
#include <iostream>


/**********stack pool implementarion *************/
template <typename T, typename N = std::size_t>
class stack_pool{

  struct node_t{
    T value;
    N next;
  };

  std::vector<node_t> pool;
  using stack_type = N;
  using value_type = T;
  using size_type = typename std::vector<node_t>::size_type;
  stack_type free_nodes; 
  
  node_t& node(stack_type x)  noexcept { return pool.at(x-1); }
  const node_t& node(stack_type x) const noexcept { return pool.at(x-1); }

  public:
  template <typename X>
  /*****Iterator class implementation*****/
  class Iterator{
  N ptr;
  X * ptpool;
  public:
  using difference_type   = std::ptrdiff_t;
  using value_type        = T;
  using pointer           = N;  // or also value_type*
  using reference         = T&; 
  using iterator_category = std::forward_iterator_tag;

  explicit Iterator(pointer my_ptr, X *  my_ptpool): ptr{my_ptr}, ptpool{my_ptpool} {}
  void print_ptr() const noexcept {std::cout<<" my index is "<<ptr<<std::endl;} 
  pointer& myptr() noexcept {return ptr;} 
  // operator overloading
  reference operator*() const {return ptpool->at(ptr-1).value;}
  
  Iterator& operator++() {ptr= ptpool->at(ptr-1).next; return *this;} // prefix increment
  
  Iterator operator++(int) {auto tmp = *this; ++(*this); return tmp; } //postifx increment

  friend bool operator== (const Iterator& a, const Iterator& b) { return a.ptr == b.ptr; };
  friend bool operator!= (const Iterator& a, const Iterator& b) { return a.ptr != b.ptr; };     

};
/****************************************/
  //ITERATORS
  using iterator = Iterator<std::vector<node_t>> ;
  using const_iterator = Iterator<const std::vector<node_t>>;
  iterator begin(stack_type x){return iterator(x, &pool);} 
  iterator end(stack_type ){return iterator(end(),&pool);}
    
  const_iterator cbegin(stack_type x) const { return const_iterator( x, &pool); }
  const_iterator cend(stack_type ) const {return const_iterator( end(),  &pool);}

  //CONSTRUCTORS
  stack_pool() {free_nodes=new_stack();}
  
  explicit stack_pool(size_type n) {
    free_nodes=new_stack();
    pool.reserve(n);
  }
        
  stack_type new_stack() noexcept { return end(); } // return an empty stack

  // Vector handling
  void reserve(size_type n)  {pool.reserve(n);}; // reserve n nodes in the pool
  
  size_type capacity() const noexcept {return pool.capacity();}; // the capacity of the pool
  
  size_type size(){return pool.size();}
  
  bool empty(stack_type x) noexcept { return begin(x)==end(x);
    }
  stack_type end() const noexcept { return stack_type(0); }

  T& value(stack_type x)  { return node(x).value;}
  const T& value(stack_type x) const  { return node(x).value;}
  
  stack_type& next(stack_type x){
    return node(x).next;
  };
  const stack_type& next(stack_type x) const{
    return node(x).next;
  };
  
  stack_type push(const T& val, stack_type head) noexcept { // pushing a node on top
    node_t tmp;
    tmp.next=head;
    tmp.value=val;
    if(free_nodes==end()) {
      pool.push_back(std::move(tmp));
      head= pool.size();
    }
    else{
      auto tmp_ind =pool[free_nodes-1].next;
      pool[free_nodes-1]=std::move(tmp);
      head=free_nodes;
       /*updating free nodes*/ 
      free_nodes=tmp_ind;
    }
    return head;
  }

  stack_type push(T&& val, stack_type head) noexcept {
    node_t tmp;
    tmp.next=head;
    tmp.value= std::move(val);
    if(free_nodes==end()) {
      pool.push_back(std::move(tmp));
      head= pool.size();
    }
    else{
      auto tmp_ind =pool[free_nodes-1].next;
      pool[free_nodes-1]=std::move(tmp);
      head=free_nodes;
      /*updating free nodes */
      free_nodes=tmp_ind;
    }
    return head;

  }

  stack_type pop(stack_type x){ // delete first node
    auto head=next(x);
    /* updating free nodes */
    next(x)=free_nodes;
    free_nodes=x;
    return head;
  } 

  void print_node(stack_type x) noexcept { 
    std::cout<<"The value of the node is: "<<node(x).value<<std::endl;
  }
  stack_type free_stack(stack_type x){ // free entire stack
    auto it=cbegin(x);
    while(it!=cend(x)){
        auto tmp=it.myptr();
        pop(tmp);
        ++it;
    }
    return end();
  }

  void print_stack(const stack_type head) noexcept {
    std::cout << "my head is: " << head << ", " << std::endl;
    for (auto it = pool.cbegin(head); it != pool.cend(head); ++it) std::cout << it.print_ptr() << " and my value is: " << *it << std::endl;
  }
  
};

