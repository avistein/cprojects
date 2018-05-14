#include <stdio.h>
struct process{
    int pid;
    int bt;
};

void rr(struct process p[], int n, int qt, int wt[], int tat[]){
    int i, btSum = 0, j, bt[20], prev[20], tatSum = 0, wtSum = 0;
    for(i = 0; i < n; i++){
        btSum += p[i].bt;
        bt[i] = p[i].bt;
        wt[i] = 0;
        prev[i] = 0;
    }
    for(i = 0; i != btSum;){
        for(j = 0; j < n; j++){
            if(bt[j]){
                wt[j] += i - prev[j];

                if(bt[j] - qt < 0){
                    i += bt[j];
                    bt[j] = 0;
                }
                else{
                    bt[j] -= qt;
                    i += qt;
                }
                prev[j] = i;
            }
        }
    }
    for(i = 0; i < n; i++){
        tat[i] = p[i].bt + wt[i];
        tatSum += tat[i];
        wtSum += wt[i];
    }
    printf("\n\nProcess\t\tBurst Time\t\tTurn around time\t\tWaiting Time");
    for(i = 0; i < n; i++){
            printf("\n%d\t\t%d\t\t\t\t%d\t\t\t\t%d", p[i].pid, p[i].bt, tat[i], wt[i]);
    }
    printf("\nAverage waiting time = %f", wtSum / (float)n);
    printf("\nAverage turn around time = %f", tatSum / (float)n);
}
int main(){
    int wt[20], tat[20], i, k, n, tmp;
    struct process p[20];
    int qnt;
    printf("\nEnter no of processes : ");
    scanf("%d", &n);
    for(i = 0; i < n; i++){
        p[i].pid = i;
        printf("\nEnter burst time for P%d: ", i);
        scanf("%d", &p[i].bt);
    }
    printf("\nEnter quantum : ");
    scanf("%d",&qnt);
    rr(p, n, qnt, wt, tat);
}
