/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Main
*/

#include "SparseArray.hh"
#include "Zipper.hh"

#include <iostream> // std::cout
#include <any> // std::any
#include <typeinfo> // typeid

// template<typename T>
// void print_any(const std::optional<T> &a)
// {
    // if (a.has_value())
    // {
        // std::cout << a.value() << std::endl;
    // }
    // else
    // {
        // std::cout << "No value" << std::endl;
    // }
// }

void print_any(const std::any &a)
{
    if (a.type() == typeid(int))
    {
        std::cout << std::any_cast<int>(a) << std::endl;
    }
    else if (a.type() == typeid(double))
    {
        std::cout << std::any_cast<double>(a) << std::endl;
    }
    else if (a.type() == typeid(std::string))
    {
        std::cout << std::any_cast<std::string>(a) << std::endl;
    }
    else
    {
        std::cout << "Type non supporté" << std::endl;
    }
}

int main()
{
    sparse_array array;
    array.insert_at(0, 42);

    sparse_array<std::string> array2;
    array2.insert_at(0, "Hello");

    sparse_array<std::any> array3;

    array3.insert_at(0, 42);

    std::cout << array.size() << std::endl;
    std::cout << array2.size() << std::endl;
    std::cout << array3.size() << std::endl;

    zipper_iterator<decltype(array)> iter_begin(std::make_tuple(array.begin()), array.size());
    zipper_iterator<decltype(array)> iter_end(std::make_tuple(array.end()), array.size());

    for (auto it = iter_begin; it != iter_end; it++)
    {
        print_any(std::get<0>(*it));
    }

}
