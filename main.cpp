//
//  main.cpp
//  Truth Table Generator
//
//  Created by Jacob Hanson on 1/30/19.
//  Copyright © 2019 Jacob Hanson. All rights reserved.
//

#include <iostream>
#include <stack>
#include <vector>
#include <cmath>

using namespace std;

void createPossMatrix(vector<vector<int>> &matrix, vector<char> &vars);
void runOperation(stack<int> &operandStack, stack<char> &operators);
int processExpression(string expression, vector< vector<int> > &possMatrix, vector<char> operands, int attemptNum);
void setOperands(string input, vector<char> &operands);
void generateTable(vector<char> operands, string expression, vector<vector<int>> poss, vector<int> soln);
int andFunc(int, int);
int orFunc(int, int);
int notFunc(int);
int ifFunc(int, int);

int main() {
    vector<char> operands;
    stack<int> operandStack;
    stack<char> operators;
    vector< vector<int> > possMatrix;
    vector<int> solnMatrix;
    string input;
    
    cout <<"Enter operators separated by spaces: ";
    getline(cin, input);
    
    setOperands(input, operands);
    createPossMatrix(possMatrix, operands);
    
    cout <<"Enter expression: ";
    getline(cin, input);
    for (int i = 0; i < pow(2, operands.size()); i++)
        solnMatrix.push_back(processExpression(input, possMatrix, operands, i));
    
    generateTable(operands, input, possMatrix, solnMatrix);
    return 0;
}

void setOperands(string input, vector<char> &operands) {
    for (int i = 0; i < input.length(); i++) {
        if (input[i] == ' ')
            continue;
        else
            operands.push_back(input[i]);
    }
}

void createPossMatrix(vector<vector<int>> &matrix, vector<char> &vars) {
    int n = (int) vars.size();
    for (int i = 0; i < (1<<n); i++) {
        vector<int> row;
        for (int j = 0; j < n; j++)
            row.push_back((int)((i & (1<<j))?1:0));
        matrix.push_back(row);
    }
}

int processExpression(string expression, vector< vector<int> > &possMatrix, vector<char> operands, int attemptNum) {
    stack<char> operators;
    stack<int> operandStack;
    
    for (int i = 0; i < expression.length(); i++) {
        switch (expression[i]) {
            case ' ': break;
            case '>': while (!operators.empty() && (operators.top() == '^' || operators.top() == 'v' || operators.top() == '~'))
                runOperation(operandStack, operators);
                break;
            case '^':
            case 'v': while (!operators.empty() && (operators.top() == '~'))
                runOperation(operandStack, operators);
                break;
            case '(':
            case '~': operators.push(expression[i]);
                break;
            case ')': while ((operators.top() =! '('))
                runOperation(operandStack, operators);
                cout <<operators.top();
                operators.pop();
                break;
            default:
                break;
        }
        for (int j = 0; j < operands.size(); j++) {
            if (expression[i] == operands[j])
                operandStack.push(possMatrix[attemptNum][j]);
        }
    }
    
    while (!operators.empty())
        runOperation(operandStack, operators);
    
    return operandStack.top();
}

void runOperation(stack<int> &operandStack, stack<char> &operators) {
    char op = operators.top();
    operators.pop();
    int op2 = operandStack.top();
    operandStack.pop();
    if (op == '~') {
        if (op2 == 0)
            operandStack.push(notFunc(op2));
    }
    else {
        int op1 = operandStack.top();
        operandStack.pop();
        if (op == '^')
            operandStack.push(andFunc(op1, op2));
        if (op == 'v')
            operandStack.push(orFunc(op1, op2));
        if (op == '>')
            operandStack.push(ifFunc(op1, op2));
    }
    op2 = operandStack.top();
}

void generateTable(vector<char> operands, string expression, vector<vector<int>> poss, vector<int> soln) {
    for (int i = 0; i < operands.size(); i++)
        cout <<"| " <<operands[i] <<" ";
    cout <<"| " <<expression <<endl;
    for (int i = 0; i < poss.size(); i++) {
        for (int j = 0; j < poss[i].size(); j++)
            cout <<"| " <<poss[i][j] <<" ";
        cout <<"| " <<soln[i] <<endl;
    }
}

int andFunc(int x, int y) {
    if (x == 0 || y == 0)
        return 0;
    else return 1;
}

int orFunc(int x, int y) {
    if (x == 1 || y == 1)
        return 1;
    else return 0;
}

int notFunc(int x) {
    if (x == 0)
        return 1;
    else return 0;
}

int ifFunc (int x, int y) {
    if (x == 1 && y == 0)
        return 0;
    return 1;
}
