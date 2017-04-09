
#include <iostream>
#include <string>

#include <vector>
#include <queue>
#include <map>
#include <tuple>

#include <exception>
#include <stdexcept>

#include <cassert>

#include <limits>
#include <cmath>

typedef std::size_t MY_SIZE;
typedef std::pair<MY_SIZE, MY_SIZE> ONE_CASE;
typedef std::vector<ONE_CASE> CASE_VEC;
typedef std::vector<bool> MY_BITSET;

namespace CFG
{
	static const bool verbose = false;
}

inline MY_SIZE my_log2(MY_SIZE a)
{
	return (MY_SIZE) (std::log2( (float) a) );
}

CASE_VEC parse_cases()
{
	CASE_VEC all_cases;

	MY_SIZE num_cases = 0;
	std::cin >> num_cases;

	if (!std::cin.good())
	{
		return all_cases;
	}

	all_cases.reserve(num_cases);

	for (;;)
	{
		MY_SIZE n = 0;
		std::cin >> n;

		MY_SIZE k = 0;
		std::cin >> k;

		if (std::cin.good())
		{
			if (CFG::verbose)
			{
				std::cout << "Input: " << n << " " << k << std::endl;
			}

			all_cases.push_back(std::make_pair(n, k));
		}
		else
		{
			break;
		}
	}

	return all_cases;
}


// Bitset implementation.
// Retval: Size, StartIdx, EndIdx
std::tuple<MY_SIZE, MY_SIZE, MY_SIZE> find_largest_hole(const MY_BITSET & stalls)
{
	bool middle_of_a_hole = false;
	MY_SIZE largest_hole_start = std::numeric_limits<MY_SIZE>::max();
	MY_SIZE largest_hole_end = std::numeric_limits<MY_SIZE>::max();
	MY_SIZE largest_hole_size = 0; // Use 0 to denote no holes were found

	MY_SIZE prev_hole_start = std::numeric_limits<MY_SIZE>::max();
	for (MY_SIZE idx = 0; idx <= stalls.size(); ++idx)
	{
		bool occupied = (idx == stalls.size() ? true : stalls[idx]);
		if (!middle_of_a_hole && !occupied)
		{
			// Hole start
			middle_of_a_hole = true;
			prev_hole_start = idx;
		}
		else if (middle_of_a_hole && occupied)
		{
			// Hole end
			middle_of_a_hole = false;
			MY_SIZE prev_hole_end = idx - 1;
			MY_SIZE hole_size = prev_hole_end - prev_hole_start + 1;
			if (hole_size > largest_hole_size)
			{
				largest_hole_size = hole_size;
				largest_hole_start = prev_hole_start;
				largest_hole_end = prev_hole_end;
			}
		}
	}

	return std::make_tuple(largest_hole_size, largest_hole_start, largest_hole_end);
}

// Bitset implementation
// Returns: Max, Min
// Input: n stalls, k persons
std::pair<MY_SIZE, MY_SIZE> compute_bitset(MY_SIZE n, MY_SIZE k)
{
	static MY_BITSET stalls; // Saves multiple allocation & deallocation, trading multi-threading ability.

	stalls.clear();
	stalls.resize(n, false);

	for (MY_SIZE i_person = 0; ; ++i_person)
	{
		// Find hole
		MY_SIZE size, start, end;
		std::tie(size, start, end) = find_largest_hole(stalls);
		if (size == 0)
		{
			throw std::runtime_error("No holes found for unknown reasons! Likely an algorithm mistake.");
		}
		assert(end >= start);
		assert(end - start + 1 == size);

		// Insert person
		MY_SIZE insertion_point = (start + end) / 2;
		assert(stalls.at(insertion_point) == false);
		stalls.at(insertion_point) = true;

		// Last person done, calculate return value and exit.
		if (i_person == k - 1)
		{
			MY_SIZE max = end - insertion_point;
			MY_SIZE min = insertion_point - start;
			assert(max >= min);
			return std::make_pair(max, min);
		}
	}

	//throw std::runtime_error("Unknown error: Shouldn't have exited the for loop.");
	//return std::make_pair(0, 0);
}

inline MY_SIZE hole_size(MY_SIZE start, MY_SIZE end)
{
	if (end < start)
	{
		throw std::runtime_error("Illegal hole: end must not be smaller than start!");
	}
	return (end - start + 1);
}

class HOLE
{
public:
	HOLE() = delete;
	HOLE(MY_SIZE start, MY_SIZE end)
	:
		_start(start),
		_end(end)
	{
		assert(end >= start);
	}

	MY_SIZE size() const
	{
		return hole_size(_start, _end);
	}

	MY_SIZE start() const
	{
		return _start;
	}

	MY_SIZE end() const
	{
		return _end;
	}

private:
	MY_SIZE _start;
	MY_SIZE _end;
};

struct HOLE_PRIORITY_COMPARE
{
	bool operator()(const HOLE & a, const HOLE & b)
	{
		if (a.size() != b.size())
		{
			// If size different, prioritize the larger hole
			return a.size() < b.size();
		}
		else
		{
			// If size same, prioritize the left most hole
			return a.start() > b.start();
		}
	}
};

class COUNTER_BASED_HOLE_MGR
{
public:
	COUNTER_BASED_HOLE_MGR(MY_SIZE num_stalls)
	{
		if (num_stalls < 1)
		{
			throw std::runtime_error("Must have at least 1 stall");
		}
		MY_SIZE init_hole_size = num_stalls;
		_hole_tracker.emplace(init_hole_size, 1);
	}

	auto add_person()
	{
		if (_hole_tracker.empty())
		{
			throw std::runtime_error("Not enough holes left!");
		}

		auto largest_hole_iter = _hole_tracker.end();
		--largest_hole_iter;
		MY_SIZE parent_hole_size = largest_hole_iter->first;
		if (largest_hole_iter->second > 1)
		{
			--(largest_hole_iter->second);
		}
		else
		{
			_hole_tracker.erase(largest_hole_iter);
		}

		// Take the largest hole and break it down

		MY_SIZE hole1_size = (parent_hole_size - 1) / 2;
		MY_SIZE hole2_size = parent_hole_size - 1 - hole1_size;

		{
			// Add hole 1
			auto iter = _hole_tracker.find(hole1_size);
			if (iter == _hole_tracker.end())
			{
				_hole_tracker.emplace(hole1_size, 1);
			}
			else
			{
				++(iter->second);
			}
		}
		{
			// Add hole 2
			auto iter = _hole_tracker.find(hole2_size);
			if (iter == _hole_tracker.end())
			{
				_hole_tracker.emplace(hole2_size, 1);
			}
			else
			{
				++(iter->second);
			}
		}

		assert(hole2_size >= hole1_size);
		return std::make_pair(hole2_size, hole1_size);
	}
private:
	std::map<MY_SIZE, MY_SIZE> _hole_tracker; // (hole_size, num_holes_with_the_size)
};

class HOLE_MGR
{
public:
	HOLE_MGR(MY_SIZE num_stalls)
	{
		if (num_stalls < 1)
		{
			throw std::runtime_error("Must have at least 1 stall");
		}
		_hole_pri_queue.emplace(0, num_stalls - 1);
	}

	auto add_person()
	{
		if (_hole_pri_queue.empty())
		{
			throw std::runtime_error("Not enough holes left!");
		}
		// Take the top hole and pop it from the queue
		auto best_hole = _hole_pri_queue.top();
		_hole_pri_queue.pop();

		// Insert two smaller holes, if any
		MY_SIZE insertion_point = (best_hole.start() + best_hole.end()) / 2;
		if (CFG::verbose)
		{
			std::cout << "Insert=" << insertion_point << " ";
		}

		MY_SIZE hole1_size = 0, hole2_size = 0;

		{
			// Add hole 1 (left)
			MY_SIZE hypothetical_hole1_start = best_hole.start();
			if (insertion_point - hypothetical_hole1_start >= 1 )
			{
				// Room for hole 1 exists
				MY_SIZE hypothetical_hole1_end = insertion_point - 1;
				_hole_pri_queue.emplace(hypothetical_hole1_start, hypothetical_hole1_end);
				hole1_size = hole_size(hypothetical_hole1_start, hypothetical_hole1_end);
				if (CFG::verbose)
				{
					std::cout << "Hole1=" << "[" << hypothetical_hole1_start << ","
						<< hypothetical_hole1_end << "] ";
				}
			}
		}

		{
			// Add hole 2 (right)
			MY_SIZE hypothetical_hole2_end = best_hole.end();
			if (hypothetical_hole2_end - insertion_point >= 1)
			{
				// Room for hole 2 exists
				MY_SIZE hypothetical_hole2_start = insertion_point + 1;
				_hole_pri_queue.emplace(hypothetical_hole2_start, hypothetical_hole2_end);
				hole2_size = hole_size(hypothetical_hole2_start, hypothetical_hole2_end);
				if (CFG::verbose)
				{
					std::cout << "Hole2=" << "[" << hypothetical_hole2_start << ","
						<< hypothetical_hole2_end << "]";
				}
			}
		}
		if (CFG::verbose)
		{
			std::cout << std::endl;
		}

		assert(hole2_size >= hole1_size);
		return std::make_pair(hole2_size, hole1_size);
	}

private:
	std::priority_queue<HOLE, std::vector<HOLE>, HOLE_PRIORITY_COMPARE> _hole_pri_queue;
};


// Compute using priority queue implementation
// Return: final two hole sizes (max, min)
template<class HOLE_MGR_TYPE>
std::pair<MY_SIZE, MY_SIZE> compute_with_hole_mgr(MY_SIZE num_stalls, MY_SIZE num_persons)
{
	HOLE_MGR_TYPE my_hole_mgr(num_stalls);

	for (MY_SIZE i_person = 0; ; ++i_person)
	{
		if (i_person == num_persons - 1)
		{
			// last person to add
			return my_hole_mgr.add_person();
		}
		else
		{
			my_hole_mgr.add_person();
		}
	}
}


void compute_check(MY_SIZE num_stalls, MY_SIZE num_persons)
{
	if (num_stalls == 0)
	{
		throw std::runtime_error("No stalls in the bathroom.");
	}

	if (num_persons == 0)
	{
		throw std::runtime_error("No people entering the bathroom.");
	}

	if (num_persons > num_stalls)
	{
		throw std::runtime_error("More people than stalls!");
	}
}

std::map<MY_SIZE, MY_SIZE> split_all_holes(const std::map<MY_SIZE, MY_SIZE> & hole_table)
{
	std::map<MY_SIZE, MY_SIZE> new_hole_table;
	for (auto iter = hole_table.begin(); iter != hole_table.end(); ++iter)
	{
		MY_SIZE hole_size = iter->first;
		MY_SIZE hole_count = iter->second;

		MY_SIZE split_size_1 = (hole_size - 1) / 2;
		MY_SIZE split_size_2 = hole_size - 1 - split_size_1;

		if (split_size_1 > 0)
		{
			auto hole1_iter = new_hole_table.find(split_size_1);
			if (hole1_iter != new_hole_table.end())
			{
				(hole1_iter->second) += hole_count;
			}
			else
			{
				new_hole_table.emplace(split_size_1, hole_count);
			}
		}

		if (split_size_2 > 0)
		{
			auto hole2_iter = new_hole_table.find(split_size_2);
			if (hole2_iter != new_hole_table.end())
			{
				(hole2_iter->second) += hole_count;
			}
			else
			{
				new_hole_table.emplace(split_size_2, hole_count);
			}
		}
	}
	return new_hole_table;
}

std::pair<MY_SIZE, MY_SIZE>
compute_smart(MY_SIZE num_stalls, MY_SIZE num_persons)
{
	if (CFG::verbose)
	{
		std::cout << "Solving: " << num_stalls << " " << num_persons << std::endl;
	}
	std::map<MY_SIZE, MY_SIZE> hole_table;
	hole_table.emplace(num_stalls, 1);

	MY_SIZE target_depth = my_log2(num_persons);
	MY_SIZE nodes_added = 1;
	MY_SIZE tree_depth = 0;
	MY_SIZE last_depth_nodes = 1;
	while (nodes_added < num_persons)
	{
		hole_table = split_all_holes(hole_table);
		nodes_added += (last_depth_nodes = last_depth_nodes * 2);
		++tree_depth;
	}

	if (CFG::verbose)
	{
		std::cout << "At Depth: " << tree_depth << " " << target_depth << std::endl;
		if (tree_depth != target_depth)
		{
			std::cout << "LOL it's floating point rounding error again...\n";
		}

		for (auto & p : hole_table)
		{
			std::cout << "(" << p.first << "," << p.second << "), ";
		}
		std::cout << std::endl;
	}

	assert(num_persons > (((MY_SIZE)1 << tree_depth) - 1));
	MY_SIZE nth_largest_hole_on_final_level = num_persons - (((MY_SIZE)1 << tree_depth) - 1);
	if (CFG::verbose)
	{
		std::cout << "Finding nth largest on the leaf level: " << nth_largest_hole_on_final_level << std::endl;
	}

	MY_SIZE hole_counter = 0;
	for (auto riter = hole_table.rbegin(); riter != hole_table.rend(); ++riter)
	{
		hole_counter += riter->second;
		if (hole_counter >= nth_largest_hole_on_final_level)
		{
			MY_SIZE hole_size = riter->first;
			MY_SIZE split1 = (hole_size - 1) / 2;
			MY_SIZE split2 = hole_size - 1 - split1;
			return std::make_pair(split2, split1);
		}
	}


	throw std::runtime_error("Couldn't find hole!");
}

auto compute(MY_SIZE num_stalls, MY_SIZE num_persons)
{
	compute_check(num_stalls, num_persons);
	//return compute_with_hole_mgr<HOLE_MGR>(num_stalls, num_persons);
	//return compute_with_hole_mgr<COUNTER_BASED_HOLE_MGR>(num_stalls, num_persons);
	return compute_smart(num_stalls, num_persons);
}

int main()
{
	auto cases = parse_cases();

	MY_SIZE i_case = 1;
	for (auto & my_case : cases)
	{
		MY_SIZE max, min;
		std::tie(max, min) = compute(my_case.first, my_case.second);
		std::cout << "Case #" << i_case << ": "
			<< max << " " << min
			<< std::endl;
		++i_case;
	}
	return 0;
}
