
#include <vector>

#include <algorithm>

#include <cassert>

#include <iostream>
#include <string>

#include <exception>
#include <stdexcept>

typedef std::vector<bool> MY_BITSET;
typedef std::size_t MY_SIZE;
typedef std::vector< std::pair<MY_BITSET, MY_SIZE> > TEST_CASE_VEC;

const int NO_SOLUTION = -1;

namespace CFG
{
	static bool verbose = false;
}

inline bool my_xor(bool a, bool b)
{
	return a ^ b;
}

int solve(
	const MY_BITSET & pancakes, // true for blank, false for smiley
	MY_SIZE k)
{
	if (k == 0 || k > pancakes.size())
	{
		return std::all_of(pancakes.cbegin(), pancakes.cend(), [](bool b){ return b == false; })
			? 0 : NO_SOLUTION;
	}

	MY_SIZE num_positions = pancakes.size() + 1 - k;

	MY_BITSET solutions(num_positions); // true for flip, false for don't flip

	bool cache = false;
	MY_SIZE cache_size = 0;
	const MY_SIZE max_cache_size = k - 1;

	int sum = 0;

	for (MY_SIZE i_position = 0; i_position < num_positions; ++i_position)
	{
		bool pancake = pancakes[i_position];
		solutions[i_position] = my_xor(pancake, cache);
		if (solutions[i_position])
		{
			++sum;
		}

		// Update cache
		// Push back
		cache = my_xor(cache, solutions[i_position]);
		++cache_size;
		if (cache_size > max_cache_size)
		{
			// Pop front
			cache = my_xor(cache, solutions[i_position - max_cache_size]);
			--cache_size;
		}
	}

	// Now conditions for first num_positions pancakes are satisfied.
	// Verify if the pancakes left are satisfied yet.

	if (CFG::verbose)
	{
		std::cout << "solution: ";
		for (bool b : solutions)
		{
			std::cout << (b ? '-' : '+');
		}
		std::cout << std::endl;
	}

	bool satisfied = true;
	for (MY_SIZE i_pancake = num_positions; i_pancake < pancakes.size(); ++i_pancake)
	{
		if (CFG::verbose) {
			std::cout << "verify: " << i_pancake << std::endl;
		}

		assert(cache_size != 0);

		bool pancake = pancakes[i_pancake];
		if (pancake == cache)
		{

		}
		else
		{
			satisfied = false;
		}

		//MY_SIZE target_cache_size = pancakes.size() - i_pancake - 1;
		if (i_pancake >= max_cache_size)
		{
			// Start depop the cache
			cache = my_xor(cache, solutions[i_pancake - max_cache_size]);
			--cache_size;

			if (CFG::verbose) {
				std::cout << "drop cache: " << cache_size << std::endl;
			}
		}

	}

	assert(cache_size == 0);

	//satisfied = true;

	if (satisfied)
	{
		return sum;
	}
	else
	{
		return NO_SOLUTION;
	}
}

MY_BITSET string_to_pancake(const std::string & pancake_string)
{
	MY_BITSET pancake;
	pancake.reserve(pancake_string.size());
	for (char c : pancake_string)
	{
		if (c == '+')
		{
			pancake.push_back(false);
		}
		else if (c == '-')
		{
			pancake.push_back(true);
		}
	}
	return pancake;
}

TEST_CASE_VEC
read_pancakes()
{
	TEST_CASE_VEC all_cases;

	MY_SIZE num_cases = 0;
	std::cin >> num_cases;

	if (!std::cin.good())
	{
		return all_cases;
	}

	all_cases.reserve(num_cases);

	for (;;)
	{
		std::string pancake_string;
		std::cin >> pancake_string;
		MY_BITSET pancake = string_to_pancake(pancake_string);

		MY_SIZE k = 0;
		std::cin >> k;



		if (std::cin.good())
		{
			if (CFG::verbose)
			{
				for (bool b : pancake)
				{
					std::cout << (b ? '-' : '+');
				}
				std::cout << " " << k << std::endl;
			}
			all_cases.push_back(std::make_pair(std::move(pancake), k));
		}
		else
		{
			break;
		}
	}

	return all_cases;
}

int main()
{
	TEST_CASE_VEC all_cases = read_pancakes();
	MY_SIZE i_case = 1;
	for (auto & p : all_cases)
	{
		int solution = solve(p.first, p.second);
		std::cout << "Case #" << i_case << ": " <<
			((solution == NO_SOLUTION) ? std::string("IMPOSSIBLE") : std::to_string(solution))
			<< std::endl;

		++i_case;
	}

	return 0;
}
