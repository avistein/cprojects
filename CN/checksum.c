#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define MAX 50
#define not(x) (x?0:1)
//void checksumChecker(
void comple(char string[],int len){

	int i;
	for(i = 0; i < len; i++)
		string[i] = not(string[i] - 48) + 48;

}

void padZeroes(char string[], int n){

	int i;
	char temp = string[0];
	for(i = 0; i < n - 1; i++)
		string[i] = '0';
	string[i++] = temp;
	string[i] = '\0';

}

void splitString(char string[], int len, char half1[], char half2[]){

	int i, j;
	for(i = 0,j = len / 2 ; i < len / 2,j < len; i++,j++){

		half1[i] = string[i];
		half2[i] = string[j];

	}
	half1[i] = '\0';
	half2[i] = '\0';

}

void binAdd(char half1[], char half2[], char checkSum[], int len){

	int i;
	int a, b, c = 0;
	for(i = len - 1; i >= 0; i--){

		a = half1[i] - 48;
		b = half2[i] - 48;
		checkSum[i] = (a ^ b ^ c) + 48;
		c = (a & b) | (b & c) | (c & a);

	}
	checkSum[len] = '\0';
	if(c){

		char temp[MAX];
		temp[0] = '1';
		padZeroes(temp,len);
		binAdd(checkSum, temp, checkSum, len);

	}

}

int main(){

	char bitSeq[MAX], checkSum[MAX], half1[MAX], half2[MAX];
	printf("\nEnter the bit sequence : ");
	scanf("%s",bitSeq);
	splitString(bitSeq,strlen(bitSeq),half1,half2);
	binAdd(half1, half2, checkSum, strlen(half1));
	comple(checkSum,strlen(checkSum));
	printf("\nThe checksum is : %s\n",checkSum);
	return 0;

}
