
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <tuple>
#include <algorithm>

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
	std::vector<MY_SIZE> requirements;
	std::vector< std::vector<MY_SIZE> > packs_per_ingred;
};


MY_SIZE compute(const CASE & my_case)
{

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
