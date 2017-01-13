//
//  testWeightMap.cpp
//  HW1
//
//  Created by Shirley He on 1/18/16.
//  Copyright © 2016 Shirley He. All rights reserved.
//


#include "WeightMap.h"
#include <cassert>

int main()
{
    WeightMap w1;
    assert(!w1.enroll("Adam", -70));
    assert(w1.enroll("Adam", 70) && w1.enroll("Ben", 71) && w1.enroll("Claire", 72));
    assert(w1.weight("Claire")==72 && w1.weight("Dan")==-1);
    assert(w1.adjustWeight("Ben", -70) && !w1.adjustWeight("ben", 0) && !w1.adjustWeight("Ben", -2));
    assert(w1.size()==3);
    assert(!w1.enroll("Adam", 5));
    assert(w1.enroll("Zed", 0));
    w1.print();
}
