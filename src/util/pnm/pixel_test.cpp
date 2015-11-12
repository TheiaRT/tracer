#include <iostream>
#include <cassert>

#include "pixel.h"


int main()
{
    std::string json = "[1,2,3]";
    pixel_t from_json;
    assert(true == pixel_t::from_json(json, from_json));
    assert(1 == from_json.r);
    assert(2 == from_json.g);
    assert(3 == from_json.b);

    return 0;
}
