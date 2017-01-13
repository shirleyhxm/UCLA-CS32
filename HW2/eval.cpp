//
//  main.cpp
//  HW2(eval)
//
//  Created by Shirley He on 2/2/16.
//  Copyright © 2016 Shirley He. All rights reserved.
//

#include "Map.h"
#include <iostream>
#include <string>
#include <stack>
#include <cctype>
#include <cassert>
using namespace std;


int evaluate(string infix, const Map& values, string& postfix, int& result);
int precedence(char c);

int precedence(char c)
{
    if (c == '+' || c == '-')
        return 1;
    if (c == '/' || c == '*')
        return 2;
    return -1;
    
}

int evaluate(string infix, const Map& values, string& postfix, int& result)
{
    string infix1 = "";
    for (int i = 0; i < infix.length(); i++)
    {
        if (infix[i] != ' ')
            infix1 += infix[i];
    }
    infix = infix1;
    
    if (infix.length()%2 == 0)
        return 1;
    
    postfix = "";
    int openPar = 0;
    int closePar = 0;
    stack<char> oprt;
    
    char ch;
    for (int i = 0; i < infix.length(); i++)
    {
        ch = infix[i];
        if (islower(ch))
        {
            if (i != 0 && (islower(infix[i-1]) || infix[i-1]==')'))
                return 1;
            postfix += ch;
        }
        else
        {
            switch (ch)
            {
                case '(':
                    openPar++;
                    if (i != 0 && infix[i-1]!='+' && infix[i-1]!='-' && infix[i-1]!='*' && infix[i-1]!='/' && infix[i-1]!='(')
                        return 1;
                    oprt.push(ch);
                    break;
                case ')':
                    closePar++;
                    if (closePar>openPar || (!islower(infix[i-1]) && infix[i-1]!=')'))
                        return 1;
                    while(oprt.top() != '(')
                    {
                        postfix += oprt.top();
                        oprt.pop();
                    }
                    oprt.pop();
                    break;
                case '+':
                case '-':
                case '*':
                case '/':
                    if (i == 0 || !islower(infix[i-1]) || i == infix.length()-1)
                        return 1;
                    while (!oprt.empty() && oprt.top() != '(' && precedence(ch) <= precedence(oprt.top()))
                    {
                        postfix += oprt.top();
                        oprt.pop();
                    }
                    oprt.push(ch);
                    break;
                default:
                    return 1;
                    break;
            }
        }
    }
    if (openPar != closePar)
        return 1;
    
    while (!oprt.empty())
    {
        postfix += oprt.top();
        oprt.pop();
    }
    
    stack<int> oprn;
    int operand, operand1, operand2;
    for (int i = 0; i < postfix.length(); i++)
    {
        ch = postfix[i];
        if (islower(ch))
        {
            if (!values.get(ch, operand))
                return 2;
            oprn.push(operand);
        }
        else
        {
            operand2 = oprn.top();
            oprn.pop();
            operand1 = oprn.top();
            oprn.pop();
            switch (ch)
            {
                case '+':
                    oprn.push(operand1+operand2);
                    break;
                case '-':
                    oprn.push(operand1-operand2);
                    break;
                case '*':
                    oprn.push(operand1*operand2);
                    break;
                case '/':
                    if (operand2 == 0)
                        return 3;
                    oprn.push(operand1/operand2);
                    break;
                default:
                    break;
            }
        }
    }
    result = oprn.top();
    return 0;
}


int main()
{
    char vars[] = { 'a', 'e', 'i', 'o', 'u', 'y', '#' };
    int  vals[] = {  3,  -9,   6,   2,   4,   1  };
    Map m;
    for (int k = 0; vars[k] != '#'; k++)
        m.insert(vars[k], vals[k]);
    string pf;
    int answer;
    assert(evaluate("a+ e", m, pf, answer) == 0  &&
           pf == "ae+"  &&  answer == -6);
    answer = 999;
    assert(evaluate("", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("a+", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("a i", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("ai", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("()", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("y(o+u)", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("a+E", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("(a+(i-o)", m, pf, answer) == 1  &&  answer == 999);
    // unary operators not allowed:
    assert(evaluate("-a", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("a*b", m, pf, answer) == 2  &&
           pf == "ab*"  &&  answer == 999);
    assert(evaluate("y +o *(   a-u)  ", m, pf, answer) == 0  &&
           pf == "yoau-*+"  &&  answer == -1);
    answer = 999;
    assert(evaluate("o/(y-y)", m, pf, answer) == 3  &&
           pf == "oyy-/"  &&  answer == 999);
    assert(evaluate(" a  ", m, pf, answer) == 0  &&
           pf == "a"  &&  answer == 3);
    assert(evaluate("((a))", m, pf, answer) == 0  &&
           pf == "a"  &&  answer == 3);
    cout << "Passed all tests" << endl;
}
