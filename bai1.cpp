#include<stdio.h>
int main() {
	int x = 100, dem = 0;
	while((x -= 2) > 0) dem++;
	printf("%d",dem);
}
