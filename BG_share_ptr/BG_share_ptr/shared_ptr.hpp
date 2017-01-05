/*
Created by Borsik Gabor
gabor.borsik@gmail.com
*/
#ifndef SHARED_PTR_HPP_INCLUDED
#define SHARED_PTR_HPP_INCLUDED

#include <iostream>

template <class T>
class shared_ptr
{

public:
	shared_ptr() : _object(nullptr), _ref_counter(nullptr) {}
	shared_ptr(T *ptr_) : _object(ptr_), _ref_counter(new int(1)) {}
	shared_ptr(const shared_ptr<T>& ptr_) : _object(ptr_._object), _ref_counter(ptr_._ref_counter) { ++(*_ref_counter); }
	~shared_ptr();

	shared_ptr<T>& operator= (const shared_ptr<T>& ptr_);
	shared_ptr<T>& operator= (const T& ptr_);

	T& operator* () { return *_object; }
	const T& operator* () const { return *_object; }

	T& operator[] (int index_) { return _object[index_]; }
	const T& operator[] (int index_) const { return _object[index_]; }

	T& operator-> () { return *_object; }
	const T& operator-> () const { return *_object; }

	T* get() { return _object; }
	T* get() const { return _object; }
	bool is_unique() const { return (*_ref_counter) == 1; }
	shared_ptr<T>& reset();

private:
	T *_object;
	int *_ref_counter;
};

template <class T>
shared_ptr<T>::~shared_ptr()
{
	if (_ref_counter != nullptr)
	{
		if (*_ref_counter == 1) 
		{
			delete _object, _ref_counter;
		}
		else --(*_ref_counter);
	}
	else delete _object, _ref_counter;
}

template <class T>
shared_ptr<T>& shared_ptr<T>::operator= (const shared_ptr<T>& ptr_)
{
	if (this == &ptr_) return *this; // self-assignment
	else
	{
		if (_ref_counter == nullptr)
		{
			_object = ptr_._object;
			_ref_counter = ptr_._ref_counter;
			if (_ref_counter != nullptr) ++(*_ref_counter);
			return *this;
		}
		else
		{
			if (is_unique())
			{
				delete _object, _ref_counter;
				_object = ptr_._object;
				_ref_counter = ptr_._ref_counter;
				++(*_ref_counter);
				return *this;
			}
			else
			{
				--(*_ref_counter);
				_object = ptr_._object;
				_ref_counter = ptr_._ref_counter;
				++(*_ref_counter);
				return *this;
			}
		}
	}
}

template <class T>
shared_ptr<T>& shared_ptr<T>::operator= (const T& ptr_)
{
	*this = shared_ptr<T>(ptr_);
	return *this;
}

template <class T>
shared_ptr<T>& shared_ptr<T>::shared_ptr::reset()
{
	if (*_ref_counter == 1)
	{
		delete _object, _ref_counter;
		_object = nullptr;
		_ref_counter = nullptr;
		return *this;
	}
	else
	{
		--(*_ref_counter);
		_object = nullptr;
		_ref_counter = nullptr;
		return *this;
	}
}
#endif // SHARED_PTR_HPP_INCLUDED