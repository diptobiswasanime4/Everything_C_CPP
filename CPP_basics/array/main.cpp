#include <iostream>

using namespace std;

int main()
{
    int int_arr[4] = {1, 2, 3};

    cout << int_arr << endl;
    cout << int_arr[2] << endl;
    cout << int_arr[3] << endl;

    int *ptr = int_arr;
    ptr += 3;
    *ptr = 10;

    cout << int_arr[3] << endl;

    /*
    Hence it is verified
    that an array is a
    contiguous memory allocation
    */

    return 0;
}