#include "vector.h"

int main(){
    vector2_t myVec = {3.1, 4};

    std::cout << myVec.to_string() << std::endl;

    std::cout << "mag:  " << myVec.magnitude() << std::endl;
    std::cout << "*:    " << (myVec * 2).to_string() << std::endl;
    std::cout << "dot:  " << myVec.dot(myVec) << std::endl;

    std::cout << "add:  " << (myVec + myVec).to_string() << std::endl;
    std::cout << "subt: " << ((myVec*2) - myVec).to_string() << std::endl;

    std::cout << "dist:  " << myVec.distance_from(myVec * 2) << std::endl;
    std::cout << "norm1: " << myVec.normalize().to_string() << std::endl;
    std::cout << "norm2: " << (myVec*2).normalize().to_string() << std::endl;

    std::cout << "json:  " << myVec.to_json() << std::endl;

    return 0;
}