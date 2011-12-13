#ifndef _ALLOCATOR_INCLUDED_
#define _ALLOCATOR_INCLUDED_

/**
 * naive allocator, a wrapper around malloc/free
 * (c) 2011 Ji Han
 */

#include <cstdio>
#include <cstdlib>
#include <exception>
#include <limits>

namespace test {

template<typename T>
class allocator
{
  public:
    typedef T value_type;
    typedef T &reference;
    typedef const T &const_reference;
    typedef T *pointer;
    typedef const T *const_pointer;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    template<typename U> struct rebind { typedef allocator<U> other; };

    allocator() throw();
    allocator(const allocator &) throw();
    template<typename U> allocator(const allocator<U> &) throw();
    ~allocator() throw();

    bool operator==(const allocator &) const throw();
    bool operator!=(const allocator &) const throw();
    template<typename U> bool operator==(const allocator<U> &) const throw();
    template<typename U> bool operator!=(const allocator<U> &) const throw();

    size_type max_size() const throw();

    // the use of u is unspecified, but it is intended as an aid to locality.
    pointer allocate(size_type n, const_pointer u=nullptr) throw(std::bad_alloc);
    void deallocate(pointer p, size_type n) throw();

    void construct(pointer p, const_reference val) throw();
    void destroy(pointer p) throw();
};

template<> class allocator<void>
{
  public:
    typedef void value_type;
    typedef void *pointer;
    typedef const void *const_pointer;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    template<typename U> struct rebind { typedef allocator<U> other; };

    allocator() throw() {}
    allocator(const allocator &) throw() {}
    template<typename U> allocator(const allocator<U> &) throw() {}
    ~allocator() throw() {}
};

// naive allocator (simply a wrapper around malloc/free)
// ctor does nothing
template<typename T> allocator<T>::allocator() throw() {}
template<typename T> allocator<T>::allocator(const allocator<T> &) throw() {}
template<typename T> template<typename U> allocator<T>::allocator(const allocator<U> &) throw() {}

// dtor has nothing to do
template<typename T> allocator<T>::~allocator() throw() {}

// c.f. ISO/IEC 14882 requirements for operator==()
template<typename T>
bool allocator<T>::operator==(const allocator<T> &other) const throw()
{
    return true;
}

template<typename T>
bool allocator<T>::operator!=(const allocator<T> &other) const throw()
{
    return !(*this == other);
}

template<typename T> template<typename U>
bool allocator<T>::operator==(const allocator<U> &other) const throw()
{
    return *this == static_cast<typename allocator<U>::template rebind<T>::other>(other);
}

template<typename T> template<typename U>
bool allocator<T>::operator!=(const allocator<U> &other) const throw()
{
    return !(*this == other);
}

template<typename T>
typename allocator<T>::size_type allocator<T>::max_size() const throw()
{
    return std::numeric_limits<size_type>::max();
}

template<typename T>
typename allocator<T>::pointer
allocator<T>::allocate(typename allocator<T>::size_type n,
                       typename allocator<T>::const_pointer u)
throw(std::bad_alloc)
{
#if defined(_DEBUG)
    fprintf(stderr, "%s (%d, %x)\n", __FUNCTION__, n, u);
#endif
    typename allocator<T>::pointer p = nullptr;
    p = static_cast<typename allocator<T>::pointer>(malloc(static_cast<std::size_t>(sizeof(T) * n)));
    if (!p) { throw(std::bad_alloc()); }
    return p;
}

template<typename T>
void
allocator<T>::deallocate(typename allocator<T>::pointer p,
                         typename allocator<T>::size_type n)
throw()
{
#if defined(_DEBUG)
    fprintf(stderr, "%s (%x, %d)\n", __FUNCTION__, p, n);
#endif
    free(p);
}

template<typename T>
void
allocator<T>::construct(typename allocator<T>::pointer p,
                        typename allocator<T>::const_reference val)
throw()
{
#if defined(_DEBUG)
    fprintf(stderr, "%s (%x, %d)\n", __FUNCTION__, p, val);
#endif
    ::new (static_cast<void *>(p)) T(val);
}

template<typename T>
void
allocator<T>::destroy(typename allocator<T>::pointer p) throw()
{
#if defined(_DEBUG)
    fprintf(stderr, "%s (%x)\n", __FUNCTION__, p);
#endif
    p->~T();
}


} // namespace

#endif
