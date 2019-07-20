#ifndef DATA_TYPES_HPP_
#define DATA_TYPES_HPP_

#include <systemc>
#include <iostream>
#include <string>
#include <sstream>

template <int LVL> class queue_type {
public:
  unsigned int data;
  sc_dt::sc_uint<LVL> eot;

  queue_type() : data(0), eot(0) {}
  queue_type(unsigned int d) : data(d) {}

  void operator=(const unsigned int rhs){ data = rhs; }
  void operator=(const queue_type rhs) {
    data = rhs.data;
    eot = rhs.eot;;
  }

  template<int W>
  void operator=(const sc_dt::sc_uint<W> rhs) { data = rhs; }

  bool eos() const{
    for(int i=0; i<LVL; i++)
      if(eot[i] == 0)
        return false;

    return true;
  }

  operator unsigned int() const {return data;};
  operator int() const {return data;};

  template<int L>
  friend std::ostream& operator<<(std::ostream& out, const queue_type<L>& q);

  std::string str(){
    std::stringstream ss;
    ss << "0x" << std::hex << data;
    return ss.str();
  }

};

template< int L>
std::ostream& operator<<(std::ostream& out, const queue_type<L>& q){
  out << "Data: " << q.data << "\n" << "eot: " << q.eot << std::endl;
  return out;
}

// template <int W>
// class Uint_type : public sc_dt::sc_uint<W>
// {
// public:
//   Uint_type<W>& operator = (sc_dt::uint_type v)
//   { sc_uint<W>::operator = (v); return *this; }
//   // int a;
//   // sc_dt::sc_uint<width> data;
// };

#endif /* DATA_TYPES_HPP_ */
