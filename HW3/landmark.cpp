//
//  main.cpp
//  HW3
//
//  Created by Shirley He on 2/3/16.
//  Copyright © 2016 Shirley He. All rights reserved.
//

#include <iostream>
#include <string>
using namespace std;

class Landmark
{
public:
    Landmark(string nm);
    virtual ~Landmark();
    virtual string color() const;
    virtual string icon() const = 0;
    virtual string name() const;
private:
    string m_nm;
};

Landmark::Landmark(string nm): m_nm(nm){}

Landmark::~Landmark(){}

string Landmark::color() const
{
    return "yellow";
}

string Landmark::name() const
{
    return m_nm;
}


class Restaurant : public Landmark
{
public:
    Restaurant(string nm, int capa);
    ~Restaurant();
    virtual string icon() const;
private:
    int m_capa;
};

Restaurant::Restaurant(string nm, int capa)
: Landmark(nm), m_capa(capa)
{}

Restaurant::~Restaurant()
{
    cout << "Destroying the restaurant " << Landmark::name() << "." << endl;
}

string Restaurant::icon() const
{
    if (m_capa < 40)
        return "small knife/fork";
    else
        return "large knife/fork";
}

class Hospital : public Landmark
{
public:
    Hospital(string nm);
    ~Hospital();
    virtual string icon() const;
    virtual string color() const;
};

Hospital::Hospital(string nm)
: Landmark(nm)
{}

Hospital::~Hospital()
{
    cout << "Destroying the hospital " << Landmark::name() << "." << endl;
}

string Hospital::icon() const
{
    return "H";
}

string Hospital::color() const
{
    return "blue";
}

class Hotel : public Landmark
{
public:
    Hotel(string nm);
    ~Hotel();
    virtual string icon() const;
};

Hotel::Hotel(string nm)
: Landmark(nm)
{}

Hotel::~Hotel()
{
    cout << "Destroying the hotel " << Landmark::name() << "." << endl;
}

string Hotel::icon() const
{
    return "bed";
}
