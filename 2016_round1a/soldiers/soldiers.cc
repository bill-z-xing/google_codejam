
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

typedef std::size_t MY_SIZE;
typedef std::uint16_t HEIGHT;
typedef std::vector<HEIGHT> HEIGHT_LIST;

namespace CFG
{
	static const bool verbose = true;
	static const bool very_verbose = false;
}

HEIGHT_LIST compute(std::vector<HEIGHT_LIST> & all_lists)
{
	std::sort(
		all_lists.begin(), all_lists.end(),
		[](const HEIGHT_LIST & a, const HEIGHT_LIST & b)
		{
			return std::lexicographical_compare(
				a.cbegin(), a.cend(),
				b.cbegin(), b.cend());
		});
	return (HEIGHT_LIST());
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

		// Read grid size
		MY_SIZE grid_size = 0;
		std::cin >> grid_size;
		if (CFG::very_verbose)
		{
			std::cout << "Grid: " << grid_size << std::endl;
		}

		if (!std::cin.good())
		{
			throw std::runtime_error("Can't read grid size");;
		}

		std::vector< std::vector<HEIGHT> > list_of_lists;

		// Read lists
		const MY_SIZE num_lists = grid_size * 2 - 1;
		for (MY_SIZE i_list = 0; i_list < num_lists; ++i_list)
		{
			list_of_lists.emplace_back();
			auto & list = list_of_lists.back();
			HEIGHT prev_height = 0;
			for (MY_SIZE i_elem = 0; i_elem < grid_size; ++i_elem)
			{
				HEIGHT height = 0;
				std::cin >> height;
				if (CFG::very_verbose)
				{
					std::cout << "Height: " << height << std::endl;
				}
				if (!std::cin.good())
				{
					throw std::runtime_error("Can't read height");
				}
				if (height <= prev_height)
				{
					throw std::runtime_error("List not in ascending order");
				}
				prev_height = height;
				list.push_back(height);
			}
		}

		auto missing_list = compute(list_of_lists);

		std::cout << "Case #" << i_case << ":";
		for (auto height : missing_list)
		{
			std::cout << " " << height;
		}
		std::cout << std::endl;
	}

	return 0;
}
