//
//  main.cpp
//  HW3(No.4)
//
//  Created by Shirley He on 2/8/16.
//  Copyright © 2016 Shirley He. All rights reserved.
//

int countIncludes(const double a1[], int n1, const double a2[], int n2)
{
    if (n1 <= 0 && n2 != 0)
        return 0;
    if (n2 <= 0)
        return 1;
    if (a1[0] == a2[0])
        return countIncludes(a1+1, n1-1, a2, n2) + countIncludes(a1+1, n1-1, a2+1, n2-1);
    return countIncludes(a1+1, n1-1, a2, n2);
}

void exchange(double& x, double& y)
{
    double t = x;
    x = y;
    y = t;
}

void split(double a[], int n, double splitter,
           int& firstNotGreater, int& firstLess)
{
    if (n < 0)
        n = 0;

    firstNotGreater = 0;
    firstLess = n;
    int firstUnknown = 0;
    while (firstUnknown < firstLess)
    {
        if (a[firstUnknown] < splitter)
        {
            firstLess--;
            exchange(a[firstUnknown], a[firstLess]);
        }
        else
        {
            if (a[firstUnknown] > splitter)
            {
                exchange(a[firstNotGreater], a[firstUnknown]);
                firstNotGreater++;
            }
            firstUnknown++;
        }
    }
}


void order(double a[], int n)
{
    if (n > 1)
    {
        int firstNotGreater, firstLess = 0;
        split(a, n, a[0], firstNotGreater, firstLess);
        order(a, firstNotGreater);
        order(a+firstLess, n-firstLess);
    }
}

