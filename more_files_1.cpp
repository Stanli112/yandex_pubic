
#include <iostream>
#include <string>
#include <algorithm>
#include <set>
#include <vector>

using namespace std;


void PrintSpacesPositions(string& str) {
    for (auto it = find(str.begin(), str.end(), ' ');
        it != str.end(); it = find(next(it),
            str.end(), ' ')) {
        cout << distance(str.begin(), it) << endl;
    }
}

set<int>::const_iterator FindNearestElement(const set<int>& numbers, int border) {
    // set<int>::const_iterator — тип итераторов для константного множества целых чисел
    auto it = numbers.lower_bound(border);
    if (it == numbers.begin()) {
        return it;
    }

    auto prev_it = prev(it);  // предшественник
    // смотрим кто ближе, того и возвращаем 
    return (it == numbers.end() || border - *prev_it <= *it - border) ? prev_it : it;
}

template <typename RandomIt>
pair<RandomIt, RandomIt> FindStartsWith(RandomIt range_begin, RandomIt range_end, char prefix) {
    // Все строки, начинающиеся с '<prefix>', больше или равны строке "<prefix>"
    auto left = lower_bound(range_begin, range_end, string(1, prefix));

    // Составим следующий в алфавите символ.
    // Не страшно, если prefix = 'z':
    //в этом случае мы получим следующий за 'z' символ в таблице символов
    char next_prefix = static_cast<char>(prefix + 1);

    // Строка "<next_prefix>" в рамках буквенных строк
    // является точной верхней гранью
    // множества строк, начнающихся с '<prefix>'
    auto right = lower_bound(range_begin, range_end, string(1, next_prefix));

    return { left, right };
}

template <typename RandomIt>
pair<RandomIt, RandomIt> FindStartsWith(RandomIt range_begin, RandomIt range_end, string prefix) {
    // Все строки, начинающиеся с prefix, больше или равны строке "<prefix>"
    auto left = lower_bound(range_begin, range_end, prefix);
    // Составим строку, которая в рамках буквенных строк является точной верхней гранью
    // множества строк, начинающихся с prefix
    string upper_bound = prefix;
    ++upper_bound[upper_bound.size() - 1];
    // Первое встреченное слово, не меньшее upper_bound, обязательно является концом полуинтервала
    auto right = lower_bound(range_begin, range_end, upper_bound);
    return { left, right };
}

void StartQuest_1()
{
    string str = "He said: one and one and one is three"s;
    PrintSpacesPositions(str);
}

void StartQuest_2()
{
    set<int> numbers = { 1, 4, 6 };
    cout << *FindNearestElement(numbers, 0) << " " << *FindNearestElement(numbers, 3) << " "
        << *FindNearestElement(numbers, 5) << " " << *FindNearestElement(numbers, 6) << " "
        << *FindNearestElement(numbers, 100) << endl;

    set<int> empty_set;

    cout << (FindNearestElement(empty_set, 8) == end(empty_set)) << endl;
}

void StartQuest_3()
{
    const vector<string> sorted_strings = { "moscow", "murmansk", "vologda" };

    const auto m_result = FindStartsWith(begin(sorted_strings), end(sorted_strings), 'm');
    for (auto it = m_result.first; it != m_result.second; ++it) {
        cout << *it << " ";
    }
    cout << endl;

    const auto p_result = FindStartsWith(begin(sorted_strings), end(sorted_strings), 'p');
    cout << (p_result.first - begin(sorted_strings)) << " " << (p_result.second - begin(sorted_strings)) << endl;

    const auto z_result = FindStartsWith(begin(sorted_strings), end(sorted_strings), 'z');
    cout << (z_result.first - begin(sorted_strings)) << " " << (z_result.second - begin(sorted_strings)) << endl;

}

int main() {
    //StartQuest_1();
    //StartQuest_2();
    StartQuest_3();

    return 0;
}