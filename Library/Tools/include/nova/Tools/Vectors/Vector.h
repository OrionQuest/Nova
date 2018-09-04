//!#####################################################################
//! \file Vector.h
//!#####################################################################
// Class Vector
//######################################################################
#ifndef __Vector__
#define __Vector__

#include <nova/Tools/Utilities/Utilities.h>
#include <array>
#include <algorithm>
#include <cassert>
#include <numeric>
#include <cmath>
#include <iostream>

namespace Nova{
/*! \class Vector
 *
 - A C++11 wrapper for the std::array<T,d> class which provides accessory
   functions for vector arithmetic, cross/dot product, etc.
*/
template <class T,int d,bool order_ascending=true> 
class Vector
{
    using T_STORAGE                         = std::array<T,d>;

  public:
    typedef typename T_STORAGE::iterator iterator;
    typedef typename T_STORAGE::const_iterator const_iterator;
    typedef typename T_STORAGE::reverse_iterator reverse_iterator;
    typedef typename T_STORAGE::const_reverse_iterator const_reverse_iterator;

    T_STORAGE _data;
    
    // constructors
    
    /*! Base Constructor
     *
     * Resulting Vector is filled with T().
     */
    Vector()
    {_data.fill(T());}

    /*! Scalar Value Constructor
     *
     * \param constant Scalar value to fill the Vector with.
     */
    Vector(T constant)
    {_data.fill(constant);}

    /*! Initializer List Constructor
     *
     * \param l Initializer list from which to fill the Vector. The list must be of length d.
     */
    Vector(std::initializer_list<T> l)
    {
        if(l.size()==d) std::copy(l.begin(),l.end(),_data.begin());
        else throw std::out_of_range("Cannot construct Vector from an initializer_list this size.");
    }
    
    /*! Array Constructor
     *
     * \param other std::array instance to initialize the Vector from.
     */
    Vector(const T_STORAGE& other)
        :_data(other)
    {}
    
    /*! Copy Constructor
     *
     * \param other Vector to copy.
     */
    Vector(const Vector& other)
        :_data(other._data)
    {}

    /*! Explicit Copy Constructor
     *
     * \param other Vector to copy which is of a different type T2.
     */
    template<class T2> explicit Vector(const Vector<T2,d>& other)
    {
        for(int i=0;i<d;++i) _data[i]=(T)other[i];
    }
    
    /*! Assignment Operator
     */
    Vector& operator=(const Vector& other)
    {
        std::copy(other.cbegin(),other.cend(),begin());
        return *this;
    }

    /*! Returns a vector with all elements in reverse order.
     */
    Vector Reversed() const
    {
        Vector ret;
        std::copy(crbegin(),crend(),ret.begin());
        return ret;
    }

    // Accessors

    /*! Returns reference to the underlying std::array.
     */
    T_STORAGE& data()
    {return _data;}

    /*! Returns const reference to the underlying std::array.
     */
    const T_STORAGE& data() const
    {return _data;}

    /*! Returns iterator pointing to the beginning of the Vector.
     */
    auto begin() -> typename T_STORAGE::iterator
    {return _data.begin();}

    /*! Returns iterator pointing to the end of the Vector.
     */
    auto end() -> typename T_STORAGE::iterator
    {return _data.end();}

    /*! Returns const iterator pointing to the beginning of the Vector.
     */
    auto cbegin() const -> typename T_STORAGE::const_iterator
    {return _data.cbegin();}

    /*! Returns const iterator pointing to the end of the Vector.
     */
    auto cend() const -> typename T_STORAGE::const_iterator
    {return _data.cend();}

    /*! Returns reverse iterator pointing to the end of the Vector.
     */
    auto rbegin() -> typename T_STORAGE::reverse_iterator
    {return _data.rbegin();}

    /*! Returns reverse iterator pointing to the beginning of the Vector.
     */
    auto rend() -> typename T_STORAGE::reverse_iterator
    {return _data.rend();}

    /*! Returns const reverse iterator pointing to the end of the Vector.
     */
    auto crbegin() const -> typename T_STORAGE::const_reverse_iterator
    {return _data.crbegin();}

    /*! Returns const reverse iterator pointing to the beginning of the Vector.
     */
    auto crend() const -> typename T_STORAGE::const_reverse_iterator
    {return _data.crend();}

    /*! Returns reference to ith element using parentheses operator.
     */
    T& operator()(int i)
    {return _data.at(i);}

    /*! Returns reference to ith element using square brackets operator.
     */
    T& operator[](int i)
    {return _data.at(i);}

    /*! Returns const reference to ith element using parentheses operator.
     */
    const T& operator()(int i) const
    {return _data.at(i);}

    /*! Returns const reference to ith element using square brackets operator.
     */
    const T& operator[](int i) const
    {return _data.at(i);}

    // IO

    /*! Output operator.
     *
     * \param out Output stream.
     * \param index Vector that stores the values being output.
     */
    template<class TT,int dd,bool order>
    friend std::ostream& operator<<(std::ostream& out,const Vector<T,d,order_ascending>& index);

    /*! Input operator.
     *
     * \param in Input stream.
     * \param index Vector that will store the values being input.
     */
    template<class TT,int dd,bool order>
    friend std::istream& operator>>(std::istream& in,Vector<T,d,order_ascending>& index);

    /*! Subtracts a vector from a scalar value.
     *
     * \param a The constant from which the values will be subtracted.
     * \param v The Vector which is being negated.
     */
    template<class TT,int dd,bool order>
    friend Vector<TT,dd,order> operator-(const T& a,const Vector<TT,dd,order>& v);

    // Arithmetic Operators

    /*! Adds a scalar value to all the components of the Vector and returns a new Vector.
     *
     * \param a The scalar value being added to each component of the Vector.
     */
    Vector operator+(T a) const
    {
        Vector data_copy(*this);
        data_copy+=a;
        return data_copy;
    }

    /*! Negation operator.
     */
    Vector operator-() const
    {
        Vector v;
        for(int i=0;i<d;++i) v[i]=-_data.at(i);
        return v;
    }

    /*! Inequality comparison operator
     *
     * \param v The Vector being compared with.
     */
    bool operator!=(const Vector& v) const
    {
        for(int i=0;i<d;++i) if(v[i]!=_data.at(i)) return true;
        return false;
    }

    /*! Adds a scalar value to all the components of the Vector.
     *
     * \param a The scalar value being added to each component of the Vector.
     */
    Vector& operator+=(T a)
    {
        std::transform(begin(),end(),begin(),[a](T e){return e + a;});
        return *this;
    }

    /*! Subtracts a scalar value from all the components of the Vector and returns a new Vector.
     *
     * \param a The scalar value being subtracted from each component of the Vector.
     */
    Vector operator-(T a) const
    {
        Vector data_copy(*this);
        data_copy-=a;
        return data_copy;
    }

    /*! Subtracts a scalar value from all the components of the Vector.
     *
     * \param a The scalar value being subtracted from each component of the Vector.
     */
    Vector& operator-=(T a)
    {
        std::transform(begin(),end(),begin(),[a](T e){return e - a;});
        return *this;
    }

    /*! Multiplies a scalar value to all the components of the Vector and returns a new Vector.
     *
     * \param a The scalar value being multiplied to each component of the Vector.
     */
    Vector operator*(T a) const
    { 
        Vector data_copy(*this);
        data_copy*=a;
        return data_copy;
    }

    /*! Multiplies a scalar value to all the components of the Vector.
     *
     * \param a The scalar value being multiplied to each component of the Vector.
     */
    Vector& operator*=(T a)
    {
        std::transform(begin(),end(),begin(),[a](T e){return e * a;});
        return *this;
    }

    /*! Divides all the components of the Vector by a scalar value and returns a new Vector.
     *
     * \param a The scalar value that divides each component of the Vector.
     */
    Vector operator/(T a) const
    {
        Vector data_copy(*this);
        data_copy/=a;
        return data_copy;
    }

    /*! Divides all the components of the Vector by a scalar value.
     *
     * \param a The scalar value that divides each component of the Vector.
     */
    Vector& operator/=(T a)
    {
        std::transform(begin(),end(),begin(),[a](T e){return e/a;});
        return *this;
    }

    /*! Adds a vector value to the Vector and returns a new Vector.
     *
     * \param a The vector value being added to the Vector.
     */
    Vector operator+(const Vector& a) const
    {
        Vector data_copy(*this);
        data_copy+=a;
        return data_copy;
    }

    /*! Adds a vector value to the Vector.
     *
     * \param a The vector value being added to the Vector.
     */
    Vector& operator+=(const Vector& a)
    {
        std::transform(begin(),end(),a.cbegin(),begin(),[](T e1,T e2){return e1+e2;});
        return *this;
    }

    /*! Subtracts a vector value from the Vector and returns a new Vector.
     *
     * \param a The vector value being subtracted from the Vector.
     */
    Vector operator-(const Vector& a) const
    {
        Vector data_copy(*this);
        data_copy-=a;
        return data_copy;
    }

    /*! Subtracts a vector value from the Vector.
     *
     * \param a The vector value being subtracted from the Vector.
     */
    Vector& operator-=(const Vector& a)
    {
        std::transform(begin(),end(),a.cbegin(),begin(),[](T e1,T e2){return e1-e2;});
        return *this;
    }

    /*! Multiplies a vector value to the Vector and returns a new Vector.
     *
     * \param a The vector value being multiplied to the Vector.
     */
    Vector operator*(const Vector& a) const
    {
        Vector data_copy(*this);
        data_copy*=a;
        return data_copy;
    }

    /*! Multiplies a vector value to the Vector.
     *
     * \param a The vector value being multiplied to the Vector.
     */
    Vector& operator*=(const Vector& a)
    {
        std::transform(begin(),end(),a.cbegin(),begin(),[](T e1,T e2){return e1*e2;});
        return *this;
    }

    /*! Divides the Vector by another vector and returns a new Vector.
     *
     * \param a The vector value dividing the Vector.
     */
    Vector operator/(const Vector& a) const
    {
        Vector data_copy(*this);
        data_copy/=a;
        return data_copy; 
    }

    /*! Divides the Vector by another vector.
     *
     * \param a The vector value dividing the Vector.
     */
    Vector& operator/=(const Vector& a)
    {
        std::transform(begin(),end(),a.cbegin(),begin(),[](T e1,T e2) {return e1/e2;});
        return *this;
    }
    
    // Comparison Operators

    /*! Returns true if the Vector is component-wise equal to another Vector.
     *
     * \param other The vector being compared to.
     */
    bool operator==(const Vector& other) const
    {
        bool result = std::equal(cbegin(),cend(),other.cbegin(),[](T e1,T e2){return e1==e2;});
        return result;
    }

    /*! Returns true if the Vector is lexicographically less than another Vector.
     *
     * \param other The vector being compared to.
     */
    bool operator<(const Vector& other) const
    {
        if(order_ascending){for(int v=0;v<d;++v){if(_data[v]>=other[v]){return false;}}}
        else{for(int v=d-1;v>=0;--v){if(_data[v]>=other[v]){return false;}}}
        return true;
    }

    /*! Returns true if the Vector is lexicographically greater than another Vector.
     *
     * \param other The vector being compared to.
     */
    bool operator>(const Vector& other) const
    {
        if(order_ascending){for(int v=0;v<d;++v){if(_data[v]<=other[v]){return false;}}}
        else{for(int v=d-1;v>=0;--v){if(_data[v]<=other[v]){return false;}}}
        return true;
    }

    /*! Returns true if the Vector is lexicographically less than or equal to another Vector.
     *
     * \param other The vector being compared to.
     */
    bool operator<=(const Vector& other) const
    {
        if(order_ascending){for(int v=0;v<d;++v){if(_data[v]>other[v]){return false;}}}
        else{for(int v=d-1;v>=0;--v){if(_data[v]>other[v]){return false;}}}
        return true;
    }

    /*! Returns true if the Vector is lexicographically greater than or equal to another Vector.
     *
     * \param other The vector being compared to.
     */
    bool operator>=(const Vector& other) const
    {
        if(order_ascending){for(int v=0;v<d;++v){if(_data[v]<other[v]){return false;}}}
        else{for(int v=d-1;v>=0;--v){if(_data[v]<other[v]){return false;}}}
        return true;
    }

    /*! Returns a higher-dimensional Vector with a new element inserted at a prescribed index.
     *
     * \param element Element to be inserted in the Vector.
     * \param index Index specifying where the element should be inserted.
     */
    Vector<T,d+1> Insert(const T& element,const int index) const
    {
        Vector<T,d+1> r;
        r[index]=element;
        for(int i=0;i<d;++i) r[i+(i>=index)]=(*this)[i];
        return r;
    }

    /*! Removes an entry at a given index and returns a lower dimensional vector.
     *
     * \param index The index of the element to be deleted.
     */
    Vector<T,d-1> Remove_Index(const int index) const
    {
        assert(index>=0 && index<d);
        Vector<T,d-1> ret;
        for(int i=0;i<index;++i) ret[i]=(*this)[i];
        for(int i=index+1;i<d;++i) ret[i-1]=(*this)[i];
        return ret;
    }

    /*! Returns a unit-vector along a given axis.
     *
     * \param axis The axis along which the vector points.
     */
    static Vector Axis_Vector(const int axis)
    {
        assert(axis>=0 && axis<d);
        Vector vec;vec(axis)=1;
        return vec;
    }

    /*! Initializes all entries to a specified value.
     *
     * \param value The value to initialize all entries with.
     */
    void Fill(const T value)
    {_data.fill(value);}

    /*! Returns the product of all the components of the Vector.
     */
    T Product() const
    {
        T value=(T)1.;
        for(int i=0;i<d;++i) value*=_data[i];
        return value;
    }

    /*! Returns the sum of all the components of the Vector.
     */
    T Sum() const
    {
        T value=(T)0.;
        for(int i=0;i<d;++i) value+=_data[i];
        return value;
    }

    /*! Returns the square of the Euclidean-norm of the Vector.
     */
    T Norm_Squared() const
    {
        return std::inner_product(cbegin(),cend(),cbegin(),T(),
                                  [](T e1,T e2){return e1+e2;},  // BiOp1 (*)
                                  [](T e1,T e2){return e1*e2;}   // BiOp2 (+)
                                 );
    }

    /*! Returns the Euclidean-norm of the Vector.
     */
    T Norm() const
    {
        return std::sqrt(Norm_Squared());
    }

    /*! Normalizes the Vector in-place.
     */
    T Normalize()
    {
        T norm=Norm();
        if(norm) *this=this->operator*((T)1./norm);
        else *this=Axis_Vector(0);
        return norm;
    }

    /*! Returns the normalized Vector.
     */
    Vector Normalized() const
    {
        Vector result(*this);
        result.Normalize();
        return result;
    }

    /*! Clamps the individual components to a specified threshold.
     *
     * \param a The clamping threshold.
     */
    Vector Clamp_Min(const T a) const
    {
        Vector result;
        for(int i=0;i<d;++i) result[i] = std::max(_data[i],a);
        return result;
    }

    /*! Returns a vector with absolute value for each component entry.
     */
    Vector Abs() const
    {
        Vector result;
        for(int i=0;i<d;++i) result[i]=(T)fabs(_data[i]);
        return result;
    }

    /*! Returns the dot product of the Vector with another Vector.
     */
    T Dot_Product(const Vector& v) const
    {
        return std::inner_product(cbegin(),cend(),v.cbegin(),T(),
                                  [](T e1,T e2){return e1+e2;},  // BiOp1 (+)
                                  [](T e1,T e2){return e1*e2;}   // BiOp2 (*)
                                 );
    }

    /*! Returns the minimum component of the Vector.
     */
    T Min() const
    {
        return *std::min_element(_data.begin(),_data.end());
    }

    /*! Returns the size of the Vector.
     */
    int Size() const
    {return d;}

    /*! Returns the maximum component of the Vector.
     */
    T Max() const
    {
        return *std::max_element(_data.begin(),_data.end());
    }

    /*! Returns true if a given element is present in the Vector, false otherwise.
     *
     * \param element The key being searched for.
     */
    bool Contains(const T& element) const
    {
        return std::find(cbegin(),cend(),element)!=cend();
    }

    /*! Returns true if all components are greater than the given Vector, false otherwise.
     *
     * \param v Given Vector being compared to.
     */
    bool All_Greater(const Vector& v) const
    {
        for(int i=0;i<d;++i) if(_data[i]<=v[i]) return false;
        return true;
    }

    /*! Returns true if all components are less than the given Vector, false otherwise.
     *
     * \param v Given Vector being compared to.
     */
    bool All_Less(const Vector& v) const
    {
        for(int i=0;i<d;++i) if(_data[i]>=v[i]) return false;
        return true;
    }

    /*! Returns true if all components are greater than or equal to the given Vector, false otherwise.
     *
     * \param v Given Vector being compared to.
     */
    bool All_Greater_Equal(const Vector& v) const
    {
        for(int i=0;i<d;++i) if(_data[i]<v[i]) return false;
        return true;
    }

    /*! Returns true if all components are less than or equal to the given Vector, false otherwise.
     *
     * \param v Given Vector being compared to.
     */
    bool All_Less_Equal(const Vector& v) const
    {
        for(int i=0;i<d;++i) if(_data[i]>v[i]) return false;
        return true;
    }

    /*! Returns the normalized orthogonal vector.
     */
    Vector Unit_Orthogonal_Vector() const
    {return Orthogonal_Vector().Normalized();}

//######################################################################
    /*! Returns an orthogonal vector.
     */
    Vector Orthogonal_Vector() const;
//######################################################################
};

template<class T,bool order_ascending=true> constexpr
Vector<T,3,order_ascending> Cross_Product(const Vector<T,3,order_ascending>& v1,const Vector<T,3,order_ascending>& v2)
{
    return Vector<T,3,order_ascending>{v1[1]*v2[2]-v1[2]*v2[1],v1[2]*v2[0]-v1[0]*v2[2],v1[0]*v2[1]-v1[1]*v2[0]};
}

template<class T,bool order_ascending=true> constexpr
Vector<T,1,order_ascending> Cross_Product(const Vector<T,2,order_ascending>& v1,const Vector<T,2,order_ascending>& v2)
{
    return Vector<T,1,order_ascending>(v1[0]*v2[1]-v1[1]*v2[0]);
}

template<class T,int d,bool order_ascending>
std::ostream& operator<<(std::ostream& out,const Vector<T,d,order_ascending>& index)
{
    std::copy(index.cbegin(),index.cend(),infix_ostream_iterator<T>(out," "));
    return out;
}

template<class T,int d,bool order_ascending>
std::istream& operator>>(std::istream& in,Vector<T,d,order_ascending>& index)
{
    std::copy(std::istream_iterator<T>(in),std::istream_iterator<T>(),index.begin());
    return in;
}

template<class T,int d,bool order_ascending>
Vector<T,d,order_ascending> Componentwise_Min(const Vector<T,d,order_ascending>& v1,const Vector<T,d,order_ascending>& v2)
{
    Vector<T,d,order_ascending> vec;
    for(int i=0;i<d;++i) vec[i]=std::min(v1[i],v2[i]);
    return vec;
}

template<class T,int d,bool order_ascending>
Vector<T,d,order_ascending> Componentwise_Max(const Vector<T,d,order_ascending>& v1,const Vector<T,d,order_ascending>& v2)
{
    Vector<T,d,order_ascending> vec;
    for(int i=0;i<d;++i) vec[i]=std::max(v1[i],v2[i]);
    return vec;
}

template<class T,int d,bool order_ascending>
Vector<T,d,order_ascending> operator-(const T& a,const Vector<T,d,order_ascending>& v)
{
    Vector<T,d,order_ascending> vec;
    for(int i=0;i<d;++i) vec[i]=a-v[i];
    return vec;
}

template<class T,int d,bool order_ascending>
Vector<T,d,order_ascending> operator*(const T& a,const Vector<T,d,order_ascending>& v)
{
    Vector<T,d,order_ascending> vec;
    for(int i=0;i<d;++i) vec[i]=a*v[i];
    return vec;
}

template<class T,int d,bool order_ascending> inline Vector<T,d,order_ascending>
operator/(const T& a,const Vector<T,d,order_ascending>& v)
{
    Vector<T,d,order_ascending> result;
    for(int i=0;i<d;++i) result[i]=a/v[i];
    return result;
}
}
#include <nova/Tools/Read_Write/Vectors/Read_Write_Vector.h>
#endif
