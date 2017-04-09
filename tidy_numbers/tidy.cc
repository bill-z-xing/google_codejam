#include <cstdint>
#include <vector>
#include <algorithm>
#include <iostream>

typedef uint8_t DIGIT;
typedef std::vector<DIGIT> DIGIT_GRID;
typedef std::vector<DIGIT_GRID> DIGIT_GRID_VEC;

typedef std::size_t MY_SIZE;

namespace CFG
{
	static const bool verbose = false;
};

DIGIT_GRID_VEC
read_test_cases()
{
	DIGIT_GRID_VEC all_cases;

	MY_SIZE num_cases = 0;
	std::cin >> num_cases;

	if (!std::cin.good())
	{
		return all_cases;
	}

	all_cases.reserve(num_cases);

	std::string big_int_buffer;
	DIGIT_GRID digit_grid_buffer;
	for (;;)
	{
		digit_grid_buffer.clear();

		std::cin >> big_int_buffer;

		if (std::cin.good())
		{
			for (char c : big_int_buffer)
			{
				if (c >= '0' && c <= '9')
				{
					digit_grid_buffer.push_back(c - '0');
				}
			}
			all_cases.push_back(std::move(digit_grid_buffer));

		}
		else
		{
			break;
		}
	}

	return all_cases;
}

void tidify(DIGIT_GRID & digit_grid)
{
	if (digit_grid.size() < 2)
	{
		return; // Already tidy
	}


	bool first = true;
	for (auto it = digit_grid.rbegin(); it < digit_grid.rend(); ++it)
	{
		if (first)
		{
			first = false;
			continue;
		}

		if (*it > *(it-1))
		{
			--(*it);
			std::fill(digit_grid.rbegin(), it, 9);
		}
	}
}

int main()
{
	auto digit_grid_vec = read_test_cases();

	if (CFG::verbose)
	{
		for (auto & digit_grid : digit_grid_vec)
		{
			std::cout << "Input: " << std::endl;
			for (DIGIT d : digit_grid)
			{
				std::cout << (unsigned short) d;
			}
			std::cout << std::endl;
		}
	}

	MY_SIZE i_case = 1;
	for (auto & digit_grid : digit_grid_vec)
	{
		tidify(digit_grid);

		std::cout << "Case #" << i_case << ": ";
		bool seen_non_zero = false;
		for (DIGIT d : digit_grid)
		{
			if (seen_non_zero == false && d == 0)
			{
				continue;
			}

			seen_non_zero = true;
			std::cout << (unsigned short) d;
		}
		if (seen_non_zero == false)
		{
			std::cout << 0;
		}
		std::cout << std::endl;

		++i_case;
	}

	return 0;
}
