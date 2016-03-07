#include<stdio.h>
#include<string.h>
#include<stdlib.h>
int main(){
	printf("Paint the function!\n");
	printf("print\'#\' to leave\n");
	char a[1001];
	FILE *fp=fopen("functionset.h","w");
	int n=0;
	while (1){
		printf("y=");
		fgets(a,1000,stdin);
		a[strlen(a)-1]=';';
		if (strstr(a,"#")!=NULL)  break;
		fprintf(fp,"double f%d(double x){\n",++n);
		fprintf(fp,"return ");
		fprintf(fp,"%s\n}\n",a);
	}
	fclose(fp);

	fp=fopen("getbmp.c","w");
	FILE *fp1=fopen("board","r");

	while (fgets(a,1000,fp1)!=NULL){
		if (strstr(a,"$$")!=NULL) break;
		else fprintf(fp,"%s",a);
	}
	int i;
	for (i=1;i<=n;i++){
		fprintf(fp,"paint(a,b,y1,y2,width,height,f%d);\n",i);
	}	

	while (fgets(a,1000,fp1)!=NULL)
		fprintf(fp,"%s",a);
	fclose(fp);
	fclose(fp1);

	system("gcc getbmp.c -o getbmp -lm");
	system("./getbmp");
	return 0;
}
