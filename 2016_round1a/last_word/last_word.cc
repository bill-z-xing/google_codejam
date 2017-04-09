
#include <iostream>
#include <string>
#include <deque>

typedef std::size_t MY_SIZE;

namespace CFG
{
	static const bool verbose = false;
}

std::string compute(const std::string & input)
{
	std::deque<char> workspace;

	for (char c : input)
	{
		if (workspace.empty())
		{
			workspace.push_back(c);
		}
		else if (c >= workspace.front())
		{
			workspace.push_front(c);
		}
		else
		{
			workspace.push_back(c);
		}
	}

	// Translate deque to string
	std::string final_result;
	final_result.assign(workspace.cbegin(), workspace.cend());


	return final_result;
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
