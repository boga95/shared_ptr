/*
Created by Borsik Gabor
gabor.borsik@gmail.com
*/
#ifndef GRAPH_HPP_INCLUDED
#define GRAPH_HPP_INCLUDED
#include <iostream>
#include "shared_ptr.hpp"

template <class T>
class Graph
{
public:
	Graph() {}
	Graph(T *object_) : _object(object_) {}
	Graph(const Graph<T>& graph);
	~Graph() { delete _object; }

	void set_ptr1(const shared_ptr<Graph<T>>& ptr1_) { _ptr1 = ptr1_; }
	void set_ptr2(const shared_ptr<Graph<T>>& ptr2_) { _ptr2 = ptr2_; }

	T* get_object() const { return _object; } 
	const shared_ptr<Graph<T>>& get_ptr1() const { return _ptr1; }
	const shared_ptr<Graph<T>>& get_ptr2() const { return _ptr2; }
	
private:
	shared_ptr<Graph<T>> _ptr1;
	shared_ptr<Graph<T>> _ptr2;
	T* _object;
};

template<class T>
Graph<T>::Graph(const Graph<T>& graph) :
	_object(graph.get_object())
										
{
	_ptr1 = graph.get_ptr1();
	_ptr2 = graph.get_ptr2();
}

#endif // GRAPH_HPP_INCLUDED