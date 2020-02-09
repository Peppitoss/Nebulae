#include"VectorHelper.h"

template <typename  T> bool RemoveItemFromVector(std::vector<T>& v, T item)
{
	typename std::vector<T>::iterator it = std::find(v.begin(), v.end(), item);
	if (it != v.end())
	{
		using std::swap;
		swap(*it, v.back());
		v.pop_back();
		return true;
	}
	return false;
}

template<typename  T> void RemoveItemsFromVector(std::vector<T>& vec, std::vector<int> remaining)
{
	std::vector<T> copyVec = vec;
	vec.clear();

	for (int i = 0; i < remaining.size(); i++)
		vec.push_back(copyVec[remaining[i]]);
}

