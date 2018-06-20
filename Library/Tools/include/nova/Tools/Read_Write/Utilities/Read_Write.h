//!#####################################################################
//! \file Read_Write.h
//!#####################################################################
// Class Read_Write
//######################################################################
#ifndef __Read_Write__
#define __Read_Write__

#include <cassert>
#include <cstring>
#include <memory>
#include <iostream>
#include <utility>
#include <vector>

namespace Nova{
template<class T> inline void Read_Primitive(std::istream& input,T& d)
{input.read((char*)&d,sizeof(T));}

template<class T> inline void Write_Primitive(std::ostream& output,const T& d)
{output.write((const char*)&d,sizeof(T));}

template<class T>
class Read_Write
{
  public:
    static void Read(std::istream& input,T& d)
    {input.read(reinterpret_cast<char*>(&d),sizeof(T));}

    static void Write(std::ostream& output,const T& d)
    {output.write(reinterpret_cast<const char*>(&d),sizeof(T));}
};

template<class T>
class Read_Write<T*>
{
  public:
    static void Read(std::istream& input,std::unique_ptr<T>& d)
    {
        bool data_exists;
        Read_Primitive(input,data_exists);
        if(data_exists)
        {
            if(d!=nullptr)
            {
                T *p=d.release();
                assert(d.get()==nullptr);
                delete p;
            }
            d=new T();
            Read_Write<T>::Read(input,*d);
        }
        else d=nullptr;
    }

    static void Write(std::ostream& output,std::unique_ptr<T> const& d)
    {
        Write_Primitive(output,d!=nullptr);
        if(d!=nullptr) Read_Write<T>::Write(output,*d);
    }
};

template<>
class Read_Write<std::string>
{
  public:
    static void Read(std::istream& input,std::string& d)
    {
        int n;
        Read_Primitive(input,n);
        char* buffer=new char[n];
        input.read(buffer,n);
        d.assign(buffer,buffer+n);
        delete[] buffer;
    }

    static void Write(std::ostream& output,const std::string& d)
    {
        int n=int(d.size());
        Write_Primitive(output,n);
        const char* s=d.c_str();
        output.write(s,n);
    }
};

template<class T>
class Read_Write<std::vector<T>>
{
  public:
    static void Read(std::istream& input,std::vector<T>& object)
    {
        unsigned size;
        Read_Primitive(input,size);
        object.clear();
        object.resize(size);
        for(unsigned i=0;i<size;++i) Read_Primitive(input,object[i]);
    }

    static void Write(std::ostream& output,const std::vector<T>& object)
    {
        Write_Primitive(output,object.size());
        for(unsigned i=0;i<object.size();++i) Write_Primitive(output,object[i]);
    }
};

template<class T>
class Read_Write<std::pair<T*,unsigned>>
{
  public:
    static void Read(std::istream& input,std::pair<T*,unsigned>& object)
    {
        unsigned size;
        Read_Primitive(input,size);
        if(size > object.second) throw(std::out_of_range("Block size too large!"));
        for(unsigned i=0;i<size;++i) Read_Primitive(input,object.first[i]);
    }

    static void Write(std::ostream& output,const std::pair<T*,unsigned>& object)
    {
        Write_Primitive(output,object.second);
        for(unsigned i=0;i<object.second;++i) Write_Primitive(output,object.first[i]);
    }
};
}
#endif
