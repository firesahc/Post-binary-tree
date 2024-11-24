#include <stdio.h>
#include <stdlib.h>
#define MaxSize 50






typedef struct Snode{
	char sign;
	struct Snode *next;
}SLStackNode,*SLinkStack;

void SInitStack(SLinkStack *top){
	if((*top=(SLinkStack)malloc(sizeof(SLStackNode)))==NULL){
		exit(-1);
	}
	(*top)->next=NULL;
}

int SStackEmpty(SLinkStack top){
	if(top->next==NULL){
		return 1;
	}
	else
		return 0;
}

int SPushStack(SLinkStack top,char e){
	SLStackNode *p;
	if((p=(SLStackNode*)malloc(sizeof(SLStackNode)))==NULL){
		printf("内存分配失败");
		exit(-1);
	}
	p->sign=e;
	p->next=top->next;
	top->next=p;
	return 1;
}

int SPopStack(SLinkStack top,char *e){
	SLStackNode *p;
	p=top->next;
	if(!p){
		printf("栈已空");
		return 0;
	}
	top->next=p->next;
	*e=p->sign;
	free(p);
	return 1;
}

int SGetTop(SLinkStack top,char *e){
	SLStackNode *p;
	p=top->next;
	if(!p){
		printf("栈已空");
		return 0;
	}
	*e=p->sign;
	return 1;
}






typedef struct Nnode{
	double number;
	char tag;
	struct Nnode *next;
}NLStackNode,*NLinkStack;

void NInitStack(NLinkStack *top){
	if((*top=(NLinkStack)malloc(sizeof(NLStackNode)))==NULL){
		exit(-1);
	}
	(*top)->next=NULL;
}

int NStackEmpty(NLinkStack top){
	if(top->next==NULL){
		return 1;
	}
	else
		return 0;
}

int NPushStack(NLinkStack top,double e,char t){
	NLStackNode *p;
	if((p=(NLStackNode*)malloc(sizeof(NLStackNode)))==NULL){
		printf("内存分配失败");
		exit(-1);
	}
	p->number=e;
	p->tag=t;
	p->next=top->next;
	top->next=p;
	return 1;
}

int NPopStack(NLinkStack top,double *e){
	NLStackNode *p;
	p=top->next;
	if(!p){
		printf("栈已空");
		return 0;
	}
	top->next=p->next;
	*e=p->number;
	free(p);
	return 1;
}

int NGetTop(NLinkStack top,double *e,char *t){
	NLStackNode *p;
	p=top->next;
	if(!p){
		printf("栈已空");
		return 0;
	}
	*e=p->number;	
	*t=p->tag;
	return 1;
}






typedef struct Node{
	char sign;
	double number;
	struct Node *previous;
	struct Node *lchild;
	struct Node *rchild;
}*BiTree,BiNode;

void InitBiTree(BiTree *T){
	*T=NULL;
	*T=(BiTree)malloc(sizeof(BiTree));
	(*T)->previous=NULL;
	(*T)->lchild=NULL;
	(*T)->rchild=NULL;
}






void CreateBiTree(NLinkStack M,SLinkStack S,BiTree *p){
	//当需要对符号进行出栈时进入二叉树构建
	char sign,tag;
	double ch,number;
	BiTree T;
	
	InitBiTree(&T);
	SPopStack(S,&sign);
	
	T->previous=*p;
	T->sign=sign;
	
	if(!NStackEmpty(M)&&NGetTop(M,&ch,&tag)&&tag!='+'){
		NPopStack(M,&number);
		InitBiTree(&(T->lchild));
		T->lchild->number=number;
	}
	else{
		T->lchild=T->previous;
		T->previous=T->previous->previous;
		if(tag=='+'){
			NPopStack(M,&number);
		}
	}
	if(!NStackEmpty(M)&&NGetTop(M,&ch,&tag)&&tag!='+'){
		NPopStack(M,&number);
		InitBiTree(&(T->rchild));
		T->rchild->number=number;
	}
	else{
		T->rchild=T->previous;
		T->previous=T->previous->previous;
		if(tag=='+'){
			NPopStack(M,&number);
		}
	}	
	NPushStack(M,0.0,'+');
	*p=T;
}


BiTree TranslateExpress(char str[]){
	char ch,e;
	double x1,x2,value;
	int i=0,j;
	
	SLinkStack S;
	NLinkStack M;
	BiTree exp;
	
	SInitStack(&S);
	NInitStack(&M);
	InitBiTree(&exp);
	
	ch=str[i];
	i++;
	while(ch!='\0'){
		switch (ch) {
		case '(':
			SPushStack(S,ch);
			break;
		case ')':
			while(SGetTop(S,&e)&&e!='('){
				CreateBiTree(M,S,&exp);//右括号导致符号出栈，进入构建二叉树
			}
			SPopStack(S,&e);
			break;
		case '+':
		case '-':
			while(!SStackEmpty(S)&&SGetTop(S,&e)&&e!='('){
				CreateBiTree(M,S,&exp);//加减号导致符号出栈，进入构建二叉树
			}
			SPushStack(S,ch);
			break;
		case '*':
		case '/':
			SPushStack(S,ch);
			break;
		case ' ':
			break;
		default:
			j=0;
			while(ch>='0'&&ch<='9'){
				NPushStack(M,ch-'0','0');
				j++;
				while(j>1){
					NPopStack(M,&x2);
					NPopStack(M,&x1);
					value=10*x1+x2;
					j--;
					NPushStack(M,value,'0');
				}
				ch=str[i];
				i++;
			}
			i--;
		}
		ch=str[i];
		i++;
	}
	while(!SStackEmpty(S)){
		CreateBiTree(M,S,&exp);	//录入完毕导致符号出栈，进入构建二叉树
	}
	return exp;
}

void InOrderTraverse(BiTree T){
	if(T){
		InOrderTraverse(T->lchild);
		if(T->sign=='\r'){
			printf("%f",T->number);	
			printf("%c",' ');
		}
		else{
			printf("%-2c",T->sign);
		}
		InOrderTraverse(T->rchild);
	}
}

void PostOrderTraverse(BiTree T){
	if(T){
		PostOrderTraverse(T->lchild);
		PostOrderTraverse(T->rchild);
		if(T->sign=='\r'){
			printf("%f",T->number);	
			printf("%c",' ');
		}
		else{
			printf("%-2c",T->sign);
		}
	}
}

float ComputeExpress(BiTree T,NLinkStack M){
	int i=0;
	double x1,x2,result;
	if(T){
		ComputeExpress(T->lchild,M);
		ComputeExpress(T->rchild,M);
		if(T->sign=='\r'){
			NPushStack(M,T->number,'0');
		}
		else{
			switch (T->sign) {
			case '+':
				NPopStack(M,&x1);
				NPopStack(M,&x2);
				result=x1+x2;
				NPushStack(M,result,'0');
				break;
			case '-':
				NPopStack(M,&x1);
				NPopStack(M,&x2);
				result=x1-x2;
				NPushStack(M,result,'0');
				break;
			case '*':
				NPopStack(M,&x1);
				NPopStack(M,&x2);
				result=x1*x2;
				NPushStack(M,result,'0');
				break;
			case '/':
				NPopStack(M,&x1);
				NPopStack(M,&x2);
				result=x1/x2;
				NPushStack(M,result,'0');
				break;
			}
			i++;
		}
	}
	return result;
}

int main(void){
	char a[MaxSize];
	double result;
	BiTree exp;
	NLinkStack M;
	NInitStack(&M);
	
	printf("请输入算式\n");
	gets(a);
	exp=TranslateExpress(a);
	printf("中序表达式\n");
	InOrderTraverse(exp);
	printf("\n");
	printf("后序表达式\n");
	PostOrderTraverse(exp);	
	printf("\n");
	printf("计算结果\n");
	result=ComputeExpress(exp,M);
	printf("%f",result);
	
	return 0;
}
