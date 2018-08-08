//!#####################################################################
//! \file Array_ND.h
//!#####################################################################
// Class Array_ND
//######################################################################
#ifndef __Array_ND__
#define __Array_ND__

#include <nova/Tools/Arrays/Array.h>
#include <nova/Tools/Utilities/Range.h>

namespace Nova{
/*! \class Array_ND
 *
 - A multi-dimensional array that allows storage of data on Cartesian grid-based lattices.
*/
template<class T,int d>
class Array_ND
{
  public:
    Range<int,d> domain;
    Array<T> data;
    Vector<int,d> counts;

    // constructor

    /*! Default Constructor which initializes an empty array.
     */
    Array_ND()
    {}

    /*! Base Constructor which resizes the array to a given size and fills it with an initial value.
     *
     * \param domain_input Range of indices that address individual elements in the array.
     * \param initial_value All entries of the array default to this initial value.
     */
    Array_ND(const Range<int,d>& domain_input,const T& initial_value=T(0))
        :domain(domain_input),data(domain.Size(),initial_value)
    {
        counts=domain.max_corner-domain.min_corner+1;
    }

    /*! Resizes the array to a given size and fills it with an initial value.
     *
     * \param domain_input Range of indices that address individual elements in the array.
     * \param initial_value All entries of the array default to this initial value.
     */
    void Resize(const Range<int,d>& domain_input,const T& initial_value=T(0))
    {
        domain=domain_input;
        data.resize(domain.Size(),initial_value);
        counts=domain.max_corner-domain.min_corner+1;
    }

    /*! Returns true if a given index is inside the domain (ends inclusive), and false otherwise.
     *
     * \param index Index of the element being tested.
     */
    bool Valid_Index(const Vector<int,d>& index)
    {return domain.Inside(index);}

    /*! Returns reference to element using parentheses operator.
     *
     * \param index Index of the element being referenced.
     */
    T& operator()(const Vector<int,d>& index)
    {
        int linear_index=Compute_Index(index);
        return data(linear_index);
    }

    /*! Returns reference to element using square brackets operator.
     *
     * \param index Index of the element being referenced.
     */
    T& operator[](const Vector<int,d>& index)
    {
        int linear_index=Compute_Index(index);
        return data(linear_index);
    }

    /*! Returns const reference to element using parentheses operator.
     *
     * \param index Index of the element being referenced.
     */
    const T& operator()(const Vector<int,d>& index) const
    {
        int linear_index=Compute_Index(index);
        return data(linear_index);
    }

    /*! Returns const reference to element using square brackets operator.
     *
     * \param index Index of the element being referenced.
     */
    const T& operator[](const Vector<int,d>& index) const
    {
        int linear_index=Compute_Index(index);
        return data(linear_index);
    }

  private:

    /*! Returns the linear index corresponding to a multi-dimensional geometric index.
     *
     * \param index The multi-dimensional index whose linear index is being computed.
     */
    inline int Compute_Index(const Vector<int,d>& index) const
    {
        Vector<int,d> actual_index=index-domain.min_corner;
        int linear_index=actual_index(0);
        for(int v=1;v<d;++v) linear_index=linear_index*counts(v)+actual_index(v);
        return linear_index;
    }
};
}
#endif
