//!#####################################################################
//! \file Array.h
//!#####################################################################
// Class Array
//######################################################################
#ifndef __Array__
#define __Array__

#include <nova/Tools/Utilities/Utilities.h>
#include <algorithm>
#include <cassert>
#include <cstring>
#include <vector>

namespace Nova{
/*! \class Array
 *
 - A C++11 wrapper for the std::vector<T> class which provides accessory
   functions for initialization, arithmetic, etc.
*/
template<class T>
class Array
{
    typedef std::vector<T> T_STORAGE;

  public:
    using iterator                          = typename T_STORAGE::iterator;
    using const_iterator                    = typename T_STORAGE::const_iterator;
    using reverse_iterator                  = typename T_STORAGE::reverse_iterator;
    using const_reverse_iterator            = typename T_STORAGE::const_reverse_iterator;

    T_STORAGE _data;

    // constructors

    /*! Empty base constructor
     */
    Array() {}

    /*! Base Constructor which resizes the array to a given size and fills it with an initial value.
     *
     * \param m Given size of the array.
     * \param initial_value All entries of the array default to this initial value.
     */
    Array(const size_t m,const T& initial_value=T(0))
        :_data(m,initial_value)
    {}

    /*! Base Destructor.
     */
    ~Array() {}

    /*! Vector Constructor
     *
     * \param other std::vector instance to initialize the Array from.
     */
    Array(const T_STORAGE& other)
        :_data(other)
    {}

    /*! Copy Constructor
     *
     * \param other Array to copy.
     */
    Array(const Array& other)
        :_data(other._data)
    {}

    /*! Initializer List Constructor
     *
     * \param l Initializer list from which to fill the Array.
     */
    Array(std::initializer_list<T> l)
    {
        _data.resize(l.size());
        std::copy(l.begin(),l.end(),_data.begin());
    }

    /*! Explicit Copy Constructor
     *
     * \param other Array to copy which is of a different type T2.
     */
    template<class T2> explicit Array(const Array<T2>& other)
    {
        _data.resize(other._data.size());
        for(size_t i=0;i<other._data.size();++i) _data[i]=(T)other[i];
    }

    /*! Append Operator
     *
     * \param x Element to be inserted at the end of the array.
     */
    void Append(const T& x)
    {_data.push_back(x);}

    /*! Append_Unique Operator
     *
     * \param x Element to be inserted in the array if it is not already present.
     */
    void Append_Unique(const T& x)
    {
        if(std::find(cbegin(),cend(),x)==cend()) _data.push_back(x);
    }

    /*! Append_Elements Operator
     *
     * \param other Array whose elements are inserted at the end.
     */
    void Append_Elements(const Array& other)
    {
        _data.insert(_data.end(),other._data.begin(),other._data.end());
    }

    /*! Checks if the array is empty.
     */
    bool Empty() const
    {return _data.empty();}

    /*! Clears the array.
     */
    void Clear()
    {_data.clear();}

    /*! Reserves space in the array.
     *
     * \param n Size of the array.
     */
    void Reserve(size_t n)
    {_data.reserve(n);}

    /*! Pops the last element in the array.
     */
    void Pop_Back()
    {_data.pop_back();}

    /*! Returns a reference to the last element in the array.
     */
    T& Back()
    {return _data.back();}

    /*! Removes a specified element from the array.
     *
     * \param index Index of the element to be deleted.
     */
    void Remove_Index(const int index)
    {
        assert(index>=0 && index<size());
        _data.erase(begin()+index);
    }

    /*! Returns a const reference to the last element in the array.
     */
    const T& Back() const
    {return _data.back();}

    /*! Assignment Operator
     */
    Array& operator=(const Array& other)
    {
        resize(other.size());
        std::copy(other.cbegin(),other.cend(),begin());
        return *this;
    }

    /*! Multiplication Operator
     *
     * \param a The factor to multiply all elements of the array with.
     */
    Array operator*(const T& a) const
    {
        Array ret(size());
        for(size_t i=0;i<size();++i) ret(i)=_data[i]*a;
        return ret;
    }

    /*! Multiplication and assignment operator
     *
     * \param a The factor to multiply all elements of the array with.
     */
    Array& operator*=(const T& a)
    {return *this=*this*a;}

    /*! Division Operator
     *
     * \param a The factor to divide all elements of the array by.
     */
    Array operator/(const T& a) const
    {
        Array ret(size());
        for(size_t i=0;i<size();++i) ret(i)=_data[i]/a;
        return ret;
    }

    /*! Division and assignment operator
     *
     * \param a The factor to divide all elements of the array by.
     */
    Array& operator/=(const T& a)
    {return *this=*this/a;}

    /*! Addition Operator
     *
     * \param a The array whose elements are added.
     */
    Array operator+(const Array& a) const
    {
        assert(size()==a.size());
        Array ret(size());
        for(size_t i=0;i<size();++i) ret(i)=_data[i]+a[i];
        return ret;
    }

    /*! Addition and assignment operator
     *
     * \param a The array whose elements are added.
     */
    Array& operator+=(const Array& a)
    {return *this=*this+a;}

    /*! Subtraction Operator
     *
     * \param a The array whose elements are subtracted.
     */
    Array operator-(const Array& a) const
    {
        assert(size()==a.size());
        Array ret(size());
        for(size_t i=0;i<size();++i) ret(i)=_data[i]-a[i];
        return ret;
    }

    /*! Subtraction and assignment operator
     *
     * \param a The array whose elements are subtracted.
     */
    Array& operator-=(const Array& a)
    {return *this=*this-a;}

    /*! Static copy operator
     *
     * \param c A constant.
     * \param array Another given array.
     * \param result Contains the result of multiplying array with the constant.
     */
    template<class T2>
    static void Copy(const T2 c,const Array& array,Array& result)
    {result=c*array;}

    /*! Static copy operator
     *
     * \param c1 A constant.
     * \param v1 Another given array.
     * \param v2 Another given array.
     * \param result Contains the result of a saxpy.
     */
    template<class T2>
    static void Copy(const T2 c1,const Array& v1,const Array& v2,Array& result)
    {result=c1*v1+v2;}

    /*! Static sort operator
     *
     * \param v The given array.
     */
    static void Sort(Array& v)
    {std::sort(v._data.begin(),v._data.end());}

    // accessors

    /*! Returns reference to the underlying std::vector.
     */
    T_STORAGE& data()
    {return _data;}

    /*! Returns size of the array.
     */
    size_t size() const
    {return _data.size();}

    /*! Resizes the array.
     *
     * \param size The length of the new array.
     */
    void resize(const size_t size)
    {_data.resize(size);}

    /*! Resizes the array.
     *
     * \param size The length of the new array.
     * \param initial_value Value to initialize the new elements with.
     */
    void resize(const size_t size,const T& initial_value)
    {_data.resize(size,initial_value);}

    /*! Fills the array with a specified value.
     *
     * \param value The constant value for initializing each entry in the Array.
     */
    void Fill(const T& value)
    {for(size_t i=0;i<size();++i) _data[i]=value;}

    /*! Returns const reference to the underlying std::vector.
     */
    const T_STORAGE& data() const
    {return _data;}

    /*! Returns iterator pointing to the beginning of the Array.
     */
    auto begin() -> typename T_STORAGE::iterator
    {return _data.begin();}

    /*! Returns iterator pointing to the end of the Array.
     */
    auto end() -> typename T_STORAGE::iterator
    {return _data.end();}

    /*! Returns const iterator pointing to the beginning of the Array.
     */
    auto cbegin() const -> typename T_STORAGE::const_iterator
    {return _data.cbegin();}

    /*! Returns const iterator pointing to the end of the Array.
     */
    auto cend() const -> typename T_STORAGE::const_iterator
    {return _data.cend();}

    /*! Returns reverse iterator pointing to the end of the Array.
     */
    auto rbegin() -> typename T_STORAGE::reverse_iterator
    {return _data.rbegin();}

    /*! Returns reverse iterator pointing to the beginning of the Array.
     */
    auto rend() -> typename T_STORAGE::reverse_iterator
    {return _data.rend();}

    /*! Returns const reverse iterator pointing to the end of the Array.
     */
    auto crbegin() const -> typename T_STORAGE::const_reverse_iterator
    {return _data.crbegin();}

    /*! Returns const reverse iterator pointing to the beginning of the Array.
     */
    auto crend() const -> typename T_STORAGE::const_reverse_iterator
    {return _data.crend();}

    /*! Returns reference to ith element using square brackets operator.
     */
    T& operator[](int i)
    {return _data.at(i);}

    /*! Returns reference to ith element using parentheses operator.
     */
    T& operator()(int i)
    {return _data.at(i);}

    /*! Returns const reference to ith element using parentheses operator.
     */
    const T& operator()(int i) const
    {return _data.at(i);}

    /*! Returns const reference to ith element using square brackets operator.
     */
    const T& operator[](int i) const
    {return _data.at(i);}

    /*! Returns base pointer.
     */
    T* Get_Array_Pointer()
    {return &_data[0];}

    /*! Returns const base pointer.
     */
    const T* Get_Array_Pointer() const
    {return &_data[0];}

    // IO

    /*! Output operator.
     *
     * \param out Output stream.
     * \param index Array that stores the values being output.
     */
    template<class TT>
    friend std::ostream& operator<<(std::ostream& out,const Array<T>& index);

    /*! Input operator.
     *
     * \param out Input stream.
     * \param index Array that will store the values being input.
     */
    template<class TT>
    friend std::istream& operator>>(std::istream& in,Array<T>& index);
};

template<class T,class T2>
Array<T2> operator*(const T& a,const Array<T2>& array)
{
    Array<T2> ret(array.size());
    for(size_t i=0;i<array.size();++i) ret(i)=array(i)*a;
    return ret;
}

template<class T>
std::ostream& operator<<(std::ostream& out,const Array<T>& index)
{
    std::copy(index.cbegin(),index.cend(),infix_ostream_iterator<T>(out," "));
    return out;
}

template<class T>
std::istream& operator>>(std::istream& in,Array<T>& index)
{
    std::copy(std::istream_iterator<T>(in),std::istream_iterator<T>(),index.begin());
    return in;
}
}
#include <nova/Tools/Read_Write/Arrays/Read_Write_Array.h>
#endif
