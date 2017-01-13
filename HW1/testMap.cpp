//
//  testMap.cpp
//  HW1
//
//  Created by Shirley He on 1/16/16.
//  Copyright © 2016 Shirley He. All rights reserved.
//

#include "Map.h"
#include <iostream>
#include <cassert>
using namespace std;

void testMap()
{
    Map m1;  // maps strings to doubles
    assert(m1.empty());
    assert(m1.insert("A", 1.1));
    assert(m1.update("A", 2.2) && !m1.update("B", 2.2));
    assert(m1.insertOrUpdate("A", 3.3) && m1.insertOrUpdate("B", 2.2));
    assert(m1.erase("A") && !m1.erase("C"));
    assert(m1.size()==1);
    assert(!m1.contains("A") && m1.contains("B"));
    
    ValueType v = -1.1;
    assert(!m1.get("A", v)  &&  v == -1.1); // v unchanged by get failure
    KeyType k = "hello";
    assert(m1.get(0, k, v)  &&  k == "B"  &&  v == 2.2);
    
    Map m2;
    m2.insert("Ethel", 3.538);
    m2.insert("Lucy", 2.956);
    m1.swap(m2);
    assert(m1.size() == 2  &&  m1.contains("Ethel")  &&  m1.contains("Lucy")  && m2.size() == 1  &&  m2.contains("B"));
    
    cout << "Passed all tests" << endl;
}
