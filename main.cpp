#include <iostream>
#include <forward_list>
import MyForward_List;

int main()
{
	std::forward_list<int> c(1, 1);
	MyForward_List list(3, 2);
	list.push_front(4);
	list.push_front(5);
	list.pop_front();
	list.emplace_front(7);
	list.push_front(8);

	list.erase_after(std::next(list.begin(), 3));

	list.emplace_front(7);
	list.emplace_after(list.begin(), 9);

	std::cout << sizeof(MyForward_List<int>);

	for (int el : list)
		std::cout << el << ' ';
	
	std::cout << std::distance(list.begin(), list.end());
}