#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void addZeroes(char[] ,int, int);
char* generateCRCcode(char[] , char[],int);
void generateCodeWord(char[] , char[]);
void crcChecker(int, int);

void addZeroes(char dataWord[],int startLength,int noOfZeroes){
	while(noOfZeroes--)
		dataWord[startLength++] = '0';
	dataWord[startLength] = '\0';
}

char* generateCRCcode(char dataWord[],char divisor[],int ch){
	int i = 0,j,k,flag,c = 0;
	char *crc;
	while(dataWord[i] != '\0'){
		flag = 0;
		j = 0;
		if(strlen(dataWord) - i < strlen(divisor))
			break;
		while(divisor[j] != '\0'){
		    int temp = (dataWord[i] - 48) ^ (divisor[j] - 48);
            dataWord[i] = temp + 48;
			if(dataWord[i] == '1' && !flag){
				flag = 1;
				k = i;
			}
			i++;
			j++;
		}

		if(flag)
			i = k;
	}
	dataWord[strlen(dataWord)] = '\0';
	if(ch == 1){
        crc = malloc(sizeof(char)*(strlen(dataWord)-i));
        while(dataWord[i] != '\0'){
		crc[c++] = dataWord[i++];
        }
        crc[c] = '\0';
        return crc;
	}
	else{
        crcChecker(i,strlen(dataWord)-1);
        return "";
	}
}
void generateCodeWord(char codeWord[],char crc[]){
    strcat(codeWord,crc);
}

void crcChecker(int a, int b){
    if(a == b)
        printf("\nData accepted");
    else
        printf("\nData not accepted");
}
int main(void) {
	char dataWord[50],divisor[50],codeWord[50],recv[50];
	char* crc;
	int i = 0,ch,c;
	printf("\n\t\t***Menu***\n\n1. Generate CRC code\n2. CRC Checker\n");
	printf("\nEnter choice : ");
	scanf("%d",&ch);
	while ( ( c = getchar() ) != EOF && c != '\n' );
	switch(ch){
        case 1:
            printf("\nEnter the dataword : ");
            scanf("%s",dataWord);
            strcpy(codeWord,dataWord);
            printf("\nEnter divisor : ");
            scanf("%s",divisor);
            addZeroes(dataWord,strlen(dataWord),strlen(divisor) - 1);
            crc = generateCRCcode(dataWord,divisor,1);
            generateCodeWord(codeWord,crc);
            printf("\nGenerated code word : %s",codeWord);
            break;
        case 2:
            printf("\nEnter the received data : ");
            scanf("%s",recv);
            strcpy(codeWord,dataWord);
            printf("\nEnter divisor : ");
            scanf("%s",divisor);
            generateCRCcode(dataWord,divisor,2);
            break;
        default:
            printf("\nWrong choice");
	}
	return 0;
}
