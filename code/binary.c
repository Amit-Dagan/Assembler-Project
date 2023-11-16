# include <stdio.h>
# include <string.h>
# include <ctype.h> 
# include <stdlib.h>


int decimal_to_binary(int decimalnum)
{
    int  binarynum = 0;
    int rem, temp = 1;

    while (decimalnum!=0)
    {
        rem = decimalnum%2;
        decimalnum = decimalnum / 2;
        binarynum = binarynum + rem*temp;
        temp = temp * 10;
    }
    return binarynum;
}


int get_thirty_two_charecter(int n){

	char carecters[32] = "!@#$%^&*<>abcdefghijklmnopqrstuv";
	return carecters[n];
}


/*
int main(){

	int d = 51;
	int b = decimal_to_binary(d);
	
	printf("%d", b);


}
*/
