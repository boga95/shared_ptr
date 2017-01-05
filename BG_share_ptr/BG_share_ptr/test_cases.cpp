#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "stdafx.h"

void TEST_CASE("destructor destroys object")
{
	REQUIRE(false);
}

/*TEST_CASE("only last destructor destroys object")
{
	bool deleted = false;

	{
		shared_ptr<delete_mock> p1(new delete_mock(deleted));
		{
			shared_ptr<delete_mock> p2(p1);
		}
		REQUIRE(!deleted);
	}

	REQUIRE(deleted);
}*/