#ifndef MAVLINK_CPP_BYTEBUFFER_H
#define MAVLINK_CPP_BYTEBUFFER_H

#include <iostream>
#include <deque>

#include "protocol.h"

namespace mavlink
{
  class ByteBuffer
  {
    public:
      ByteBuffer():
        m_buffer()
      {}

      ByteBuffer(ByteBuffer const& buffer):
        ByteBuffer()
      {
        _load(buffer);
      }

      ByteBuffer(const char * data, size_t length):
        ByteBuffer()
      {
        for(unsigned int i = 0; i < length; ++i)
          m_buffer.push_back(data[i]);
      }

      void operator = (ByteBuffer const& buffer)
      {
        m_buffer.clear();
        _load(buffer);
      }

      template <typename T>
      void push_back(T n);
      void push_back(ByteBuffer const& n){_load(n);}


      template <typename T>
      void push_back_array(T const* n, int array_length);

      template <typename T>
      void operator << (T n);
      void operator << (ByteBuffer const& n){push_back(n);}

      template <typename T>
      T get(unsigned int n) const;
      template <typename T>
      T* get_array(unsigned int n, unsigned int array_length) const;

      uint8_t operator [] (unsigned int i) const{return m_buffer[i];}

      void pop_front(unsigned int i = 1)
      {
        for(unsigned int j = 0; j < i && m_buffer.size() > 0; ++j)
          m_buffer.pop_front();
      }

      size_t size() const{return m_buffer.size();}

      operator char*() const
      {
        char * buf = new char[m_buffer.size()];

        int i(0);
        for(auto it = m_buffer.cbegin(); it != m_buffer.cend(); ++it)
          buf[i++] = *it;

        return buf;
      }

      class const_iterator
      {
        friend class ByteBuffer;

        public:
          const_iterator(std::deque<uint8_t>::const_iterator it):m_it(it){}
          const_iterator(ByteBuffer::const_iterator const& it):m_it(it.m_it){}

          ByteBuffer::const_iterator& operator ++ (){++m_it;return *this;}
          ByteBuffer::const_iterator  operator ++ (int)
          {
            ByteBuffer::const_iterator tmp(*this);
            operator ++();
            return tmp;
          }
          ByteBuffer::const_iterator& operator -- (){--m_it;return *this;}
          ByteBuffer::const_iterator  operator -- (int)
          {
            ByteBuffer::const_iterator tmp(*this);
            operator --();
            return tmp;
          }

          uint8_t operator * () const{return *m_it;}
          bool operator == (ByteBuffer::const_iterator const& it) const
            {return m_it == it.m_it;}
          bool operator != (ByteBuffer::const_iterator const& it) const
            {return !operator == (it);}

        private:
          std::deque<uint8_t>::const_iterator m_it;
      };

      ByteBuffer::const_iterator cbegin() const{return ByteBuffer::const_iterator(m_buffer.cbegin());}
      ByteBuffer::const_iterator cend() const{return ByteBuffer::const_iterator(m_buffer.cend());}

    private:
      std::deque<uint8_t> m_buffer;

      void _load(ByteBuffer const& buffer)
      {
        for(auto it = buffer.m_buffer.cbegin(); it != buffer.m_buffer.cend(); ++it)
          m_buffer.push_back(*it);
      }
  };
};

//implemenation

template <typename T>
void mavlink::ByteBuffer::push_back(T n)
{
  const char * t = (const char *)&n;
  size_t length = sizeof(n);

  if(mavlink::NEED_BYTE_SWAP)
    for(unsigned int i = length - 1; i >= 0; --i)
      m_buffer.push_back(t[i]);
  else
    for(unsigned int i = 0; i < length; ++i)
      m_buffer.push_back(t[i]);
}

template <typename T>
void mavlink::ByteBuffer::push_back_array(T const* n, int array_length)
{
  for(int i = 0; i < array_length; ++i)
    push_back<T>(n[i]);
}

template <typename T>
void mavlink::ByteBuffer::operator << (T n)
{
  push_back<T>(n);
}

template <typename T>
T mavlink::ByteBuffer::get(unsigned int n) const
{
  const size_t length(sizeof(T));

  char * t = new char[length];
  for(size_t i = 0; i < length; ++i)
  {
    #if MAVLINK_NEED_BYTE_SWAP
      t[i] = m_buffer[n + length-1-i];
    #else
      t[i] = m_buffer[n + i];
    #endif
  }

  T ret(*(reinterpret_cast<T*>(t)));
  delete[] t;

  return ret;
}

template <typename T>
T* mavlink::ByteBuffer::get_array(unsigned int n, unsigned int array_length) const
{
  T* array = new T[array_length];
  for(unsigned int i = 0; i < array_length; ++i)
    array[i] = get<T>(n+i);

  return array;
}

#endif /* MAVLINK_CPP_BYTEBUFFER_H */
