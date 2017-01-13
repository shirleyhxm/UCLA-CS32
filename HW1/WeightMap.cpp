//
//  WeightMap.cpp
//  HW1
//
//  Created by Shirley He on 1/17/16.
//  Copyright © 2016 Shirley He. All rights reserved.
//

#include "WeightMap.h"
#include "Map.h"

#include <iostream>
using namespace std;

WeightMap::WeightMap()
{
}

bool WeightMap::enroll(std::string name, double startWeight)
{
    if (!m_map.contains(name) && m_map.size()!=DEFAULT_MAX_ITEMS && startWeight >= 0)
    {
        m_map.insert(name, startWeight);
        return true;
    }
    return false;
}

double WeightMap::weight(std::string name) const
{
    double weight = -1;
    if (m_map.contains(name))
        m_map.get(name, weight);
    return weight;
}

bool WeightMap::adjustWeight(std::string name, double amt)
{
    if (!m_map.contains(name) || weight(name)+amt < 0)
        return false;
    double weight;
    m_map.get(name, weight);
    m_map.update(name, weight+amt);
    return true;
}

int WeightMap::size() const
{
    return m_map.size();
}

void WeightMap::print() const
{
    std::string name;
    double weight;
    for (int i = 0; i < m_map.size();i++)
    {
        m_map.get(i, name, weight);
        cout << name << " " << weight << endl;
    }
}