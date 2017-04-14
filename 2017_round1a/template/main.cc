
#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <algorithm>

typedef std::size_t MY_SIZE;

typedef std::string RESULT;
typedef std::string CASE;

namespace CFG
{
	static const bool verbose = true;
	static const bool very_verbose = true;
}


RESULT compute(const CASE & my_case)
{
	std::string result_string;
	result_string += "Result for case \"";
	result_string += my_case;
	result_string += "\"";
	return result_string;
}

CASE read_one_case()
{
	std::string case_string;
	std::cin >> case_string;

	if (!std::cin.good())
	{
		throw std::runtime_error("IO error while reading case!");
	}

	return (CASE(case_string));
}

int main()
{

	MY_SIZE num_cases = 0;
	std::cin >> num_cases;
	if (CFG::very_verbose)
	{
		std::cout << "Cases: " << num_cases << std::endl;
	}

	if (!std::cin.good())
	{
		throw std::runtime_error("Can't read number of cases");
	}

	for (MY_SIZE i_case = 1; i_case <= num_cases; ++i_case)
	{
		CASE my_case = read_one_case();

		if (CFG::very_verbose)
		{
			std::cout << "Input Case #" << i_case << ": " << my_case << std::endl;
		}

		RESULT my_result = compute(my_case);
		std::cout << "Case #" << i_case << ": " << my_result << std::endl;
	}

	return 0;
}
