#include<stdio.h>
#include<stdlib.h>/* srand, rand */
#include<iostream>
#include<vector>
#include<time.h>
#include<algorithm>
using namespace std;

int printarray(vector<int> array, int cnt){
    for (int i = 0; i < cnt; i++){
        cout << array[i] << " ";
    }
    cout << endl;
}

int main(){
    srand(time(0));
    int NO = 100;
    int MAX = 1000;
    vector<int> numbers;
    for (int i = 0; i < NO; i++){
        numbers.push_back(rand() % MAX);
    }

    cout << "before sort" << endl;
    printarray(numbers, NO);
    
    //to sort
    for (int i = 0; i < NO; i++){
        for (int j = 0; j < NO - i; j++){
            if (numbers[j] > numbers[j+1])
                swap(numbers[j], numbers[j+1]);
        }
    }

    cout << "after sort" << endl;
    printarray(numbers, NO);
    return 0;
}

