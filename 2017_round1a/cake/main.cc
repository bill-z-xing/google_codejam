
#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <algorithm>

typedef std::size_t MY_SIZE;

typedef std::vector<std::string> RESULT;
typedef std::vector<std::string> CASE;

namespace CFG
{
	static const bool verbose = true;
	static const bool very_verbose = true;
}


RESULT compute(const CASE & my_case)
{

	return my_case;
}

CASE read_one_case()
{
	CASE my_case;

	MY_SIZE num_cols = 0, num_rows = 0;

	std::cin >> num_rows >> num_cols;

	if (!std::cin.good())
	{
		throw std::runtime_error("IO error while reading case!");
	}

	for (MY_SIZE irow = 0; irow < num_rows; ++irow)
	{
		std::string row;
		std::cin >> row;
		if (!std::cin.good())
		{
			throw std::runtime_error("IO error before reading enough rows!");
		}
		if (row.size() != num_cols)
		{
			throw std::runtime_error("Unexpected row length!");
		}
		my_case.push_back(std::move(row));
	}

	return my_case;
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
			std::cout << "Input Case #" << i_case << ": " << std::endl;
			for (const auto & row : my_case)
			{
				std::cout << row << std::endl;
			}
		}

		RESULT my_result = compute(my_case);
		//std::cout << "Case #" << i_case << ": " << my_result << std::endl;
	}

	return 0;
}
