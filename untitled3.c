#include <stdio.h>
#include<stdlib.h>
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
	struct Node *previous;
	struct Node *lchild;
	struct Node *rchild;
}*BiTree,BiNode;

void InitBiTree(BiTree *T){
	*T=(BiTree)malloc(sizeof(BiTree));
	(*T)->previous=NULL;
	(*T)->lchild=(BiTree)malloc(sizeof(BiTree));
	(*T)->rchild=(BiTree)malloc(sizeof(BiTree));
}

void DestroyBiTree(BiTree *T){
	if(*T){
		if((*T)->lchild){
			DestroyBiTree(&((*T)->lchild));
		}
		if((*T)->rchild){
			DestroyBiTree(&((*T)->rchild));
		}
		if((*T)->previous){
			DestroyBiTree(&((*T)->previous));
		}
		free(*T);
		*T=NULL;
	}
}

BiTree Point(BiTree T,DataType e){
	BiTree Q[MaxSize];
	int front=0,rear=0;
	BiNode *p;
	if(T){
		Q[rear]=T;
		rear++;
		while(front!=rear){
			p=Q[front];
			front++;
			if(p->data==e){
				return p;
			}
			if(p->lchild){
				Q[rear]=p->lchild;
				rear++;
			}
			if(p->rchild){
				Q[rear]=p->rchild;
				rear++;
			}
		}
	}
	return NULL;
}

DataType LeftChild(BiTree T,DataType e){
	BiTree p;
	if(T){
		p=Point(T,e);
		if(p&&p->lchild){
			return p->lchild->data;
		}
	}
	return;
}

DataType RightChild(BiTree T,DataType e){
	BiTree p;
	if(T){
		p=Point(T,e);
		if(p&&p->rchild){
			return p->rchild->data;
		}
	}
	return;
}






void CreateBiTree(LinkStack M,LinkStack S,BiTree *p){
	//当需要对符号进行出栈时进入二叉树构建
	DataType sign,number;
	PopStack(S,&sign);
	BiTree T;
	char ch;
	InitBiTree(&T);
	T->previous=*p;
	T->data=sign;
	if(!StackEmpty(M)&&GetTop(M,&ch)&&ch>='0'&&ch<='9'){
		PopStack(M,&number);
		T->lchild->data=number;
	}
	else{
		T->lchild=T->previous;
		T->previous=T->previous->previous;
		if(ch<'0'||ch>'9'){
			PopStack(M,&number);
		}
	}
	if(!StackEmpty(M)&&GetTop(M,&ch)&&ch>='0'&&ch<='9'){
		PopStack(M,&number);			
		T->rchild->data=number;
	}
	else{
		T->rchild=T->previous;
		T->previous=T->previous->previous;
		if(ch<'0'||ch>'9'){
			PopStack(M,&number);
		}
	}	
	PushStack(M,sign);
	*p=T;
}


BiTree TranslateExpress(char str[]){
	LinkStack S;
	LinkStack M;
	BiTree exp;
	
	char ch;
	DataType e;
	int i=0;
	
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
			while(ch>='0'&&ch<='9'){
				PushStack(M,ch);
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
	if(NULL != T){
		InOrderTraverse(T->lchild);
		printf("%2c",T->data);
		InOrderTraverse(T->rchild);
	}
}

void PostOrderTraverse(BiTree T){
	if(NULL != T){
		InOrderTraverse(T->lchild);
		InOrderTraverse(T->rchild);
		printf("%2c",T->data);
	}
}

int main(){
	char a[MaxSize];
	BiTree exp;
	printf("请输入算式\n");
	gets(a);
	exp=TranslateExpress(a);
	printf("中序表达式\n");
	InOrderTraverse(exp);
	printf("后序表达式\n");
	PostOrderTraverse(exp);
}

