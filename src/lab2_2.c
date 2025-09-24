#include <stdio.h>

long long factorial(int n) {
    long long result = 1;
    for (int i = 1; i <= n; i++) {
        result *= i;
    }
    return result;
}

int main() {
    int n;
    printf("Enter a number: ");
    scanf("%d", &n);

    if (n < 0) {
        printf("Error: n must be >= 0\n");
    } else {
        long long result = factorial(n);
        printf("%d! = %lld\n", n, result);
    }
    return 0;
}
