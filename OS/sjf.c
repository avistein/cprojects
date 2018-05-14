#include <stdio.h>
struct process{
    int pid;
    int bt;
};

void findOrder(struct process p[], int n, int order[]){
    int i, j, bt[20];
    for(i = 0; i < n; i++){
        order[i] = p[i].pid;
        bt[i] = p[i].bt;
    }
    for(i = 0; i < n - 1; i++){
        for(j = 0; j < n - i - 1; j++){
            if(bt[j] > bt[j + 1]){
                int tmp = order[j];
                order[j] = order[j + 1];
                order[j + 1] = tmp;
                int tmp1 = bt[j];
                bt[j] = bt[j + 1];
                bt[j + 1] = tmp1;
            }
        }
    }
}

void sjf(struct process p[], int n, int order[], int wt[], int tat[]){
    int i, tatSum , wtSum;
    wt[order[0]] = 0;
    tat[order[0]] = p[order[0]].bt + wt[order[0]];
    tatSum = tat[order[0]];
    wtSum = wt[order[0]];
    for(i = 1; i < n; i++){
        wt[order[i]] = wt[order[i - 1]] + p[order[i - 1]].bt;
        tat[order[i]] = p[order[i]].bt + wt[order[i]];
        tatSum += tat[order[i]];
        wtSum += wt[order[i]];
    }
    printf("\n\nProcess\t\tBurst Time\t\tTurn Around Time\t\tWaiting Time");
    for(i = 0; i < n; i++){
            printf("\n%d\t\t%d\t\t\t%d\t\t\t%d", p[i].pid, p[i].bt, tat[i], wt[i]);
    }
    printf("\nAverage waiting time = %f", wtSum / (float)n);
    printf("\nAverage turn around time = %f", tatSum / (float)n);
}

int main(){
    int wt[20], tat[20], i;
    struct process p[20];
    int order[20];
    printf("\nEnter no of processes : ");
    scanf("%d", &n);
    for(i = 0; i < n; i++){
        p[i].pid = i;
        printf("\nEnter burst time for P%d: ", i);
        scanf("%d", &p[i].bt);
    }
    findOrder(p, n, order);
    sjf(p, n, order, wt, tat);
}
