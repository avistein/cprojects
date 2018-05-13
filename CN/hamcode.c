#include <stdio.h>
#include <math.h>
#include <string.h>
#define log2(x) log(x)/log(2)

int noOfRedBits(int length){
    int tmp = log2(length+1) + 1;
    if(pow(2,tmp) - tmp >= length + 1)
        return tmp;
    return tmp + 1;
}

void copyData(char data[], int hamcode[],int n){
    int i = n,j = 0;
    while(i >= 0){
        if(!(i & (i-1)) == 0){
            hamcode[i] = data[j++] - 48;
        }
        else
            hamcode[i] = 4;
        i--;
    }
}

int findCombi(int n, int pos, int combi[]){
    int i, j = 0;
    for(i = 1; i <=n ; i++){
        if((i >> pos) & 1){
            combi[j++] = i;
        }
    }
    return j;
}

int calcParity(int hamcode[], int combi[], int rp, int n){
    int p = 0,i;
    for(i = 0; i < n; i++){
        if(combi[i] != rp){
            p = p ^ hamcode[combi[i]];
        }
    }
    if(p)
        return 1;
    return 0;
}

void calcVRC(int hamcode[], int n, int redBits){
    int i;
    for(i = 0; i < redBits; i++){
        int combi[50];
        int len = findCombi(n, i, combi);
        int parity = calcParity(hamcode, combi, (int)pow(2,i), len);
        hamcode[(int)pow(2,i)] = parity;
    }
}

int errDet(int hamcode[], int n, int redBits){
    int i,j = 0, check[50], pos = 0;
    for(i = 0; i < redBits; i++){
        int combi[50];
        int len = findCombi(n, i, combi);
        int parity = calcParity(hamcode, combi, -1, len);
        check[j++] = parity;
    }

    for(i = 0; i < j; i++){
        pos = pow(2,i) * check[i] + pos;
    }
    return pos;
}

int main(){
    char data[50];
    int hamcode[50], i = 0, ch, j;
    printf("\nEnter data bits : ");
    scanf("%s",data);
    int len  = strlen(data);
    int redBitsLen = noOfRedBits(len);
    printf("\n\t\t***Menu***\n");
    printf("1.Generate codeword \n2.Error detection and Correction\n");
    printf("\nEnter choice : ");
    scanf("%d",&ch);
    switch(ch){
        case 1:
            printf("\n***Generate codeword***\n");
            copyData(data, hamcode,redBitsLen + len);
            calcVRC(hamcode,redBitsLen + len, redBitsLen);
            printf("\nThe sent codeword is : ");
            for(i = redBitsLen + len; i >= 1 ; i--)
                printf("%d",hamcode[i]);
            break;
        case 2:
            printf("\n***Error detection and Correction***\n");
            j = len;
            while(data[i] != '\0')
                hamcode[j--] = data[i++] - 48;
            int pos = errDet(hamcode, len, redBitsLen);
            if(pos){
                printf("\nError position = %d", pos);
                hamcode[pos] = !hamcode[pos];
                printf("\nCorrected code word : ");
                for(i = len; i >= 1 ; i--)
                    printf("%d",hamcode[i]);
            }
            else
                printf("\nNo error!");
            break;
        default:
            printf("\nWrong choice");
    }
    return 0;
}
