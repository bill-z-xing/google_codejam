
#include <iostream>
#include <string>

typedef std::size_t MY_SIZE;

namespace CFG
{
	static const bool verbose = true;
}

std::string compute(const std::string & input)
{
	return std::string("");
}

int main()
{

	MY_SIZE num_cases = 0;
	std::cin >> num_cases;

	if (!std::cin.good())
	{
		return 0;
	}

	for (MY_SIZE i = 1; ; ++i)
	{
		// Read input
		std::string word;
		std::cin >> word;

		if (std::cin.good())
		{
			if (CFG::verbose)
			{
				std::cout << "Input: " << word << " " << std::endl;
			}

			// Process input
			auto result = compute(word);

			// Print output
			std::cout << "Case: #" << i << ": " << result << std::endl;

		}
		else
		{
			break;
		}
	}



	return 0;
}
