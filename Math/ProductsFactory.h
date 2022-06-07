#pragma once

#include "Products.h"
#include <list>
#include <vector>

class ProductsFactory
{
public:
    Products& Create(std::vector<Expression*> sums)
    {
        mem.emplace_front(sums);
        return mem.front();
    }
    std::list<Products> mem;
};