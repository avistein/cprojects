#include<stdio.h>

struct resources{
    int all[10];
    int max[10];
    int need[10];
    int finish;
};

int isSafe(struct resources[],int,int,int[]);
void calcNeed(struct resources[],int,int);

int isSafe(struct resources res[], int p, int r,int avail[]){
    calcNeed(res,p,r);
    int count = 0,i,j;
    int safeSeq[p];
    while(count < p){

        int found = 0;
        for(i = 0; i < p; i++){
            if(!res[i].finish){
                for(j = 0; j < r; j++)
                    if(res[i].need[j] > avail[j])
                        break;
                if(j == r){
                    for(j = 0; j < r; j++)
                        avail[j] += res[i].all[j];
                    safeSeq[count++] = i;
                    res[i].finish = 1;
                    found = 1;
                }
            }
        }
        if(!found){
            printf("\nSystem is in unsafe state");
            return 0;
        }
    }
    printf("\nSystem is in safe state. \nSafe sequence is : ");
    for(i = 0; i < p; i++)
        printf("\n%d ",safeSeq[i]);
    return 1;
}
void calcNeed(struct resources res[],int p,int r){
    int i,j;
    for(i = 0; i < p; i++){
        for(j = 0; j < r; j++){
            res[i].need[j] = res[i].max[j] - res[i].all[j];
            if(res[i].need[j] < 0)
               res[i].need[j] = 0;
        }
    }
}

int main(){
        int p, r, i, j, avail[10], pid, newr;
        struct resources res[10];
        printf("\nEnter no of processes : ");
        scanf("%d",&p);
        printf("\nEnter no of resources : ");
        scanf("%d",&r);
        for(i = 0; i < p; i++){
            printf("\nEnter details for process P%d",i);
            printf("\nEnter allocation\t -- \t");
            for(j = 0; j < r; j++)
                scanf("%d",&res[i].all[j]);
            printf("\nEnter Max\t\t -- \t");
            for(j = 0; j < r; j++)
                scanf("%d",&res[i].max[j]);
            res[i].finish = 0;
        }
        printf("\nEnter available resources\t -- \t");
        for(j = 0; j < r; j++)
            scanf("%d",&avail[j]);
        printf("\nEnter new request details -- ");
        printf("\nEnter pid \t -- \t");
        scanf("%d",&pid);
        printf("\nEnter request for resources \t -- \t");
        for(j = 0; j < r; j++){
            scanf("%d",&newr);
            res[pid].all[j] += newr;
            avail[j] -= newr;
        }
        isSafe(res,p,r,avail);
        for(i = 0; i < p; i++){
                printf("\nProcess P%d\t",i);
                for(j = 0; j < r; j++)
                    printf("%d ",res[i].all[j]);
                printf("\t");
                for(j = 0; j < r; j++)
                    printf("%d ",res[i].max[j]);
                printf("\t");
                for(j = 0; j < r; j++)
                    printf("%d ",res[i].need[j]);
        }
        return 0;
}
