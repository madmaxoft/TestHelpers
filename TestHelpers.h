// Helper macros for writing exception-based tests

/*
The tests are supposed to be contained in small static functions that get called from a main function provided by this framework:
static void test1()
{
	// Perform the test
	TEST_EQUAL(value1, value2);  // And similar TEST_ macros
}

...

IMPLEMENT_TEST_MAIN("TestName",
	test1();
	...
)
*/




#include "fmt/format.h"





/** The exception that is thrown if a test fails.
It doesn't inherit from any type so that it is not possible to catch it by a mistake,
it needs to be caught explicitly (used in the TEST_THROWS).
Along the position of the failing test, it also contains a single message that is to be displayed. */
class TestException
{
public:
	TestException(const std::string & aFileName, int aLineNumber, const std::string & aFunctionName, const std::string & aMessage):
		mFileName(aFileName),
		mLineNumber(aLineNumber),
		mFunctionName(aFunctionName),
		mMessage(aMessage)
	{
	}

	std::string mFileName;
	int mLineNumber;
	std::string mFunctionName;
	std::string mMessage;
};





/** Checks that the two values are equal. */
#define TEST_EQUAL(VAL1, VAL2) \
	do \
	{ \
		auto v1 = VAL1; \
		auto v2 = VAL2; \
		if (v1 != v2) \
		{ \
			throw TestException(__FILE__, __LINE__, __FUNCTION__, fmt::format("Equality test failed: {} != {} \n{} = {}\n{} = {}", \
				#VAL1, #VAL2, \
				#VAL1, v1, \
				#VAL2, v2 \
			)); \
		}\
	} while (false)





/** Checks that the two values are equal. */
#define TEST_EQUAL_MSG(VAL1, VAL2, MSG) \
	do \
	{ \
		auto v1 = VAL1; \
		auto v2 = VAL2; \
		if (v1 != v2) \
		{ \
			throw TestException(__FILE__, __LINE__, __FUNCTION__, fmt::format("Equality test failed: {} != {} ({})\n{} = {}\n{} = {}", \
				#VAL1, #VAL2, MSG, \
				#VAL1, v1, \
				#VAL2, v2 \
			)); \
		}\
	} while (false)





/** Checks that the two values are not equal. */
#define TEST_NOTEQUAL(VAL1, VAL2) \
	do \
	{ \
		auto v1 = VAL1; \
		auto v2 = VAL2; \
		if (v1 == v2) \
		{ \
			throw TestException(__FILE__, __LINE__, __FUNCTION__, fmt::format("Inequality test failed: {} == {} (== {})", \
				#VAL1, #VAL2, v1 \
			)); \
		} \
	} while (false)





/** Checks that the statement evaluates to true. */
#define TEST_TRUE(X) TEST_EQUAL(X, true)





/** Checks that the statement evaluates to false. */
#define TEST_FALSE(X) TEST_EQUAL(X, false)





/** Checks that the statement returns a value greater than or equal to the specified value. */
#define TEST_GREATER_THAN_OR_EQUAL(Stmt, Val) \
	do \
	{ \
		auto v1 = Stmt; \
		auto v2 = Val; \
		if (v1 < v2) \
		{ \
			throw TestException(__FILE__, __LINE__, __FUNCTION__, fmt::format("Comparison failed: {} < {}\n{} = {}\n{} = {}", \
				#Stmt, #Val, \
				#Stmt, v1, \
				#Val, v2 \
			)); \
		} \
	} while (false)





/** Checks that the statement returns a value less than or equal to the specified value. */
#define TEST_LESS_THAN_OR_EQUAL(Stmt, Val) \
	do \
	{ \
		auto v1 = Stmt; \
		auto v2 = Val; \
		if (v1 > v2) \
		{ \
			throw TestException(__FILE__, __LINE__, __FUNCTION__, fmt::format("Comparison failed: {} > {}\n{} = {}\n{} = {}", \
				#Stmt, #Val, \
				#Stmt, v1, \
				#Val, v2 \
			)); \
		} \
	} while (false)





/** Checks that the statement throws an exception of the specified class. */
#define TEST_THROWS(Stmt, ExcClass) \
	try \
	{ \
		Stmt; \
		throw TestException(__FILE__, __LINE__, __FUNCTION__, fmt::format("Failed to throw an exception of type {}", \
			#ExcClass \
		)); \
	} \
	catch (const ExcClass &) \
	{ \
		/* This is the expected case. */ \
	} \
	catch (const std::exception & exc) \
	{ \
		throw TestException(__FILE__, __LINE__, __FUNCTION__, fmt::format("An unexpected std::exception descendant was thrown, was expecting type {}. Exception message is: {}", \
			#ExcClass, exc \
		)); \
	} \
	catch (...) \
	{ \
		throw TestException(__FILE__, __LINE__, __FUNCTION__, fmt::format("An unexpected unknown exception object was thrown, was expecting type {}", \
			#ExcClass \
		)); \
	}





/** Checks that the statement throws an exception of any type. */
#define TEST_THROWS_ANY(Stmt) \
	try \
	{ \
		Stmt; \
		throw TestException(__FILE__, __LINE__, __FUNCTION__, "Failed to throw an exception of any type"); \
	} \
	catch (const TestException & exc) \
	{ \
		throw exc; \
	} \
	catch (...) \
	{ \
		/* This is the expected case */ \
	}





/** Fails the test unconditionally, with the specified message. */
#define TEST_FAIL(MSG) \
	throw TestException(__FILE__, __LINE__, __FUNCTION__, MSG);





/** Used to implement the main() function for tests. */
#define IMPLEMENT_TEST_MAIN(TestName, TestCode) \
int main() \
{ \
	fmt::print("Test started: {}\n", TestName); \
	\
	try \
	{ \
		TestCode; \
	} \
	catch (const TestException & exc) \
	{ \
		fmt::print("Test has failed at file {}, line {}, function {}:\n{}\n", \
			exc.mFileName, \
			exc.mLineNumber, \
			exc.mFunctionName, \
			exc.mMessage \
		); \
		return 1; \
	} \
	catch (const std::exception & exc) \
	{ \
		fmt::print("Test has failed, an exception was thrown: {}\n", exc.what()); \
		return 1; \
	} \
	catch (...) \
	{ \
		fmt::print("Test has failed, an unhandled exception was thrown.\n"); \
		return 1; \
	} \
	\
	fmt::print("Test finished\n"); \
	return 0; \
}
