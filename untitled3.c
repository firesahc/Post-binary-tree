#include <stdio.h>
#include <stdlib.h>
#define MaxSize 50
typedef char DataType;






typedef struct node{
	DataType data;
	struct node *next;
}LStackNode,*LinkStack;

void InitStack(LinkStack *top){
	if((*top=(LinkStack)malloc(sizeof(LStackNode)))==NULL){
		exit(-1);
	}
	(*top)->next=NULL;
}

int StackEmpty(LinkStack top){
	if(top->next==NULL){
		return 1;
	}
	else
		return 0;
}

int PushStack(LinkStack top,DataType e){
	LStackNode *p;
	if((p=(LStackNode*)malloc(sizeof(LStackNode)))==NULL){
		printf("内存分配失败");
		exit(-1);
	}
	p->data=e;
	p->next=top->next;
	top->next=p;
	return 1;
}

int PopStack(LinkStack top,DataType *e){
	LStackNode *p;
	p=top->next;
	if(!p){
		printf("栈已空");
		return 0;
	}
	top->next=p->next;
	*e=p->data;
	free(p);
	return 1;
}

int GetTop(LinkStack top,DataType *e){
	LStackNode *p;
	p=top->next;
	if(!p){
		printf("栈已空");
		return 0;
	}
	*e=p->data;
	return 1;
}






typedef struct Node{
	DataType data;
	char type;
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






void CreateBiTree(LinkStack M,LinkStack S,BiTree *p){
	//当需要对符号进行出栈时进入二叉树构建
	char ch;
	DataType sign,number;
	BiTree T;
	
	InitBiTree(&T);
	PopStack(S,&sign);
	
	T->previous=*p;
	T->data=sign;
	T->type='+';
	
	if(!StackEmpty(M)&&GetTop(M,&ch)&&ch!=-128){
		PopStack(M,&number);
		InitBiTree(&(T->lchild));
		T->lchild->data=number;
		T->lchild->type='1';
	}
	else{
		T->lchild=T->previous;
		T->previous=T->previous->previous;
		if(ch==-128){
			PopStack(M,&number);
		}
	}
	if(!StackEmpty(M)&&GetTop(M,&ch)&&ch!=-128){
		PopStack(M,&number);
		InitBiTree(&(T->rchild));
		T->rchild->data=number;
		T->rchild->type='1';
	}
	else{
		T->rchild=T->previous;
		T->previous=T->previous->previous;
		if(ch==-128){
			PopStack(M,&number);
		}
	}	
	PushStack(M,-128);
	*p=T;
}


BiTree TranslateExpress(char str[]){
	char ch,x1,x2,value;
	int i=0,j;
	DataType e;
	
	LinkStack S;
	LinkStack M;
	BiTree exp;
	
	InitStack(&S);
	InitStack(&M);
	InitBiTree(&exp);
	
	ch=str[i];
	i++;
	while(ch!='\0'){
		switch (ch) {
		case '(':
			PushStack(S,ch);
			break;
		case ')':
			while(GetTop(S,&e)&&e!='('){
				CreateBiTree(M,S,&exp);//右括号导致符号出栈，进入构建二叉树
			}
			PopStack(S,&e);
			break;
		case '+':
		case '-':
			while(!StackEmpty(S)&&GetTop(S,&e)&&e!='('){
				CreateBiTree(M,S,&exp);//加减号导致符号出栈，进入构建二叉树
			}
			PushStack(S,ch);
			break;
		case '*':
		case '/':
			PushStack(S,ch);
			break;
		case ' ':
			break;
		default:
			j=0;
			while(ch>='0'&&ch<='9'){
				PushStack(M,ch-'0');
				j++;
				while(j>1){
					PopStack(M,&x2);
					PopStack(M,&x1);
					value=10*x1+x2;
					j--;
					PushStack(M,value);
				}
				ch=str[i];
				i++;
			}
			i--;
		}
		ch=str[i];
		i++;
	}
	while(!StackEmpty(S)){
		CreateBiTree(M,S,&exp);	//录入完毕导致符号出栈，进入构建二叉树
	}
	return exp;
}

void InOrderTraverse(BiTree T){
	if(T){
		InOrderTraverse(T->lchild);
		if(T->type=='1'){
			printf("%d",T->data);	
			printf("%c",' ');
		}
		else{
			printf("%-2c",T->data);
		}
		InOrderTraverse(T->rchild);
	}
}

void PostOrderTraverse(BiTree T){
	if(T){
		PostOrderTraverse(T->lchild);
		PostOrderTraverse(T->rchild);
		if(T->type=='1'){
			printf("%d",T->data);	
			printf("%c",' ');
		}
		else{
			printf("%-2c",T->data);
		}
	}
}

float ComputeExpress(BiTree T,LinkStack M){
	int i=0;
	char x1,x2;
	float result;
	if(T){
		ComputeExpress(T->lchild,M);
		ComputeExpress(T->rchild,M);
		if(T->type=='1'){
			PushStack(M,T->data);
		}
		else{
			switch (T->data) {
			case '+':
				PopStack(M,&x1);
				PopStack(M,&x2);
				result=x1+x2;
				PushStack(M,result);
				break;
			case '-':
				PopStack(M,&x1);
				PopStack(M,&x2);
				result=x1-x2;
				PushStack(M,result);
				break;
			case '*':
				PopStack(M,&x1);
				PopStack(M,&x2);
				result=x1*x2;
				PushStack(M,result);
				break;
			case '/':
				PopStack(M,&x1);
				PopStack(M,&x2);
				result=(float)x1/(float)x2;
				PushStack(M,result);
				break;
			}
			i++;
		}
	}
	return result;
}

int main(void){
	char a[MaxSize];
	float result;
	BiTree exp;
	LinkStack M;
	InitStack(&M);
	
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
