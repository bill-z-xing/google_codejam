
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <tuple>
#include <algorithm>
#include <cmath>

typedef std::size_t MY_SIZE;


namespace CFG
{
	static const bool verbose = false;
	static const bool very_verbose = false;
}

struct CASE
{
	MY_SIZE num_ingreds = 0;
	MY_SIZE num_packs = 0;
	std::vector<MY_SIZE> requirements; // per ingred
	std::vector< std::vector<MY_SIZE> > packs_per_ingred;
};

typedef std::pair<MY_SIZE, MY_SIZE> PACK_RANGE;

std::pair<MY_SIZE, MY_SIZE>
num_servings_of_pack(const double req, const double pack_weight)
{
	double upper_total = req * 1.1;
	double lower_total = req * 0.9;

	MY_SIZE upper_servings = std::floor(upper_total / pack_weight);
	MY_SIZE lower_servings = std::ceil(lower_total / pack_weight);

	if (upper_servings >= lower_servings && upper_servings > 0)
	{
		return std::make_pair(std::max((MY_SIZE)1,lower_servings), upper_servings);
	}
	else
	{
		return std::make_pair((MY_SIZE)0, (MY_SIZE(0)));
	}
}

struct COMP
{
	typedef std::pair<MY_SIZE, MY_SIZE> T;
	bool operator()(const T & a, const T & b)
	{
		if (a.second != b.second)
		{
			return a.second < b.second;
		}
		return a.first < b.first;
	}
};

MY_SIZE compute(const CASE & my_case)
{
	// Allocate multiset for each ingredient
	std::vector< std::multiset<PACK_RANGE, COMP> > stuff(my_case.num_ingreds);
		// [ingred][num_servings]

	for (MY_SIZE i_ingred = 0; i_ingred < my_case.num_ingreds; ++i_ingred)
	{
		auto & ingred_multiset = stuff[i_ingred];
		const auto req = my_case.requirements.at(i_ingred);
		for (MY_SIZE i_pack = 0; i_pack < my_case.num_packs; ++i_pack)
		{
			const auto pack_weight = my_case.packs_per_ingred.at(i_ingred).at(i_pack);
			const auto num_servings = num_servings_of_pack(req, pack_weight);
			if (num_servings.second > 0)
			{
				ingred_multiset.insert(num_servings);
			}
		}
	}

	// Find common servings

	return 0;
}

CASE read_one_case()
{
	CASE my_case;

	std::cin >> my_case.num_ingreds >> my_case.num_packs;

	if (!std::cin.good())
	{
		throw std::runtime_error("IO error while reading case!");
	}

	if (CFG::very_verbose)
	{
		std::cout << my_case.num_ingreds << " " << my_case.num_packs << std::endl;
	}

	// Requirements
	for (MY_SIZE i_ingred = 0; i_ingred < my_case.num_ingreds; ++i_ingred)
	{
		MY_SIZE req;
		std::cin >> req;
		if (!std::cin.good())
		{
			throw std::runtime_error("IO error before reading enough requirements!");
		}
		my_case.requirements.push_back(req);
		if (CFG::very_verbose)
		{
			std::cout << "Req: " << req << std::endl;
		}
	}

	// Packages
	for (MY_SIZE i_ingred = 0; i_ingred < my_case.num_ingreds; ++i_ingred)
	{
		my_case.packs_per_ingred.emplace_back();
		auto & packs_this_ingred = my_case.packs_per_ingred.back();
		for (MY_SIZE i_pack = 0; i_pack < my_case.num_packs; ++i_pack)
		{
			MY_SIZE pack;
			std::cin >> pack;
			if (!std::cin.good())
			{
				throw std::runtime_error("IO error before reading enough packs!");
			}
			packs_this_ingred.push_back(pack);
		}
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
		{/*
			std::cout << "Input Case #" << i_case << ": " << std::endl;
			for (const auto & row : my_case)
			{
				std::cout << row << std::endl;
			}*/
		}

		auto solution = compute(my_case);
		std::cout << "Case #" << i_case << ": " << solution << std::endl;

	}

	return 0;
}
