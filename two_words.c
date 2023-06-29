#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define STRING_SIZE 64
#define PRINT_CSV

struct FILEDATA{
    char filename[STRING_SIZE];
    char mean[STRING_SIZE];
    int frame_num;
    double *voice_data;
};

int ReadFile(char* filename, struct FILEDATA *filedata){
    FILE *fp;
    fp = fopen(filename, "r");
    if(fp == NULL) return -1;
    
    fgets(filedata->filename, STRING_SIZE, fp);
    fgets(filedata->mean, STRING_SIZE, fp);
    char _buf[STRING_SIZE];
    fgets(_buf, STRING_SIZE, fp);
    sscanf(_buf, "%d", &filedata->frame_num);

    double* data;
    data = (double*)malloc(sizeof(double)*filedata->frame_num*15);
    if(data == NULL) return -1;
    for(int i=0; i<filedata->frame_num; i++){
        for(int j=0; j<15; j++){
            fscanf(fp, "%lf", &data[i*15+j]);
        }
    }

    filedata->voice_data = data;

    fclose(fp);
    return 0;
}

void CalcLocalDistance(struct FILEDATA *answer, struct FILEDATA *question, double** d){
    for(int i=0; i<answer->frame_num; i++){
        for(int j=0; j<question->frame_num; j++){
            double sum = 0,a=0,q=0;
            for(int k=0; k<15; k++){
                a = answer->voice_data[i*15+k];
                q = question->voice_data[j*15+k];
                sum += (a-q)*(a-q);
            }
            d[i][j] = sqrt(sum);
        }
    }
}

void CalcCumulativeDistance(struct FILEDATA *answer, struct FILEDATA *question, double** d,double** g){
    g[0][0] = d[0][0];
    for(int i=1;i<answer->frame_num;i++){
        g[i][0] = g[i-1][0] + d[i][0];
    }
    for(int j=1;j<question->frame_num;j++){
        g[0][j] = g[0][j-1] + d[0][j];
    }
    for(int i=1;i<answer->frame_num;i++){
        for(int j=1;j<question->frame_num;j++){
            double a = g[i][j-1] + d[i][j];
            double b = g[i-1][j] + d[i][j];
            double c = g[i-1][j-1] + 2*d[i][j];
            double min = a;
            if(min > b) min = b;
            if(min > c) min = c;
            g[i][j] = min;
        }
    }
}

int main(int argc,char *argv[]){
    if(argc <= 2){
        printf("Enter the files name.\n");
        return -1;
    }

    struct FILEDATA answer;
    struct FILEDATA question;

    if(ReadFile(argv[1], &answer) == -1){
        printf("Answer File open error.\n");
        return -1;
    }
    if(ReadFile(argv[2], &question) == -1){
        printf("Question File open error.\n");
        return -1;
    }

    double** d = (double**)malloc(sizeof(double*)*answer.frame_num);
    if(d == NULL) return -1;
    for(int i=0; i<answer.frame_num; i++){
        d[i] = (double*)malloc(sizeof(double)*question.frame_num);
        if(d[i] == NULL){
            for(int k = 0;k<i;k++){
                free(d[k]);
            }
            free(d);
            return -1;
        }
    }

    double** g = (double**)malloc(sizeof(double*)*answer.frame_num);
    if(g == NULL) return -1;
    for(int i=0; i<answer.frame_num; i++){
        g[i] = (double*)malloc(sizeof(double)*question.frame_num);
        if(g[i] == NULL){
            for(int k = 0;k<i;k++){
                free(g[k]);
            }
            free(g);
            return -1;
        }
    }

    CalcLocalDistance(&answer, &question, d);


    CalcCumulativeDistance(&answer, &question, d,g);
    
    int frame_i = answer.frame_num-1;
    int frame_j = question.frame_num-1;
    double distance = g[frame_i][frame_j];


    #ifdef PRINT_CSV
    printf("%f",distance/(frame_i+frame_j));
    #else
    printf("answer word is %s",answer.mean);
    printf("question word is %s",question.mean);
    printf("distance = %f\n", distance);
    printf("distance between answer and question = %f\n", distance/(frame_i+frame_j));
    #endif

    free(answer.voice_data);
    free(question.voice_data);
    for(int i=0;i<answer.frame_num;i++){
        free(d[i]);
        free(g[i]);
    }
    free(d);
    free(g);
    
    return 0;
}