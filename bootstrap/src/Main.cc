/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Main
*/

#include "SparseArray.hh"

#include <iostream> // std::cout
#include <any> // std::any
#include <typeinfo> // typeid

void print_any(const std::any& a)
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
    sparse_array<int> array;
    array.insert_at(0, 42);
    print_any(array[0]);
    return 0;
}
