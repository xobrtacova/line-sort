#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "options.h"
#include "sort.h"

#include <sstream>

using namespace std;

TEST_CASE("Options")
{
	SECTION("no options")
	{
		char * argv[] = { "line-sort" };
		REQUIRE(options::parse(_countof(argv), argv) == std::make_tuple(Order::ascending, Filter::all, Case::sensitive, (char *) nullptr));
	}

	SECTION("reversed")
	{
		SECTION("file")
		{
			char * argv[] = { "line-sort", "-r", "subor.txt" };
			auto options = options::parse(_countof(argv), argv);
			REQUIRE(options.value() == make_tuple(Order::descending, Filter::all, Case::sensitive, argv[_countof(argv)-1]));
		}

		SECTION("cin")
		{
			char * argv[] = { "line-sort", "-r" };
			auto options = options::parse(_countof(argv), argv);
			REQUIRE(options.value() == make_tuple(Order::descending, Filter::all, Case::sensitive, (char *) nullptr));
		}
	}

	SECTION("unique")
	{
		SECTION("file")
		{
			char * argv[] = { "line-sort", "-u", "subor.txt" };
			auto options = options::parse(_countof(argv), argv);
			REQUIRE(options.value() == make_tuple(Order::ascending, Filter::unique, Case::sensitive, argv[_countof(argv)-1]));
		}

		SECTION("cin")
		{
			char * argv[] = { "line-sort", "-u" };
			auto options = options::parse(_countof(argv), argv);
			REQUIRE(options.value() == make_tuple(Order::ascending, Filter::unique, Case::sensitive, (char *) nullptr));
		}
	}

	SECTION("ignore case")
	{
		SECTION("file")
		{
			char * argv[] = { "line-sort", "-i", "subor.txt" };
			auto options = options::parse(_countof(argv), argv);
			REQUIRE(options.value() == make_tuple(Order::ascending, Filter::all, Case::ignore, argv[_countof(argv)-1]));
		}

		SECTION("cin")
		{
			char * argv[] = { "line-sort", "-i" };
			auto options = options::parse(_countof(argv), argv);
			REQUIRE(options.value() == make_tuple(Order::ascending, Filter::all, Case::ignore, (char *) nullptr));
		}
	}

	SECTION("multiple")
	{
		SECTION("file and 3 options")
		{
			char * argv[] = { "line-sort", "-r", "-u", "-i", "subor.txt" };
			auto options = options::parse(_countof(argv), argv);
			REQUIRE(options.value() == make_tuple(Order::descending, Filter::unique, Case::ignore, argv[_countof(argv)-1]));
		}

		SECTION("cin and 3 options")
		{
			char * argv[] = { "line-sort", "-r", "-u", "-i" };
			auto options = options::parse(_countof(argv), argv);
			REQUIRE(options.value() == make_tuple(Order::descending, Filter::unique, Case::ignore, (char *) nullptr));
		}

		SECTION("file and 2 options reversed and unique")
		{
			char * argv[] = { "line-sort", "-r", "-u", "subor.txt" };
			auto options = options::parse(_countof(argv), argv);
			REQUIRE(options.value() == make_tuple(Order::descending, Filter::unique, Case::sensitive, argv[_countof(argv)-1]));
		}

		SECTION("cin and 2 options reversed and unique")
		{
			char * argv[] = { "line-sort", "-r", "-u" };
			auto options = options::parse(_countof(argv), argv);
			REQUIRE(options.value() == make_tuple(Order::descending, Filter::unique, Case::sensitive, (char *) nullptr));
		}

		SECTION("file and 2 options ignore case and reversed")
		{
			char * argv[] = { "line-sort", "-i", "-r", "subor.txt" };
			auto options = options::parse(_countof(argv), argv);
			REQUIRE(options.value() == make_tuple(Order::descending, Filter::all, Case::ignore, argv[_countof(argv)-1]));
		}

		SECTION("cin and 2 options ignore case and reversed")
		{
			char * argv[] = { "line-sort", "-i", "-r" };
			auto options = options::parse(_countof(argv), argv);
			REQUIRE(options.value() == make_tuple(Order::descending, Filter::all, Case::ignore, (char *) nullptr));
		}

		SECTION("file and 2 options unique and ignore case")
		{
			char * argv[] = { "line-sort", "-u", "-i", "subor.txt" };
			auto options = options::parse(_countof(argv), argv);
			REQUIRE(options.value() == make_tuple(Order::ascending, Filter::unique, Case::ignore, argv[_countof(argv)-1]));
		}

		SECTION("cin and 2 options unique and ignore case")
		{
			char * argv[] = { "line-sort", "-u", "-i" };
			auto options = options::parse(_countof(argv), argv);
			REQUIRE(options.value() == make_tuple(Order::ascending, Filter::unique, Case::ignore, (char *) nullptr));
		}
	}

	SECTION("one bad option")
	{
		SECTION("option before file")
		{
			char * argv[] = { "line-sort", "-dsfrf", "subor.txt" };
			auto options = options::parse(_countof(argv), argv);
			REQUIRE_FALSE(options.has_value());
		}

		SECTION("option behind file")
		{
			char * argv[] = { "line-sort", "subor.txt", "-r" };
			auto options = options::parse(_countof(argv), argv);
			REQUIRE_FALSE(options.has_value());
		}

		SECTION("cin")
		{
			char * argv[] = { "line-sort", "-isdlk" };
			auto options = options::parse(_countof(argv), argv);
			REQUIRE_FALSE(options.has_value());
		}
	}

	SECTION("two bad options")
	{
		SECTION("file")
		{
			char * argv[] = { "line-sort", "-dsfrf", "-dsfsa", "subor.txt" };
			auto options = options::parse(_countof(argv), argv);
			REQUIRE_FALSE(options.has_value());
		}

		SECTION("cin")
		{
			char * argv[] = { "line-sort", "-isdlk", "-dsf" };
			auto options = options::parse(_countof(argv), argv);
			REQUIRE_FALSE(options.has_value());
		}
	}

	SECTION("two options and the first bad option")
	{
		SECTION("file")
		{
			char * argv[] = { "line-sort", "-dsfrf", "-r", "subor.txt" };
			auto options = options::parse(_countof(argv), argv);
			REQUIRE_FALSE(options.has_value());
		}

		SECTION("cin")
		{
			char * argv[] = { "line-sort", "-isdlk", "-r" };
			auto options = options::parse(_countof(argv), argv);
			REQUIRE_FALSE(options.has_value());
		}
	}

	SECTION("two options and the second bad option")
	{
		SECTION("file")
		{
			char * argv[] = { "line-sort", "-u", "-dgasfg", "subor.txt" };
			auto options = options::parse(_countof(argv), argv);
			REQUIRE_FALSE(options.has_value());
		}

		SECTION("cin")
		{
			char * argv[] = { "line-sort", "-u", "-fsgf" };
			auto options = options::parse(_countof(argv), argv);
			REQUIRE_FALSE(options.has_value());
		}
	}

	SECTION("three options and the first bad option")
	{
		SECTION("file")
		{
			char * argv[] = { "line-sort", "-dsfrf", "-u", "-i", "subor.txt" };
			auto options = options::parse(_countof(argv), argv);
			REQUIRE_FALSE(options.has_value());
		}

		SECTION("cin")
		{
			char * argv[] = { "line-sort", "-isdlk", "-u", "-i" };
			auto options = options::parse(_countof(argv), argv);
			REQUIRE_FALSE(options.has_value());
		}
	}

	SECTION("three options and the second bad option")
	{
		SECTION("file")
		{
			char * argv[] = { "line-sort", "-r", "-fdsg", "-i", "subor.txt" };
			auto options = options::parse(_countof(argv), argv);
			REQUIRE_FALSE(options.has_value());
		}

		SECTION("cin")
		{
			char * argv[] = { "line-sort", "-r", "-fsgers", "-i" };
			auto options = options::parse(_countof(argv), argv);
			REQUIRE_FALSE(options.has_value());
		}
	}

	SECTION("three options and the third bad option")
	{
		SECTION("file")
		{
			char * argv[] = { "line-sort", "-r", "-u", "-dafsd", "subor.txt" };
			auto options = options::parse(_countof(argv), argv);
			REQUIRE_FALSE(options.has_value());
		}

		SECTION("cin")
		{
			char * argv[] = { "line-sort", "-r", "-u", "-fasg" };
			auto options = options::parse(_countof(argv), argv);
			REQUIRE_FALSE(options.has_value());
		}
	}

	SECTION("three bad options")
	{
		SECTION("file")
		{
			char * argv[] = { "line-sort", "-dsfrf", "-fsgs", "-dfa", "subor.txt" };
			auto options = options::parse(_countof(argv), argv);
			REQUIRE_FALSE(options.has_value());
		}

		SECTION("cin")
		{
			char * argv[] = { "line-sort", "-isdlk", "-fsgs", "-dfa" };
			auto options = options::parse(_countof(argv), argv);
			REQUIRE_FALSE(options.has_value());
		}
	}
}

namespace
{
	namespace data
	{
		std::istringstream empty{ R"()" };
		std::istringstream one_char{ R"( )" };
		std::istringstream one_line{ R"(
)" };
		std::istringstream numbered{ R"(1
3
9
4
5
2
)" };
	}
}

TEST_CASE("Sorting")
{
	SECTION("ascending")
	{
		std::ostringstream output{};

		REQUIRE(sort::process(Order::ascending, Filter::all, Case::sensitive, data::empty, output) == true);
		REQUIRE(output.str() == "");

		/*output.str("");
		REQUIRE(sort::process(Order::ascending, Filter::all, Case::sensitive, data::one_char, output) == true);
		REQUIRE(output.str() == " \n");

		output.str("");
		REQUIRE(sort::process(Order::ascending, Filter::all, Case::sensitive, data::one_line, output) == true);
		REQUIRE(output.str() == "\n");

		output.str("");
		REQUIRE(sort::process(Order::ascending, Filter::all, Case::sensitive, data::numbered, output) == true);
		REQUIRE(output.str() == "1\n2\n3\n4\n5\n9\n");*/
	}

	SECTION("ascending - unique")
	{
		std::ostringstream output{};

		REQUIRE(sort::process(Order::ascending, Filter::unique, Case::sensitive, data::empty, output) == true);
		REQUIRE(output.str() == "");

		/*output.str("");
		REQUIRE(sort::process(Order::ascending, Filter::unique, Case::sensitive, data::one_char, output) == true);
		REQUIRE(output.str() == " \n");

		output.str("");
		REQUIRE(sort::process(Order::ascending, Filter::unique, Case::sensitive, data::one_line, output) == true);
		REQUIRE(output.str() == "\n");

		output.str("");
		REQUIRE(sort::process(Order::ascending, Filter::unique, Case::sensitive, data::numbered, output) == true);
		REQUIRE(output.str() == "1\n2\n3\n4\n5\n9\n");*/
	}

	SECTION("ascending - unique - ignore case")
	{
		std::ostringstream output{};

		REQUIRE(sort::process(Order::ascending, Filter::unique, Case::ignore, data::empty, output) == true);
		REQUIRE(output.str() == "");

		/*output.str("");
		REQUIRE(sort::process(Order::ascending, Filter::unique, Case::ignore, data::one_char, output) == true);
		REQUIRE(output.str() == " \n");

		output.str("");
		REQUIRE(sort::process(Order::ascending, Filter::unique, Case::ignore, data::one_line, output) == true);
		REQUIRE(output.str() == "\n");

		output.str("");
		REQUIRE(sort::process(Order::ascending, Filter::unique, Case::ignore, data::numbered, output) == true);
		REQUIRE(output.str() == "1\n2\n3\n4\n5\n9\n");*/
	}

	SECTION("descending")
	{
		std::ostringstream output{};

		REQUIRE(sort::process(Order::descending, Filter::all, Case::sensitive, data::empty, output) == true);
		REQUIRE(output.str() == "");

		/*output.str("");
		REQUIRE(sort::process(Order::descending, Filter::all, Case::sensitive, data::one_char, output) == true);
		REQUIRE(output.str() == " \n");

		output.str("");
		REQUIRE(sort::process(Order::descending, Filter::all, Case::sensitive, data::one_line, output) == true);
		REQUIRE(output.str() == "\n");

		output.str("");
		REQUIRE(sort::process(Order::descending, Filter::all, Case::sensitive, data::numbered, output) == true);
		REQUIRE(output.str() == "1\n2\n3\n4\n5\n9\n");*/
	}

	SECTION("descending - unique")
	{
		std::ostringstream output{};

		REQUIRE(sort::process(Order::descending, Filter::unique, Case::sensitive, data::empty, output) == true);
		REQUIRE(output.str() == "");

		/*output.str("");
		REQUIRE(sort::process(Order::descending, Filter::unique, Case::sensitive, data::one_char, output) == true);
		REQUIRE(output.str() == " \n");

		output.str("");
		REQUIRE(sort::process(Order::descending, Filter::unique, Case::sensitive, data::one_line, output) == true);
		REQUIRE(output.str() == "\n");

		output.str("");
		REQUIRE(sort::process(Order::descending, Filter::unique, Case::sensitive, data::numbered, output) == true);
		REQUIRE(output.str() == "1\n2\n3\n4\n5\n9\n");*/
	}

	SECTION("descending - unique - ignore case")
	{
		std::ostringstream output{};

		REQUIRE(sort::process(Order::descending, Filter::unique, Case::ignore, data::empty, output) == true);
		REQUIRE(output.str() == "");

		/*output.str("");
		REQUIRE(sort::process(Order::descending, Filter::unique, Case::ignore, data::one_char, output) == true);
		REQUIRE(output.str() == " \n");

		output.str("");
		REQUIRE(sort::process(Order::descending, Filter::unique, Case::ignore, data::one_line, output) == true);
		REQUIRE(output.str() == "\n");

		output.str("");
		REQUIRE(sort::process(Order::descending, Filter::unique, Case::ignore, data::numbered, output) == true);
		REQUIRE(output.str() == "1\n2\n3\n4\n5\n9\n");*/
	}
}