
#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <algorithm>

typedef std::size_t MY_SIZE;
typedef std::uint16_t HEIGHT;
typedef std::vector<HEIGHT> HEIGHT_LIST;
const HEIGHT invalid_height = 0;

namespace CFG
{
	static const bool verbose = true;
	static const bool very_verbose = false;
}



class GRID
{
public:
	typedef HEIGHT T;

	typedef std::vector<T> LIST;
	typedef std::vector<T *> LIST_PTR;

	friend bool operator==(const LIST_PTR & a, const LIST & b);

	friend bool operator==(const LIST & a, const LIST_PTR & b);

	GRID() = delete;

	GRID(MY_SIZE n, T initial)
	:
		_rows(n, LIST(n, initial)),
		_rows_assigned(n, false),
		_cols_assigned(n, false)
	{
		if (n < 1) throw std::runtime_error("Can't initialize an empty grid");

		for (MY_SIZE irow = 0; irow < n; ++irow)
		{
			_rows_ptrs.emplace_back();
			auto & row_ptrs = _rows_ptrs.back();
			for (MY_SIZE icol = 0; icol < n; ++icol)
			{
				row_ptrs.push_back(&(_rows.at(irow).at(icol)));
			}
		}

		for (MY_SIZE icol = 0; icol < n; ++icol)
		{
			_cols_ptrs.emplace_back();
			auto & col_ptrs = _cols_ptrs.back();
			for (MY_SIZE irow = 0; irow < n; ++irow)
			{
				col_ptrs.push_back(&(_rows.at(irow).at(icol)));
			}
		}
	}

	const LIST_PTR & get_row(MY_SIZE i_row)
	{
		return _rows_ptrs.at(i_row);
	}

	const LIST_PTR & get_col(MY_SIZE i_col)
	{
		return _cols_ptrs.at(i_col);
	}

	void assign_row(MY_SIZE i_row, const std::vector<T> & new_row)
	{
		if (_rows_assigned[i_row])
		{
			throw std::runtime_error("Row already assigned, can't override!");
		}
		const auto & row_ptrs = get_row(i_row);
		MY_SIZE ielem = 0;
		for (auto row_elem_ptr : row_ptrs)
		{
			if (*row_elem_ptr != invalid_height && *row_elem_ptr != new_row.at(ielem))
			{
				throw std::runtime_error("Can't overwrite valid data!");
			}
			*row_elem_ptr = new_row.at(ielem);
			++ielem;
		}
		_rows_assigned[i_row] = true;
	}

	void assign_col(MY_SIZE i_col, const std::vector<T> & new_col)
	{
		if (_cols_assigned[i_col])
		{
			throw std::runtime_error("Col already assigned, can't override!");
		}
		const auto & col_ptrs = get_col(i_col);
		MY_SIZE ielem = 0;
		for (auto col_elem_ptr : col_ptrs)
		{
			if (*col_elem_ptr != invalid_height && *col_elem_ptr != new_col.at(ielem))
			{
				throw std::runtime_error("Can't overwrite valid data!");
			}
			*col_elem_ptr = new_col.at(ielem);
			++ielem;
		}
		_cols_assigned[i_col] = true;
	}

	bool symmetrical() const;

	std::pair<std::vector<MY_SIZE>, std::vector<MY_SIZE>>
	find_compatible_list(const LIST & b) const
	{
		std::vector<MY_SIZE> compatible_rows;
		std::vector<MY_SIZE> compatible_cols;

		for (MY_SIZE irow = 0; irow < _rows_ptrs.size(); ++irow)
		{
			if (_rows_assigned[irow])
			{
				continue;
			}
			const auto & row_ptrs = const_cast<GRID *>(this)->get_row(irow);
			if (is_compatible(row_ptrs, b))
			{
				compatible_rows.push_back(irow);
			}
		}

		for (MY_SIZE icol = 0; icol < _cols_ptrs.size(); ++icol)
		{
			if (_cols_assigned[icol])
			{
				continue;
			}
			const auto & col_ptrs = const_cast<GRID *>(this)->get_col(icol);
			if (is_compatible(col_ptrs, b))
			{
				compatible_cols.push_back(icol);
			}
		}

		return std::make_pair(std::move(compatible_rows), std::move(compatible_cols));
	}

	std::tuple<bool, bool, MY_SIZE> // (found, is_col, row/col idx)
	find_unique_compatible_list(const LIST & b) const;

	bool is_compatible(const GRID::LIST_PTR & a, const GRID::LIST & b) const
	{
		if (a.size() != b.size())
		{
			throw std::runtime_error("Comparing list compatibility of different sizes!");
		}

		bool any_valid_in_a = false;
		for (MY_SIZE i = 0; i < a.size(); ++i)
		{
			if (*(a[i]) != b[i] && *(a[i]) != invalid_height)
			{
				return false;
			}

			if (*(a[i]) != invalid_height)
			{
				any_valid_in_a = true;
			}

			if (b[i] == invalid_height)
			{
				// Don't accept invalid list b
				throw std::runtime_error("Can't deal with invalid comparee list");
			}
		}
		return any_valid_in_a; // Make an invalid list A incompatible by definition
	}

	std::pair<bool, MY_SIZE> // is_col, idx
	which_missing_list() const
	{
		std::pair<bool, MY_SIZE> p;
		MY_SIZE count = 0;
		for (MY_SIZE irow = 0; irow < _rows_assigned.size(); ++irow)
		{
			if (_rows_assigned[irow] == false)
			{
				p = std::make_pair(false, irow);
				++count;
			}
		}
		for (MY_SIZE icol = 0; icol < _cols_assigned.size(); ++icol)
		{
			if (_cols_assigned[icol] == false)
			{
				p = std::make_pair(true, icol);
				++count;
			}
		}
		if (count != 1)
		{
			throw std::runtime_error("Can't find unique missing list!!");
		}
		return p;
	}

	friend std::ostream & operator<< (std::ostream & os, const GRID & g);



private:
	std::vector< LIST > _rows;
	std::vector< LIST_PTR > _rows_ptrs;
	std::vector< LIST_PTR > _cols_ptrs;
	std::vector<bool> _rows_assigned;
	std::vector<bool> _cols_assigned;
};



inline bool operator==(const GRID::LIST_PTR & a, const GRID::LIST & b)
{
	return std::equal(
		a.begin(), a.end(), b.begin(),
		[](GRID::T* a, const GRID::T & b)
		{
			return *a == b;
		});
}

inline bool operator==(const GRID::LIST & a, const GRID::LIST_PTR & b)
{
	return (b == a);
}


std::ostream & operator<<(std::ostream & os, const HEIGHT_LIST & l)
{
	os << "[";
	bool first = true;
	for (const auto & elem : l)
	{
		if (!first)
		{
			os << ", ";
		}
		os << elem;
		first = false;
	}
	os << "]";
	return os;
}


inline std::ostream & operator<< (std::ostream & os, const GRID & g)
{
	for (const std::vector<HEIGHT> & row : g._rows)
	{
		os << row << std::endl;
	}
	return os;
}

bool GRID::symmetrical() const
{
	for (MY_SIZE irow = 0; irow < _rows.size(); ++irow)
	{
		if (!(const_cast<GRID *>(this)->get_col(irow) == _rows[irow]))
		{
			return false;
		}
	}
	return true;
}

std::tuple<bool, bool, MY_SIZE> // (found, is_col, row/col idx)
GRID::find_unique_compatible_list(const GRID::LIST & b) const
{
	std::vector<MY_SIZE> rows, cols;
	std::tie(rows, cols) = find_compatible_list(b);

	if (rows.size() + cols.size() == 1)
	{
		if (rows.size() == 1)
		{
			return std::make_tuple(true, false, rows[0]);
		}
		else
		{
			return std::make_tuple(true, true, cols[0]);
		}
	}
	else if (rows.size() == 1 && cols.size() == 1 &&
		rows[0] == cols[0] &&
		_rows[rows[0]] == const_cast<GRID *>(this)->get_col(cols[0]))
	{
		return std::make_tuple(true, false, rows[0]);
	}
	else
	{
		return std::make_tuple(false, false, 0); // only first "false" meaningful
	}
}


const GRID::LIST_PTR & compute(std::vector<HEIGHT_LIST> & all_lists, GRID & grid)
{
	if (all_lists.size() == 0)
	{
		throw std::runtime_error("Empty List!");
	}

	// Lexicographically sort the lists
	std::sort(
		all_lists.begin(), all_lists.end(),
		[](const HEIGHT_LIST & a, const HEIGHT_LIST & b)
		{
			return std::lexicographical_compare(
				a.cbegin(), a.cend(),
				b.cbegin(), b.cend());
		});

	if (CFG::verbose)
	{
		std::cout << "Sorted:" << std::endl;
		MY_SIZE ilist = 0;
		for (auto & l : all_lists)
		{
			std::cout << "List " << ilist << ": " << l << std::endl;
			++ilist;
		}
	}

	// Keep track of which list in all_lists are already added
	std::vector<bool> list_added(all_lists.size(), false);

	// Add the first list to first row of the grid unconditionally
	grid.assign_row(0, all_lists[0]);
	list_added[0] = true;
	if (CFG::verbose)
	{
		std::cout << "Added List " << 0 << " to Row " << 1 << ": " << all_lists[0] << std::endl;
	}

	// Each round, add all lists that can find with a compatible row or column
	// to the grid.
	//
	// Quit the outer for loop when nothing could be done. Then assert
	// all lists were added
	//

	for (MY_SIZE round = 1;; ++round) // Per round
	{
		if (CFG::verbose)
		{
			std::cout << "Start round " << round << "!" << std::endl;
		}
		bool done_any_work_this_round = false;

		for (MY_SIZE ilist = 1; ilist < all_lists.size(); ++ilist)
			// Per list
		{
			if (list_added[ilist])
			{
				continue;
			}

			auto & list = all_lists[ilist];

			if (CFG::verbose)
			{
				std::cout << "Trying List " << ilist << " " << list << std::endl;
			}


			bool found_unique;
			bool is_col;
			MY_SIZE idx;
			std::tie(found_unique, is_col, idx) = grid.find_unique_compatible_list(list);

			if (CFG::verbose)
			{
				std::cout << "\tU=" << found_unique << " C=" << is_col << " I=" << idx << std::endl;
			}

			if (found_unique)
			{
				done_any_work_this_round = true;
				list_added[ilist] = true;

				if (is_col)
				{
					grid.assign_col(idx, list);
					if (CFG::verbose)
					{
						std::cout << "Added List " << ilist << " to Col " << idx << ": " << list << std::endl;
					}
				}
				else
				{
					if (CFG::verbose)
					{
						std::cout << "Added List " << ilist << " to  Row " << idx << ": " << list << std::endl;
					}
					grid.assign_row(idx, list);
				}
			}

		} // End per list

		if (!done_any_work_this_round)
		{
			break;
		}
	} // End per round

	if (!std::all_of(list_added.begin(), list_added.end(), [](bool added){return added;}))
	{
		std::cout << grid;
		throw std::runtime_error("Couldn't continue before all lists were added!");
	}

	bool col;
	MY_SIZE idx;
	std::tie(col, idx) = grid.which_missing_list();
	if (CFG::verbose)
	{
		std::cout << "Found missing list " << col << " " << idx << std::endl;
	}

	const GRID::LIST_PTR * ptr_to_list_ptr = nullptr;
	if (col)
	{
		ptr_to_list_ptr = &(grid.get_col(idx));
	}
	else
	{
		ptr_to_list_ptr = &(grid.get_row(idx));
	}

	if (CFG::verbose)
	{
		std::cout << "Missing list: ";

		for (auto height_ptr : *ptr_to_list_ptr)
		{
			std::cout << *height_ptr << " ";
		}

		std::cout << std::endl;
	}

	return *ptr_to_list_ptr;
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

		GRID grid(grid_size, invalid_height);
		const auto & missing_list = compute(list_of_lists, grid);

		std::cout << "Case #" << i_case << ":";
		for (auto height_ptr : missing_list)
		{
			std::cout << " " << *height_ptr;
		}
		std::cout << std::endl;
	}

	return 0;
}
