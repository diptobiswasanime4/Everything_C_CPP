#include <iostream>

using namespace std;

int main()
{

    int score = 20;
    int *score_ptr = &score;

    printf("score = %d\n", score);
    printf("*score_ptr = %p\n", score_ptr);

    int &ref = score;
    ref = 80;

    printf("&ref = %d\n", ref);
    printf("score = %d\n", score);
    printf("*score_ptr = %p\n", score_ptr);

    return 0;
}