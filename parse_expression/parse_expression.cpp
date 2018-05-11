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
#include<memory>
#include<assert.h>
using namespace std;

class Evaluation{
    public:
        map<char, int> dic;
        Evaluation(const map<char, int> &dic_):dic(dic_) {};
        static Evaluation& parse(string expression){
            return *parse_(expression).get();
        }
        static shared_ptr<Evaluation> parse_(string expression){
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
            for (map<char, int>::iterator iter = dic_.begin(); iter != dic_.end(); iter++)
                cout << iter->first << " " << iter->second << endl;
            return make_shared<Evaluation>(dic_);
        }
};

static const map<char, int> privilege_dic = {
    {'+', 0}, 
    {'-', 0}, 
    {'*', 1}, 
    {'/', 1}, 
    {'^', 2},
};

bool isoperator(const char chr){
    if (chr == '+' || chr == '-' || chr == '*' || chr == '-' || chr == '^')
        return true;
    else
        return false;
}

class Expression{
    public:
        Expression& reduce(const Evaluation evaluation);
        //cannot create ob ject of abstract class(at least one pure virtual function)
        virtual shared_ptr<Expression> reduce_(const Evaluation evaluation) = 0;
        virtual double eval() = 0;
        static shared_ptr<Expression> parse_(shared_ptr<Expression> left, string expression);
        static Expression& parse(string expression);
        static shared_ptr<Expression> parse_(string expression);
};

class Value: public Expression{
    public:
        double val;
        Value(double val_): val(val_) {};
        shared_ptr<Expression> reduce_(const Evaluation evaluation){
            return make_shared<Value>(val);
        }
        double eval(){
            return val;
        }
};

class Variable: public Expression{
    public:
        char var;
        Variable(char var_): var(var_) {};
        shared_ptr<Expression> reduce_(const Evaluation evaluation){
            map<char, int> dic = evaluation.dic;
            if (dic.find(var) != dic.end()){
                return make_shared<Value>(dic.find(var)->second);
            }
            else
                return make_shared<Variable>(var);
        }
        double eval(){
            return NAN;
        }
};

class Operator: public Expression{
    public:
        char op;
        int privilege;
        shared_ptr<Expression> left;
        shared_ptr<Expression> right;
        Operator(char op_, int privilege_, shared_ptr<Expression> left_, shared_ptr<Expression> right_):op(op_), privilege(privilege_), left(left_), right(right_) {};
        
        
        shared_ptr<Expression> reduce_(const Evaluation evaluation){
            shared_ptr<Expression> new_left = left->reduce_(evaluation);
            shared_ptr<Expression> new_right = right->reduce_(evaluation);
            return make_shared<Operator>(op, privilege, new_left, new_right);
        }
        virtual double eval(){
            switch (op){
                case '+':{
                    return left->eval() + right->eval();
                    break;}
                case '-':{
                    return left->eval() - right->eval();
                    break;}
                case '*':{
                    return left->eval() * right->eval();
                    break;}
                case '/':{
                    return left->eval() / right->eval();
                    break;}
                case '^':{
                    return pow(left->eval(), right->eval());
                    break;}
            }
        }
};

shared_ptr<Expression> Expression::parse_(shared_ptr<Expression> left, string expression){
    cout << "binary: " << expression << endl;
    int current_privilege = privilege_dic.at(expression[0]);
    int i = 1;
    int left_cnt = 0;

    while (i < expression.size() && (isoperator(expression[i]) == false || privilege_dic.at(expression[i]) > current_privilege || left_cnt > 0)){
        if (expression[i] == '(')
            left_cnt += 1;
        else if (expression[i] == ')')
            left_cnt -= 1;
        i += 1;
    }
    shared_ptr<Expression> newleft(new Operator(expression[0], current_privilege, left, parse_(expression.substr(1, i-1))));
    if (i == expression.size())
        return newleft;
    return parse_(newleft, expression.substr(i));
}
        
Expression& Expression::parse(string expression){
    return *parse_(expression).get();
}
        
shared_ptr<Expression> Expression::parse_(string expression){
    cout << "uni: " << expression << endl;
    int i = 0;
    if (expression[0] == '('){
        int left_cnt = 1;
        while (left_cnt > 0){
            i += 1;
            if (expression[i] ==')')
                left_cnt -= 1;
            else if (expression[i] == '(')
                left_cnt += 1;
        }
        return parse_(parse_(expression.substr(1,i-1)), expression.substr(i+1));
    }
    while (i < expression.size() && isoperator(expression[i]) == false)
        i += 1;

    shared_ptr<Expression> newleft;
    if (i > 1 || (expression[0] >= '0' && expression[0] <= '9')){
        newleft = make_shared<Value>(double(atoi(expression.substr(0, i).c_str())));
    }
    else{
        newleft = make_shared<Variable>(expression[0]);
    }
    if (i == expression.size())
        return newleft;
    else
        return parse_(newleft, expression.substr(i));     
}

Expression& Expression::reduce(const Evaluation evaluation){
    return *reduce_(evaluation).get();
}

int main(){
    //example code
    
    shared_ptr<Expression> exp = Expression::parse_("2*a-(a-b)^2");
    cout << "parse successfully!" << endl;
    shared_ptr<Evaluation> eval = Evaluation::parse_("a=10;c=7;");
    cout << "first evaluation!" << endl;
    shared_ptr<Evaluation> eval2 = Evaluation::parse_("b=8;");
    cout << "second evaluation!" << endl;
    map<char, int> dic = eval2->dic;
    
    shared_ptr<Expression> reduce1 = exp->reduce_(*eval.get());
    cout << "first reduce!" << endl;
    double result1 = reduce1->eval();
    cout << result1 << endl;
    
    shared_ptr<Expression> reduce2 = reduce1->reduce_(*eval2.get());
    cout << "second reduce!" << endl;
    double result2 = reduce2->eval();
    cout << result2 << endl;
    return 0;
}
