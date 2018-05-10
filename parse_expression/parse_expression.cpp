#include<stdio.h>
#include<stdlib.h>/* srand, rand */
#include<iostream>
#include<vector>
#include<time.h>
#include<algorithm>
#include<math.h>
#include<string>
#include<stack>
#include<map>
using namespace std;

#define VARIABLE 0
#define VALUE 1

class Evaluation{
    public:
        map<char, int> dic;
        Evaluation(const map<char, int> &dic_):dic(dic_) {};
        static Evaluation parse(string expression){
            size_t pos = string::npos;
            string token;
            map<char, int> dic_;
            while ((pos = expression.find(';')) != string::npos){
                token = expression.substr(0, pos);
                if (pos == 0)
                    break;
                size_t equalpos = token.find('=');
                dic_[token[0]] = atoi(token.substr(equalpos+1).c_str());
                expression.erase(0, pos+1);
            }
            return Evaluation(dic_);
        }
};

class Expression {
    public:
        static map<char, int> privilege_dic = {
            {'+',0}, 
            {'-',0}, 
            {'*',1}, 
            {'/',1}, 
            {'^',2},
        };
        
        friend bool isoperator(const char chr){
            if (chr == '+' || chr == '-' || chr == '*' || chr == '-' || chr == '^')
                return true;
            else
                return false;
        }
        
        static Expression parse(Expression &left, string expression){
            int current_privilege = privilege_dic[expression[0]];
            int i = 1;
            int left_cnt = 0;
            while (i < expression.size() && (isoperator(expression[i]) == false || privilege_dic[expression[i]] > current_privilege || left_cnt > 0)){
                if (expression[i] == '(')
                    left_cnt += 1;
                else if (expression[i] == ')')
                    left_cnt -= 1;
                i += 1;
            }
            if (expression[0] == '+')
                Expression newleft = Plus(expression[0], current_privilege, &left, &parse(expression.substr(1, i)));
            else if (expression[0] == '-')
                Expression newleft = Minus(expression[0], current_privilege, &left, &parse(expression.substr(1, i)));
            else if (expression[0] == '*')
                Expression newleft = Multiply(expression[0], current_privilege, &left, &parse(expression.substr(1, i)));
            else if (expression[0] == '/')
                Expression newleft = Divide(expression[0], current_privilege, &left, &parse(expression.substr(1, i)));
            else if (expression[0] == '^')
                Expression newleft = Power(expression[0], current_privilege, &left, &parse(expression.substr(1, i)));
            return parse(newleft, expression.substr(i));
        }
        
        static Expression parse(string expression){
            if (expression[0] == '(')
                return parse(expression.substr(1, -1));
            int i = 0;
            while (isoperator(expression[i]) == false)
                i += 1;
            if (i > 1 || (expression[0] >= '0' && expression[0] <= '9')){
                Expression newleft = Value(double(atoi(expression.substr(0, i).c_str())));
            }
            else{
                Expression newleft = Variable(expression[0]);
            }
            return parse(newleft, expression.substr(i));     
        }
        virtual Expression reduce(const Evaluation evaluation) = 0;
        virtual double eval() = 0;
};

class Variable: public Expression{
    public:
        char var;
        Variable(char var_): var(var_) {};
        Expression reduce(const Evaluation evaluation){
            map<char, int> dic = evaluation.dic;
            if (dic.find(var) != dic.end()){
                return Value(dic.find(var)->second);
            }
        }
        double eval(){
            return NAN;
        }
};

class Value: public Expression{
    public:
        double val;
        Value(double val_): val(val_) {};
        Expression reduce(const Evaluation evaluation){
            return Value(val);
        }
        double eval(){
            return val;
        }
};

class Operator: public Expression{
    public:
        char op;
        int privilege;
        Expression* left;
        Expression* right;
        Operator(char op_, int privilege_, Expression* left_, Expression* right_):op(op_), privilege(privilege_), left(left_), right(right_) {};
        Expression reduce(const Evaluation evaluation){
            Expression new_left = left->reduce(evaluation);
            Expression new_right = right->reduce(evaluation);
            return Operator(op, privilege, &new_left, &new_right);
        }
        virtual double eval() = 0;
};

class Plus: public Operator{
    public:
        double eval(){
            return left->eval() + right->eval();
        }
};

class Minus: public Operator{
    public:
        double eval(){
            return left->eval() - right->eval();
        }
};

class Multiply: public Operator{
    public:
        double eval(){
            return left->eval() * right->eval();
        }
};

class Divide: public Operator{
    public:
        double eval(){
            return left->eval() / right->eval();
        }
};

class Power: public Operator{
    public:
        double eval(){
            return pow(left->eval(), right->eval());
        }
};

int main(){
    //example code
    Expression exp = Expression::parse("2*a-(a-b)^2");
    Evaluation eval = Evaluation::parse("a=10;c=7;");
    Evaluation eval2 = Evaluation::parse("b=8");
    Expression reduce1 = exp.reduce(eval);
    double result1 = reduce1.eval();
    cout << result1 << endl;
    Expression reduce2 = reduce1.reduce(eval2);
    double result2 = reduce2.eval();
    cout << result2 << endl;
    return 0;
}
