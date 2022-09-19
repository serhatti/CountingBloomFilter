#include "Hasher.h"
#include <iostream>
#include <vector>
int main(){

    std::vector<bool> mybits={1,0,0,0,1,1,0,1};
    std::vector<bool> mybits2={1,1,0,0,1,1,0,1};
    auto h=Hasher<std::vector<bool> >();
    std::cout<<h(mybits,1)<<"\n";
    std::cout<<h(mybits2,1)<<"\n";
    return 0;
}