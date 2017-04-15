
#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <algorithm>

typedef std::size_t MY_SIZE;


typedef std::vector<std::string> CASE;
typedef CASE RESULT;

namespace CFG
{
	static const bool verbose = false;
	static const bool very_verbose = false;
}


void compute(CASE & my_case)
{
	MY_SIZE num_rows = my_case.size();

	std::string * first_meaningful_row = nullptr;
	std::string * prev_meaningful_row = nullptr;

	for (MY_SIZE irow = 0; irow < num_rows; ++irow)
	{
		auto & row = my_case[irow];
		MY_SIZE num_cols = row.size();
		char last_char = '?';
		char first_meaningful_char = '?';
		for (MY_SIZE icol = 0; icol < num_cols; ++icol)
		{
			char & my_char = row[icol];
			if (my_char == '?' )
			{
				if (last_char != '?')
				{
					my_char = last_char;
				}
			}
			else
			{
				last_char = my_char;
				if (first_meaningful_char == '?')
				{
					first_meaningful_char = my_char;
				}
			}
		}

		if (first_meaningful_char != '?')
		{
			for (char & c : row)
			{
				if (c != '?')
				{
					break;
				}
				c = first_meaningful_char;
			}

			if (first_meaningful_row == nullptr)
			{
				first_meaningful_row = &row;
			}
			prev_meaningful_row = &row;
		}
		else
		{
			// Totally empty
			if (prev_meaningful_row != nullptr)
			{
				row = *prev_meaningful_row;
			}
		}
	}

	for (auto & row : my_case)
	{
		if (row.at(0) == '?')
		{
			row = *first_meaningful_row;
		}
		else
		{
			break;
		}
	}


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

		compute(my_case);
		std::cout << "Case #" << i_case << ": " << std::endl;
		for (const auto & row : my_case)
		{
			std::cout << row << std::endl;
		}
	}

	return 0;
}
