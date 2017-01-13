//
//  main.cpp
//  HW3(No.2)
//
//  Created by Shirley He on 2/8/16.
//  Copyright © 2016 Shirley He. All rights reserved.
//

bool anyTrue(const double a[], int n)
{
    if (n <= 0)
        return false;
    if (somePredicate(a[0]))
        return true;
    return anyTrue(a+1, n-1);
}

int countTrue(const double a[], int n)
{
    if (n <= 0)
        return 0;
    if (somePredicate(a[0]))
        return countTrue(a+1, n-1)+1;
    return countTrue(a+1, n-1);
}

int firstTrue(const double a[], int n)
{
    if (n <= 0)
        return -1;
    if (somePredicate(a[0]))
        return 0;
    if (firstTrue(a+1, n-1) == -1)
        return firstTrue(a+1, n-1);     //more than n times?
    return firstTrue(a+1, n-1)+1;
}


int indexOfMin(const double a[], int n)
{
    if (n <= 0)
        return -1;
    if (n == 1)
        return 0;
    if (a[0] <= a[n-1])
        return indexOfMin(a, n-1);
    else
        return indexOfMin(a+1, n-1)+1;
}

bool includes(const double a1[], int n1, const double a2[], int n2)
{
    if (n1 <= 0 && n2 != 0)
        return false;
    if (n2 <= 0)
        return true;
    if (a1[0] == a2[0])
        return includes(a1+1, n1-1, a2+1, n2-1);
    return includes(a1+1, n1-1, a2, n2);
}
