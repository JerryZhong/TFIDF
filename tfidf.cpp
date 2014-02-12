#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<errno.h>
#include<tfidf.h>
using namespace std;
#define Malloc(type,n) (type *)malloc((n)*sizeof(type))

struct feature_node *x_space ;
struct feature_node *inv_space;//词倒排表
struct problem prob ;

static char *line=NULL;
static int max_line_len;

 void exit_input_error(int line_num)
{
    fprintf(stderr,"Wrong input format at line %d\n", line_num);
    exit(1);
}


/*
 *target:get one line data.
 * */
 static char* readline(FILE *input)
{
    int len;
    if(fgets(line,max_line_len,input)==NULL)
        return NULL;
    
     while(strrchr(line,'\n')==NULL)
    {
        max_line_len *=2;
        line = (char *)realloc(line,max_line_len);
        len  = (int)strlen(line);
        if (fgets(line+len,max_line_len,input)==NULL)
            break;
    }
    return line;
}

void read_index2df(const char * filename)
{
    int featuresize=0;
    char *endptr;
    FILE *fp=fopen(filename,"r");
     if(fp==NULL)
    {
        fprintf(stderr,"can't open input file %s\n",filename);
        exit(1);
    }
    max_line_len=1024;
    line = Malloc(char,max_line_len);
    while(readline(fp)!=NULL)
    {
        if(featuresize==0)
        {
            char *p= strtok(line," \t");
            featuresize = (int)strtol(p,&endptr,10);
            inv_space=Malloc(struct feature_node ,featuresize+1);
            prob.n=featuresize;
            prob.index2df=inv_space;
        }
        else
        {
            char *p= strtok(line," \t");
            int findex = (int)strtol(p,&endptr,10);
            inv_space[findex].index=findex;
            p= strtok(NULL," \t");
            inv_space[findex].value=strtod(p,&endptr);
        }
    }
}

 void read_cal_problem(const char * filename)
{
    int i;
    double linetfcount;//一个文档总共有的词的个数,用作tf计算...
    long int elements,j;
    FILE *fp = fopen(filename,"r");
    char *endptr;
    char *idx,*val;
    if(fp == NULL)
    {
        fprintf(stderr,"can't open input file %s\n",filename);
        exit(1);
    }
    prob.l=0;
    elements=0;
     while(readline(fp)!=NULL)
    {
        char *p = strtok(line," \t");//mediaid
         while(1)
        {
            p=strtok(NULL," \t"); 
            if(p==NULL || *p=='\n')
                break;
            elements++;
        }
        prob.l++;
    }
    rewind(fp);

    prob.y=Malloc(int,prob.l);
    prob.x=Malloc(struct feature_node *,prob.l) ;
    x_space=Malloc(struct feature_node,elements);
    
    j=0;
    for(i=0;i<prob.l;i++)
    {
        readline(fp);
        prob.x[i]=&x_space[j];
        val = strtok(line," \t\n");
        if(val==NULL)
            exit_input_error(i+1);
        prob.y[i] = strtod(val,&endptr);
        val = strtok(NULL," \t");
        if(val==NULL)
            exit_input_error(i+1);
        linetfcount  = strtod(val,&endptr);
         while(1)
        {
            idx = strtok(NULL,":"  );
            val = strtok(NULL," \t");
            if(val==NULL)
                break;
            int tempindex = (int) strtol(idx,&endptr,10);
            x_space[j].index = tempindex;
            x_space[j].value = (int)(10000*(strtod(val,&endptr)/linetfcount)*log(prob.l/(1.0*inv_space[tempindex].value+1)));
            j++;
        }
         x_space[j++].index = -1;
    }
    fclose(fp);
    

}//end read problem...

int save_tfidfmodel(const char *tfidf_model_file)
{
    int j=0;
    FILE *fp = fopen(tfidf_model_file,"w");
     for(int i=0;i<prob.l;i++)
    {
        fprintf(fp,"%d ",prob.y[i]);
         while(x_space[j].index!=-1)
        {
                fprintf(fp,"%d:%d ",x_space[j].index,x_space[j].value);
                j++;
        }
        fprintf(fp,"\n");
        j++;
    }
    fclose(fp);
}

 int main(int argc , char **argv)
{
    char input_file_name  [1024];
    char word2df_file_name[1024];
    char output_file_name[1024] ;
    if(argc!=4)
    {
        fprintf(stderr,"tfidf formatoutput worddfcount outputweight!\n");
        exit(1);
    }
    else
    {
        strcpy(input_file_name,argv[1])  ;
        strcpy(word2df_file_name,argv[2]);
        strcpy(output_file_name,argv[3]) ;

        read_index2df(word2df_file_name) ;
        read_cal_problem(input_file_name);   
        save_tfidfmodel(output_file_name);

        free(prob.y);
        free(prob.x);
        free(x_space);
        free(inv_space);
        free(line);
    }
    return 0;
}
