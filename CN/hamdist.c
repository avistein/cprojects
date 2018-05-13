#include <stdio.h>
int main(){
    char sent_data[50], recv_data[50];
    int xor[50], bLength, hDist = 0, i = 0, j = 0;
    printf("\nEnter sent and received data : ");
    scanf("%s %s", sent_data, recv_data);
    while(sent_data[i] != 0 && recv_data[i] != 0){
        if((sent_data[i] - 48) ^ (recv_data[i] - 48)){
            hDist++;
            xor[j++] = i;
        }
        i++;
    }
    printf("\nHamming distance  = %d", hDist);
    if(!j){
        printf("\nBurst Length = %d",0);
        return 0;
    }
    printf("\nBurst Length = %d", (xor[j-1] - xor[0]) + 1);
    return 0;
}
