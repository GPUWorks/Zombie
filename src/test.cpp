#define BOOST_TEST_MODULE example
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE( test_suite )

BOOST_AUTO_TEST_CASE( test_case1 ) {
	BOOST_REQUIRE(true);
}

BOOST_AUTO_TEST_CASE( test_case2 ) {
	BOOST_ERROR( "some error 2" );
}

BOOST_AUTO_TEST_SUITE_END()