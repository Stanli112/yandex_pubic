////#include <algorithm>
////#include <iostream>
////#include <set>
////#include <string>
////#include <vector>
////
////using namespace std;
////
////template <typename It>
////void PrintRange(It range_begin, It range_end) {
////    for (auto it = range_begin; it != range_end; ++it) {
////        cout << *it << " "s;
////    }
////    cout << endl;
////}
////
////template <typename It>
////auto MakeSet(It range_begin, It range_end) {
////    return set(range_begin, range_end);
////}
////
////template <typename It>
////auto MakeVector(It range_begin, It range_end) {
////    return vector(range_begin, range_end);
////}
////
////template <typename Container>
////void EraseAndPrint(Container& container, int position, int p1, int p2) {
////    auto it_to_erased = container.erase(container.begin() + position);
////    //PrintRange(container.begin(), it_to_erased);
////    //PrintRange(it_to_erased, container.end());    
////    PrintRange(container.begin(), container.end());
////    container.erase(container.begin() + p1, container.begin() + p2);
////    PrintRange(container.begin(), container.end());
////}
////
////int main() {
////    vector<string> langs = { "Python"s, "Java"s, "C#"s, "Ruby"s, "C++"s };
////    //EraseAndPrint(langs, find(langs.begin(), langs.end(), "C++"));
////    //EraseAndPrint(langs, langs.begin());
////    //EraseAndPrint(langs, 2);
////    EraseAndPrint(langs, 0, 0, 2);
////}
//
//
////#include <iostream>
////
////using namespace std;
////
////int Fibonacci(int n) {
////    if (n == 0)
////        return 0;
////    if (n == 1)
////        return 1;
////    return Fibonacci(n - 1) + Fibonacci(n - 2);
////}
////
////int IsPowOfTwo(int n) {
////
////    if (n <= 0)
////        return 0;
////    if (n == 1)
////        return 1;
////    if (n % 2 == 0) {
////        return IsPowOfTwo(n / 2);
////    }
////    return 0;
////}
////
////int main() {
////    cout << Fibonacci(6) << endl;
////
////    int result = IsPowOfTwo(1024);
////    cout << result << endl;
////
////    return 0;
////}
//
//// Ханойские башни
//#include <iostream>
//
//using namespace std;
//
////quantity-число колец, from-начальное положение колец(1-3),to-конечное положение колец(1-3)
//                              //buf_peg - промежуточный колышек(1-3)
//void hanoi_towers(int quantity, int from, int to, int buf_peg)  
//{                               
//    if (quantity != 0)
//    {
//        hanoi_towers(quantity - 1, from, buf_peg, to);
//
//        cout << from << " -> " << to << endl;
//
//        hanoi_towers(quantity - 1, buf_peg, to, from);
//    }
//}
//
//int main()
//{
//    setlocale(LC_ALL, "rus");
//    int start_peg = 1, destination_peg = 3, buffer_peg = 2, plate_quantity;
//    cout << "Номер первого столбика: " << start_peg << endl;
//    cout << "Номер конечного столбика: " << destination_peg <<endl;
//    cout << "Номер промежуточного столбика: " << buffer_peg << endl;
//    cout << "Количество дисков:" << endl;
//    cin >> plate_quantity;
//
//    hanoi_towers(plate_quantity, start_peg, destination_peg, buffer_peg);
//    return 0;
//}

//template <typename RandomIt>
//void MergeSort(RandomIt range_begin, RandomIt range_end) {
//    {
//
//        auto n = std::distance(range_begin, range_end);
//
//        if (1 < n)
//        {
//            {
//                RandomIt l_from = range_begin;
//                RandomIt l_to = l_from;
//                std::advance(l_to, n / 2);
//                MergeSort(l_from, l_to);
//            }
//
//            {
//                RandomIt r_from = range_begin;
//                std::advance(r_from, n / 2);
//                RandomIt r_to = r_from;
//                std::advance(r_to, n - (n / 2));
//                MergeSort(r_from, r_to);
//            }
//
//            auto tmp_array = make_unique<typename RandomIt::value_type[]>(n);
//            RandomIt l_iter = range_begin;
//            RandomIt l_end = l_iter;
//            advance(l_end, n / 2);
//            RandomIt r_iter = l_end;
//            RandomIt& r_end = range_end;
//
//            auto tmp_iter = tmp_array.get();
//
//            while (l_iter != l_end || r_iter != r_end)
//            {
//                if (*l_iter < *r_iter)
//                {
//                    *tmp_iter = std::move(*l_iter);
//                    ++l_iter;
//                    ++tmp_iter;
//                }
//                else
//                {
//                    *tmp_iter = std::move(*r_iter);
//                    ++r_iter;
//                    ++tmp_iter;
//                }
//
//                if (l_iter == l_end)
//                {
//                    copy(
//                        make_move_iterator(r_iter),
//                        make_move_iterator(r_end),
//                        tmp_iter
//                    );
//
//                    break;
//                }
//
//                if (r_iter == r_end)
//                {
//                    copy(
//                        make_move_iterator(l_iter),
//                        make_move_iterator(l_end),
//                        tmp_iter
//                    );
//
//                    break;
//                }
//            }
//
//            copy(
//                make_move_iterator(tmp_array.get()),
//                make_move_iterator(&tmp_array[n]),
//                range_begin
//            );
//        }
//    }
//
//}
