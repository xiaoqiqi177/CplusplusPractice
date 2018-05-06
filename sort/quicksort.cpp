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

int quicksort(vector<int>& array, int left, int right){
    if (right > left){
        //select the last one as pivot
        int pivot = array[right];
        int index = left - 1;
        for (int i = left; i < right; i++){
            if (array[i] <= pivot){
                index += 1;
                swap(array[index], array[i]);
            }
        }
        index += 1;
        swap(array[index], array[right]);
        quicksort(array, left, index-1);
        quicksort(array, index+1, right);
    }
    return 0;
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
    
    quicksort(numbers, 0, NO-1);

    cout << "after sort" << endl;
    printarray(numbers, NO);
    return 0;
}

