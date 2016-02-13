#ifndef ET_H
#define ET_H

// for debugging output..
#include <iostream>

// expression-template namespace
namespace et {
  
  /////////////////////////////////////////////////////////////
  // Type promotion rules /////////////////////////////////////
  /////////////////////////////////////////////////////////////
  
  // defines the set of numeric types (as far as this code is concerned)
  // as a template metaprogramming structure
  template< typename T > struct _is_numeric { static const bool value=false; };
  template<> struct _is_numeric<char> { static const bool value=true; };
  template<> struct _is_numeric<short> { static const bool value=true; };
  template<> struct _is_numeric<int> { static const bool value=true; };
  template<> struct _is_numeric<long int> { static const bool value=true; };
  template<> struct _is_numeric<float> { static const bool value=true; };
  template<> struct _is_numeric<double> { static const bool value=true; };

  
  // template metaprogramming structure returning larger type, here the
  // precedence rule is to return:
  //    - integer, integer      -> biggest integer type
  //    - integer, float/double -> float/double
  //    - float/double, integer -> float/double
  //    - float, double         -> double
  //    - double, float         -> double
  // for primitive types, it should be simple to generate this table of
  // possibilities algorithmically...but I didn't... :/
  template< typename L, typename R > struct _return_type {};
  // both types the same, return the type
  template< typename T > struct _return_type<T,T> { typedef T type; };
  // char promotion
  template<> struct _return_type<char,short> { typedef short type; };
  template<> struct _return_type<short,char> { typedef short type; };
  template<> struct _return_type<char,int> { typedef int type; };
  template<> struct _return_type<int,char> { typedef int type; };
  template<> struct _return_type<char,long int> { typedef long int type; };
  template<> struct _return_type<long int,char> { typedef long int type; };
  template<> struct _return_type<char,float> { typedef float type; };
  template<> struct _return_type<float,char> { typedef float type; };
  template<> struct _return_type<char,double> { typedef double type; };
  template<> struct _return_type<double,char> { typedef double type; };
  // short promotion
  template<> struct _return_type<short,int> { typedef int type; };
  template<> struct _return_type<int,short> { typedef int type; };
  template<> struct _return_type<long int, short> { typedef long int type; };
  template<> struct _return_type<short,long int> { typedef long int type; };
  template<> struct _return_type<short,float> { typedef float type; };
  template<> struct _return_type<float,short> { typedef float type; };
  template<> struct _return_type<short,double> { typedef double type; };
  template<> struct _return_type<double,short> { typedef double type; };
  // int promotion
  template<> struct _return_type<int,long int> { typedef long int type; };
  template<> struct _return_type<long int, int> { typedef long int type; };
  template<> struct _return_type<int,float> { typedef float type; };
  template<> struct _return_type<float,int> { typedef float type; };
  template<> struct _return_type<int,double> { typedef double type; };
  template<> struct _return_type<double,int> { typedef double type; };
  // long int promotion
  template<> struct _return_type<long int,float> { typedef float type; };
  template<> struct _return_type<float,long int> { typedef float type; };
  template<> struct _return_type<long int,double> { typedef double type; };
  template<> struct _return_type<double,long int> { typedef double type; };
  // float promotion
  template<> struct _return_type<float,double> { typedef double type; };
  template<> struct _return_type<double,float> { typedef double type; };

  // conditionally defines a type based on a condition, for
  // "substitution failure is not an error" (SFINAE) template
  // metaprogramming techniques
  template< bool cond, typename T > struct _enable_if { };
  template< typename T > struct _enable_if<true,T> { typedef T type; };
  
  /////////////////////////////////////////////////////////////
  // Operations to be performed on expressions ////////////////
  /////////////////////////////////////////////////////////////
  
  // performs an addition between two arguments of potentially different
  // types, performing type-promotion as determined by the _return_type
  // structure defined above
  class op_add {
  public:
    template< typename L, typename R >
    typename _return_type<L,R>::type operator()( const L& l, const R& r ) const {
      typedef typename _return_type<L,R>::type ret_type;
      return ret_type(l) + ret_type(r);
    }
  };
  
  // performs an addition between two arguments of potentially different
  // types, performing type-promotion as determined by the _return_type
  // structure defined above
  class op_sub {
  public:
    template< typename L, typename R >
    typename _return_type<L,R>::type operator()( const L& l, const R& r ) const {
      typedef typename _return_type<L,R>::type ret_type;
      return ret_type(l) - ret_type(r);
    }
  };

  // performs an multiplication between two arguments of potentially different
  // types, performing type-promotion as determined by the _return_type
  // structure defined above
  class op_mul {
  public:
    template< typename L, typename R >
    typename _return_type<L,R>::type operator()( const L& l, const R& r ) const {
      typedef typename _return_type<L,R>::type ret_type;
      return ret_type(l) * ret_type(r);
    }
  };
  
  // performs an division between two arguments of potentially different
  // types, performing type-promotion as determined by the _return_type
  // structure defined above
  class op_div {
  public:
    template< typename L, typename R >
    typename _return_type<L,R>::type operator()( const L& l, const R& r ) const {
      typedef typename _return_type<L,R>::type ret_type;
      return ret_type(l) / ret_type(r);
    }
  };

  /////////////////////////////////////////////////////////////
  // Base class for all expressions ///////////////////////////
  /////////////////////////////////////////////////////////////

  // vector expression class. vector expression implicitly must
  // provide two member methods:
  // - inline size_t size() const: returning number of elements
  //   in the expression
  // - inline stored_type operator[]( const size_t id ) const:
  //   returning the i'th element of the expression, lazily
  //   evaluated.
  template< typename exp_type >
  class vec_exp {
  public:
    // this is the core of the expression templates. using the
    // "curiously recursive template pattern", a base class can
    // return a reference to an instance of its derived class,
    // -without- resorting to a typical c++ inheritance approach
    operator const exp_type&() const {
      return static_cast<const exp_type&>(*this);
    }
  };

  /////////////////////////////////////////////////////////////
  // Output operation on vector expressions ///////////////////
  /////////////////////////////////////////////////////////////
  template< typename vec_type >
  std::ostream& operator<<( std::ostream& os, const vec_exp< vec_type>& _in ){
    const vec_type& in = _in;
    for( int i=0; i<in.size(); ++i ){
      os << (i!=0?" ":"") << in[i];
    }
    return os;
  }
  
  /////////////////////////////////////////////////////////////
  // Arithmetic expressions involving scalars and vectors /////
  /////////////////////////////////////////////////////////////
  
  // op( vector, vector )
  template< typename L, typename R, typename OP >
  class vec_vec_exp : public vec_exp< vec_vec_exp<L,R,OP> > {
  public:
    // define the type of the expression
    typedef vec_vec_exp<L,R,OP>       this_type;
    
    // define the left and right data types
    typedef typename L::stored_type   left_stored_type;
    typedef typename R::stored_type   right_stored_type;
    
    // define the return type of the expression
    typedef typename _return_type< left_stored_type, right_stored_type >::type  stored_type;
    
    // construct a vector-vector expression for a binary operation
    // e.g. +, -, *, /
    vec_vec_exp( const vec_exp<L>& left, const vec_exp<R>& right ) : m_L(left), m_R(right), m_op() {
      if( m_L.size() != m_R.size() )
        throw "dimension mismatch";
    }
    
    // return the size of the expression
    inline size_t size() const {
      return m_L.size();
    }
    
    // lazily evaluate the expression for the given index
    inline stored_type operator[]( const int id ) const {
      return m_op( m_L[id], m_R[id] );
    }
    
  private:
    // store a reference to the left and right operands.
    // these refer to existing vectors so by storing
    // references these values can be dereferenced lazily
    // without making copies
    const L& m_L;
    const R& m_R;
    const OP m_op;
  };
  
  // op( vector, scalar )
  template< typename L, typename R, typename OP >
  class vec_scalar_exp : public vec_exp< vec_scalar_exp<L,R,OP> > {
  public:
    // define the type of the expression
    typedef vec_vec_exp<L,R,OP>       this_type;
    
    // define the left and right data types
    typedef typename L::stored_type   left_stored_type;
    typedef          R                right_stored_type;
    
    // define the return type of the expression
    typedef typename _return_type< left_stored_type, right_stored_type >::type  stored_type;
    
    // construct a vector-scalar expression for a binary operation
    vec_scalar_exp( const vec_exp<L>& left, const R& right ) : m_L(left), m_R(right), m_op() {}
    
    // return the size of the operation
    inline size_t size() const {
      return m_L.size();
    }
    
    // lazily evaluate the operation for a given index
    inline stored_type operator[]( const int id ) const {
      return m_op( m_L[id], m_R );
    }
    
  private:
    // store a reference to the left operand, but the right
    // (a scalar) operand by value, so it doesn't fall out
    // of scope when generated by the operator overloads below
    const L& m_L;
    const R  m_R;
    const OP m_op;
  };
  
  // op( scalar, vector ), analogous to vec_scalar_exp...
  template< typename L, typename R, typename OP >
  class scalar_vec_exp : public vec_exp< scalar_vec_exp<L,R,OP> > {
  public:
    typedef scalar_vec_exp<L,R,OP>    this_type;
    typedef          L                left_stored_type;
    typedef typename R::stored_type   right_stored_type;
    typedef typename _return_type< left_stored_type, right_stored_type >::type  stored_type;
    
    // construct a scalar-vector expression for a given binary operation
    scalar_vec_exp( const L& left, const vec_exp<R>& right ) : m_L(left), m_R(right), m_op() {}
    
    // return the size of the operation
    inline size_t size() const {
      return m_R.size();
    }
    
    // lazily evaluate the expression for a given index
    inline stored_type operator[]( const int id ) const {
      return m_op( m_L, m_R[id] );
    }
    
  private:
    const L  m_L;
    const R& m_R;
    const OP m_op;
  };
  
  // overload the vector+vector operator
  template< typename L, typename R >
  vec_vec_exp< L, R, op_add > operator+( const vec_exp<L>& left, const vec_exp<R>& right ){
    return vec_vec_exp<L,R,op_add>( left, right );
  }
  
  // overload the vector+scalar operator, only enabling the operation when
  // the right operand is numeric (as determined by _is_numeric above)
  template< typename L, typename R >
  typename _enable_if< _is_numeric<R>::value, vec_scalar_exp< L, R, op_add > >::type operator+( const vec_exp<L>& left, const R& right ){
    return vec_scalar_exp<L,R,op_add>( left, right );
  }
  
  // overload the scalar+vector operator, only enabling the operation when
  // the left operand is numeric (as determined by _is_numeric above)
  template< typename L, typename R >
   typename _enable_if< _is_numeric<L>::value, scalar_vec_exp< L, R, op_add > >::type operator+( const L& left, const vec_exp<R>& right ){
    return scalar_vec_exp<L,R,op_add>( left, right );
  }
  
  // overload the vector-vector operator
  template< typename L, typename R >
  vec_vec_exp< L, R, op_sub > operator-( const vec_exp<L>& left, const vec_exp<R>& right ){
    return vec_vec_exp<L,R,op_sub>( left, right );
  }
  
  // overload the vector-scalar operator, only enabling the operation when
  // the right operand is numeric (as determined by _is_numeric above)
  template< typename L, typename R >
  typename _enable_if< _is_numeric<R>::value, vec_scalar_exp< L, R, op_sub > >::type operator-( const vec_exp<L>& left, const R& right ){
    return vec_scalar_exp<L,R,op_sub>( left, right );
  }
  
  // overload the scalar-vector operator, only enabling the operation when
  // the left operand is numeric (as determined by _is_numeric above)
  template< typename L, typename R >
  typename _enable_if< _is_numeric<L>::value, scalar_vec_exp< L, R, op_sub > >::type operator-( const L& left, const vec_exp<R>& right ){
    return scalar_vec_exp<L,R,op_sub>( left, right );
  }
  
  // overload the vector*scalar operator, only enabling the operation when
  // the right operand is numeric (as determined by _is_numeric above)
  template< typename L, typename R >
  typename _enable_if< _is_numeric<R>::value, vec_scalar_exp< L, R, op_mul > >::type operator*( const vec_exp<L>& left, const R& right ){
    return vec_scalar_exp<L,R,op_mul>( left, right );
  }
  
  // overload the scalar*vector operator, only enabling the operation when
  // the left operand is numeric (as determined by _is_numeric above)
  template< typename L, typename R >
  typename _enable_if< _is_numeric<L>::value, scalar_vec_exp< L, R, op_mul > >::type  operator*( const L& left, const vec_exp<R>& right ){
    return scalar_vec_exp<L,R,op_mul>( left, right );
  }
  
  // overload the vector/scalar operator, only enabling the operation when
  // the right operand is numeric (as determined by _is_numeric above)
  template< typename L, typename R >
  typename _enable_if< _is_numeric<R>::value, vec_scalar_exp< L, R, op_div > >::type operator/( const vec_exp<L>& left, const R& right ){
    return vec_scalar_exp<L,R,op_div>( left, right );
  }
  
  /////////////////////////////////////////////////////////////
  // Actual expression representing an array //////////////////
  /////////////////////////////////////////////////////////////
  template< typename stored >
  class vector : public vec_exp< vector<stored> > {
  public:
    // We need this to be able to tell what the vector
    // is actually storing when building the expression
    // tree. Without this we can't access the data type
    typedef stored      stored_type;
    
    // default constructor
    vector(){
      m_own_data = false;
      m_size = 0;
      m_data = NULL;
    }
    
    // construct a vector of n elements. here the vector
    // owns its data, but in any other case the vector
    // will refer to an existing vector's data.
    vector( const int n ){
      m_own_data = true;
      m_size = n;
      m_data = new stored[n];
    }
    
    // destroy a vector, taking special care to only
    // delete data that is owned, directly, by the instance
    ~vector(){
      if( m_own_data && m_data ){
        delete [] m_data;
      }
      m_own_data = false;
      m_data = NULL;
      m_size = 0;
    }
    
    // construct a vector from an instance of a vector
    // expression class. first cast to the derived class
    // then allocate data and lazily evaluate the expression.
    // there is no risk of aliasing here since the vectors
    // are being constructed (and so cannot be referenced
    // already in previous expressions).
    template< typename vec_type >
    vector( const vec_exp< vec_type>& _rhs ){
      const vec_type& rhs = _rhs;
      m_own_data = true;
      m_size = rhs.size();
      m_data = new stored[ m_size ];
      for( int i=0; i<m_size; ++i ){
        m_data[i] = rhs[i];
      }
    }
    
    // assign a vector from another instance of a vector
    // expression. first cast to the derived class and then
    // (free and) allocate data. finally lazily evaluate
    // the expression. subtle bugs can appear here if care
    // is not taken, e.g. if the vector expression refers
    // to data stored in the current vector. a safe approach
    // is to make a copy to avoid aliasing, however this
    // slows the code. this code uses the copy approach.
    template< typename vec_type >
    vector<stored>& operator=( const vec_exp< vec_type >& _rhs ){
      const vec_type& rhs = _rhs;
      if( m_own_data && m_data ){
        delete [] m_data;
      }
      // copy the input vector
      vector<stored> tmp( _rhs );
      // allocate new data
      m_own_data = true;
      m_size     = rhs.size();
      m_data     = new stored[ m_size ];
      // lazily evaluate the expression
      for( int i=0; i<m_size; ++i ){
        m_data[i] = tmp[i];
      }
      return *this;
    }
    
    // return a mutable reference for the i'th element
    inline stored& operator[]( const size_t id ){
      return m_data[id];
    }
    
    // return the size of the vector (expression)
    inline size_t size() const {
      return m_size;
    }
    
    // return the value of the i'th element, add range
    // & validity checks for robustness, leave them out
    // for speed and code size
    inline stored operator[]( const size_t id ) const {
      return m_data[id];
    }
    
  private:
    // vectors track their own data and only free data
    // when they own it, as indicated by the flag
    // m_own_data. data is only allocated when vectors
    // are constructed explicitly or assigned to expressions
    bool    m_own_data;
    size_t  m_size;
    stored  *m_data;
    
  };
  
};

#endif