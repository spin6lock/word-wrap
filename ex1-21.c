/* algorithm like knuth's */
#include <stdio.h>
#include <stdlib.h>

#define whitespaces(x) ( (x)=='\t' || (x)==' ' || (x)=='\n' )
#define square(x) (x)*(x)
#define MAXLINE 1000
#define IN 1
#define OUT 0

char line[MAXLINE];
int wordLen[MAXLINE];
int wordStart[MAXLINE];
int arr[MAXLINE][MAXLINE];/* store the cost value*/
int choose[MAXLINE][MAXLINE];

int mygetline(char line[],int maxline);
int countWord(char* arr,int* len,int* begin);
int cost(int len[],int i,int j,int width);
void printfWord(int from,int to);

int main(int argc,char* argv[]){
	if (argc!=2){
		printf("usage: width\n");
		return -1;
	}
	int width=atoi(argv[1]);
	int len,i,j,wordCount,tmp,k,min,cho;
	for (i=0;i<width;i++)
		printf("*");
	printf("\n");
	while ( (len=mygetline(line,MAXLINE)) > 0){
		wordCount=countWord(line,wordLen,wordStart);
		if (wordCount!=0){
			for (i=wordCount;i>0;i--){/* building the cost matrix */
				for (j=0;j<i;j++){
					tmp=cost(wordLen,j,j+wordCount-i,width);
					if (tmp!=-1){
						arr[j][j+wordCount-i]=tmp;
						choose[j][j+wordCount-i]=j+wordCount-i;
					}
					else{
						min=65535;
						for (k=j;k<j+wordCount-i;k++){
							if (arr[j][k]+arr[k+1][j+wordCount-i]<min){
								min=arr[j][k]+arr[k+1][j+wordCount-i];
								cho=k;
							}
						}
						arr[j][j+wordCount-i]=min;
						choose[j][j+wordCount-i]=cho;
					}
				}
			}
			/*
			for(i=0;i<wordCount;i++){
				for (j=0;j<wordCount;j++)
					printf("%5d",choose[i][j]);
				printf("\n");
			}*/
			printfWord(0,wordCount-1);
		}
	}
	
	return 0;
}

/* read a line limited in 'limit' and stored it in s[] ,return the len
 * */
int mygetline(char line[],int maxline){
	int i=0;
	char c;
	while (i<maxline-1 && (c=getchar())!=EOF && c!='\n'){
		line[i]=c;
		i++;
	}
	if (c=='\n'){
		line[i]='\n';
		i++;
	}
	line[i] = '\0';
	return i;
}

int countWord(char* arr,int* len,int* begin){
	char* line0=arr;
	int wordNum=0;
	int state=OUT;
	while (*arr != '\0'){
		if (state == OUT){
			if ( whitespaces(*arr))
				;
			else{
				*begin=arr-line0;
				arr--;
				state=IN;
				*len=0;
			}
		}
		else{/* state == IN */
			if (whitespaces(*arr)){
				len++;
				begin++;
				state=OUT;
				wordNum++;
			}
			else{
				(*len)++;
			}
		}
		arr++;
	}
	return wordNum;
}

int cost(int len[],int i,int j,int width){
	int space=(j-i);
	int k;
	for (k=i;k<=j;k++){
		space+=len[k];
	}
	return (width - space) >= 0?square(width - space): -1;
}

void printfWord(int from,int to){
	int i;
	if (from > to){
		printf("bug: %.*s",wordLen[to],line+wordStart[to]);
		return;
	}
	if (choose[from][to]==to){
		for(i=from;i<to+1;i++)
			printf("%.*s ",wordLen[i],line+wordStart[i]);
		printf("\n");
	}
	else{
		printfWord(from,choose[from][to]);
		printfWord(choose[from][to]+1,to);
	}
}
