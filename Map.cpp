//
//  Map.cpp
//  HW1
//
//  Created by Shirley He on 1/16/16.
//  Copyright © 2016 Shirley He. All rights reserved.
//

#include "Map.h"

Map::Map()
{
    m_nKeys = 0;
}

bool Map::empty() const
{
    if (m_nKeys != 0)
        return false;
    return true;
}

int Map::size() const
{
    return m_nKeys;
}

bool Map::insert(const KeyType& key, const ValueType& value)
{
    if (m_nKeys == DEFAULT_MAX_ITEMS)
        return false;
    if (contains(key))
            return false;
    m_array[m_nKeys].key = key;
    m_array[m_nKeys].value = value;
    m_nKeys++;
    return true;
}

bool Map::update(const KeyType& key, const ValueType& value)
{
    for (int i = 0; i < m_nKeys; i++)
        if (m_array[i].key == key)
        {
            m_array[i].value = value;
            return true;
        }
    return false;
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
{
    if (update(key, value))
        return true;
    if (insert(key, value))
        return true;
    return false;
}

bool Map::erase(const KeyType& key)
{
    for (int i = 0; i < m_nKeys; i++)
        if (m_array[i].key == key)
        {
            for (int j = i+1; j < m_nKeys; j++)
                m_array[i] = m_array[j];
            m_nKeys--;
            return true;
        }
    return false;
}

bool Map::contains(const KeyType& key) const
{
    for (int i = 0; i < m_nKeys; i++)
        if (m_array[i].key == key)
            return true;
    return false;
}

bool Map::get(const KeyType& key, ValueType& value) const
{
    for (int i = 0; i < m_nKeys; i++)
        if (m_array[i].key == key)
        {
            value = m_array[i].value;
            return true;
        }
    return false;
}

bool Map::get(int i, KeyType& key, ValueType& value) const
{
    if (i >= 0 && i < m_nKeys)
    {
        key = m_array[i].key;
        value = m_array[i].value;
        return true;
    }
    return false;
}

void Map::swap(Map& other)  //without creating any additional array or additional Map.
{
    Info tempInfo;
    int tempInt;
    for (int i = 0; i < DEFAULT_MAX_ITEMS; i++)
    {
        tempInfo = other.m_array[i];
        other.m_array[i] = this->m_array[i];
        this->m_array[i] = tempInfo;
    }
    tempInt = other.m_nKeys;
    other.m_nKeys = this->m_nKeys;
    this->m_nKeys = tempInt;
}