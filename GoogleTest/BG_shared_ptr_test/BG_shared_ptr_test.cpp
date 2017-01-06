/*
Created by Borsik Gabor
gabor.borsik@gmail.com
*/
//Test cases for BG_shared_ptr

#include "stdafx.h"
#include <iostream>
#include "shared_ptr.hpp"
#include "graph.hpp"
#include "gtest/gtest.h"

struct delete_mock
{
public:
	explicit delete_mock(bool& flag_) : _flag(flag_) {}

	~delete_mock()
	{
		_flag = true;
	}

private:
	bool& _flag;
};

struct copy_mock
{
public:
	explicit copy_mock(int& destroyed_) :
		_destroyed(destroyed_),
		_created(nullptr)
	{}

	copy_mock(int& destroyed_, int& created_) :
		_destroyed(destroyed_),
		_created(&created_)
	{
		++created_;
	}

	copy_mock(const copy_mock& m_) :
		_destroyed(m_._destroyed),
		_created(m_._created)
	{
		if (_created)
		{
			++*_created;
		}
	}

	~copy_mock()
	{
		++_destroyed;
	}
private:
	int& _destroyed;
	int* _created;
};

///Test shared_ptr properties for non const objects
TEST(test_non_const, simple_object_destructor)
{
	bool deleted = false;

	{
		shared_ptr<delete_mock> p(new delete_mock(deleted));
	}
	EXPECT_TRUE(deleted) << "Object must be deleted!";
}

//TODO: User defined destructor
/*
TEST(test_non_const, array_destructor)
{
bool deleted1 = false;
bool deleted2 = false;

{
shared_ptr<delete_mock> p(new delete_mock[2]{ delete_mock(deleted1), delete_mock(deleted2) });
}
EXPECT_TRUE( deleted1 && deleted2 ) << "Both object must be deleted!";
}
*/

TEST(test_non_const, copy_constructor)
{
	int destroyed = 0;
	{
		shared_ptr<copy_mock> p1(new copy_mock(destroyed));
		shared_ptr<copy_mock> p2(p1);
	}
	EXPECT_EQ(destroyed, 1) << "Object must be delete once!";
}

TEST(test_non_const, assignment_operator)
{
	int destroyed = 0;
	{
		shared_ptr<copy_mock> p1(new copy_mock(destroyed));
		shared_ptr<copy_mock> p2 = p1;
	}
	EXPECT_EQ(destroyed, 1) << "Object must be delete once!";
}

TEST(test_non_const, self_assignment)
{
	int destroyed = 0;
	{
		shared_ptr<copy_mock> p1(new copy_mock(destroyed));
		p1 = p1;
	}
	EXPECT_EQ(destroyed, 1) << "Self assignment go wrong";
}

TEST(test_non_const, is_unique_function)
{
	shared_ptr<int> p1 = new int(1);
	EXPECT_TRUE(p1.is_unique()) << "This object must be unique!";
	shared_ptr<int> p2 = p1;
	EXPECT_FALSE(p1.is_unique()) << "This object must be not unique!";
	EXPECT_FALSE(p2.is_unique()) << "This object must be not unique!";
}

TEST(test_non_const, reset_function)
{
	shared_ptr<int> p1 = new int(1);
	shared_ptr<int> p2 = p1;
	EXPECT_FALSE(p1.is_unique()) << "This object must be not unique!";
	p2.reset();
	EXPECT_TRUE(p1.is_unique()) << "This object must be unique!";
}

TEST(test_non_const, get_function)
{
	shared_ptr<int> p = new int(123);
	EXPECT_EQ(*p.get(), 123);
}

TEST(test_non_const, dereference_operator)
{
	shared_ptr<int> p = new int(123);
	EXPECT_EQ(*p, 123);
	*p = 321;
	EXPECT_EQ(*p, 321);
}

TEST(test_non_const, string_index)
{
	shared_ptr<std::string> p = new std::string("Hello World");
	EXPECT_EQ((*p)[2], 'l');
}

TEST(test_graph, one_child)
{
	bool deleted1 = false;
	bool deleted2 = false;

	{
		shared_ptr<Graph<delete_mock>> p1(new Graph<delete_mock>(new delete_mock(deleted1)));
		shared_ptr<Graph<delete_mock>> p2(new Graph<delete_mock>(new delete_mock(deleted2)));
		(*p1).set_ptr1(p2);
		p2.reset();
	}
	EXPECT_TRUE(deleted1 && deleted2) << "Both objects must be deleted!";
}

TEST(test_graph, two_children)
{
	bool deleted1 = false;
	bool deleted2 = false;
	bool deleted3 = false;

	{
		shared_ptr<Graph<delete_mock>> p1(new Graph<delete_mock>(new delete_mock(deleted1)));
		shared_ptr<Graph<delete_mock>> p2(new Graph<delete_mock>(new delete_mock(deleted2)));
		shared_ptr<Graph<delete_mock>> p3(new Graph<delete_mock>(new delete_mock(deleted3)));
		(*p1).set_ptr1(p2);
		(*p1).set_ptr2(p3);
		p2.reset();
		p3.reset();
	}
	EXPECT_TRUE(deleted1) << "All objects must be deleted!";
	EXPECT_TRUE(deleted2) << "All objects must be deleted!";
	EXPECT_TRUE(deleted3) << "All objects must be deleted!";
}

TEST(test_graph, shared_children)
{
	bool deleted1 = false;
	bool deleted2 = false;
	bool deleted3 = false;
	bool deleted4 = false;

	{
		shared_ptr<Graph<delete_mock>> p1(new Graph<delete_mock>(new delete_mock(deleted1)));
		shared_ptr<Graph<delete_mock>> p2(new Graph<delete_mock>(new delete_mock(deleted2)));
		shared_ptr<Graph<delete_mock>> p3(new Graph<delete_mock>(new delete_mock(deleted3)));
		shared_ptr<Graph<delete_mock>> p4(new Graph<delete_mock>(new delete_mock(deleted4)));
		(*p1).set_ptr1(p2);
		(*p1).set_ptr2(p3);
		(*p2).set_ptr1(p4);
		(*p3).set_ptr1(p4);
		p2.reset();
		p3.reset();
		p4.reset();
	}
	EXPECT_TRUE(deleted1 && deleted2 && deleted3 && deleted4) << "All objects must be deleted!";
}

///Test shared_ptr properties for const objects, only overloaded functions
TEST(test_const, get_function)
{
	const shared_ptr<int> p = new int(123);
	EXPECT_EQ(*p.get(), 123);
}

TEST(test_const, dereference_operator)
{
	const shared_ptr<int> p = new int(123);
	EXPECT_EQ(*p, 123);
}

TEST(test_graph, copy_graph)
{
	int dcount = 0;
	int ccount = 0;
	Graph<copy_mock> g(new copy_mock(dcount));
	{
		Graph<copy_mock> h(g);
	}
	EXPECT_GT(ccount, dcount);
	std::cout << ccount << " " << dcount << std::endl;
}

int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
	std::getchar(); // keep console window open until Return keystroke
}
