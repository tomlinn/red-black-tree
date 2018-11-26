#include <stdio.h>
#include <stdlib.h>
struct RedBlackNode{
	int value;
	char color;
	struct RedBlackNode *right;
	struct RedBlackNode *left;
	struct RedBlackNode *parent;
};typedef struct RedBlackNode RBNode;

RBNode *nil;
//左旋轉 
RBNode *LEFT_ROTATE(RBNode *root,RBNode *x){
	RBNode *y=x->right;
	x->right=y->left;
	if(y->left!=nil){
		y->left->parent=x;
	}
	y->parent=x->parent;
	if(x->parent==nil){
		root=y;
	}else if(x==x->parent->left){
		x->parent->left=y;
	}else{
		x->parent->right=y;
	}
	y->left=x;
	x->parent=y;
	return root;
}
//右旋轉 
RBNode *RIGHT_ROTATE(RBNode *root,RBNode *x){
	RBNode *y=x->left;
	x->left=y->right;
	if(y->right!=nil){
		y->right->parent=x;
	}
	y->parent=x->parent;
	if(x->parent==nil){
		root=y;
	}else if(x==x->parent->right){
		x->parent->right=y;
	}else{
		x->parent->left=y;
	}
	y->right=x;
	x->parent=y;
	return root;
}

//用樹v代替樹u 
RBNode *RB_TRANSPLANT(RBNode *root,RBNode *u,RBNode *v){
	if(u->parent==nil){
		root=v;
	}else if(u==u->parent->left){
		u->parent->left=v;
	}else{
		u->parent->right=v;
	}
	v->parent=u->parent;
	return root;
}
//查找結點z下的最小值的結點 
RBNode *TREE_MINIMUM(RBNode	 *z){
	RBNode *rs=z;
	while(rs->left!=nil){
		rs=rs->left;
	}
	return rs;
}
//刪除後對紅黑樹進行調整 
RBNode *RB_DELETE_FIXUP(RBNode *root,RBNode *x){
	RBNode *w = nil;
	while(x!=root && x->color=='B'){
		if(x==x->parent->left){
			w = x->parent->right;
			if(w->color=='R'){
				w->color='B';                    //case 1	 //將sibling塗成黑色
				x->parent->color='R';						 //將current之parent塗成紅色
				root = LEFT_ROTATE(root,x->parent);			 //對current之parent做Left Rotation
				w=x->parent->right;							 //將sibling移動到current->parent的rightchild
			}
			if(w->left->color=='B' && w->right->color=='B'){
				w->color='R';                 //case 2 //將sibling塗成紅色
				x=x->parent;						   //將current移至currnet的parent
			}else {
				if(w->right->color=='B'){			  
					w->left->color='B';     //case 3  //將sibling之leftchild塗成黑色
					w->color='R';					  //將sibling塗成紅色
					root = RIGHT_ROTATE(root,w);	  //對sibling進行Right Rotation
					w = x->parent->right;			  //將sibling移至current->parent的rightchild
				}
				w->color=x->parent->color;	//case 4//將sibling塗成current之parent的顏色 //case 4
				x->parent->color='B'; 				//將parent塗成黑色
				w->right->color='B';				//將sibling之rightchild塗成黑色
				root = LEFT_ROTATE(root,x->parent);	//對parent進行Left Rotation
				x = root;
			}
		}else {
			RBNode *w = x->parent->left;
			if(w->color=='R'){
				w->color='B';
				x->parent->color='R';
				root = RIGHT_ROTATE(root,x->parent);
				w=x->parent->left;
			}
			if(w->right->color=='B' && w->left->color=='B'){
				w->color='R';
				x=x->parent;
			}else {
				if(w->left->color=='B'){
					w->right->color='B';
					w->color='R';
					root = LEFT_ROTATE(root,w);
					w = x->parent->left;
				}
				w->color=x->parent->color;
				x->parent->color='B';
				w->left->color='B';
				root = RIGHT_ROTATE(root,x->parent);
				x = root;
			}
		}
	}
	x->color='B';
	return root;
}
int Height(RBNode *root){
	if(root==nil){
		return 0;
	}else{
		int lHeight=Height(root->left);
		int rHeight=Height(root->right);
		if(lHeight>rHeight){
			return lHeight+1;
		}else{
			return rHeight+1;
		}
	}
}
//刪除紅黑樹中某一結點 
RBNode *RB_DELETE(RBNode *root,RBNode *z){
	RBNode *y=z;
	RBNode *x=nil;
	char Y_ORIGINAL_COLOR=y->color;
	if(z->left==nil){
		x = z->right;
		root=RB_TRANSPLANT(root,z,z->right);
	}else if(z->right==nil){
		x = z->left;
		root = RB_TRANSPLANT(root,z,z->left);
	}else{
		y=TREE_MINIMUM(z->right);
		Y_ORIGINAL_COLOR=y->color;
		x=y->right;
		if(y->parent==z){
			x->parent=y;
		}else{
			root = RB_TRANSPLANT(root,y,y->right);
			y->right=z->right;
			y->right->parent=y;
		}
		root = RB_TRANSPLANT(root,z,y);
		y->left=z->left;
		y->left->parent=y;
		y->color=z->color;
	}
	if(Y_ORIGINAL_COLOR=='B'){
		root = RB_DELETE_FIXUP(root,x);
	}
	return root;
}
//插入某結點後的調整
RBNode *RB_INSERT_FIXUP(RBNode *root,RBNode *z){
	while(z->parent->color=='R'){
		if(z->parent==z->parent->parent->left){
			RBNode *y=z->parent->parent->right;
			if(y->color=='R'){               //case1
				z->parent->color='B';		   //將parent塗成黑色
				y->color='B';				   //將uncle塗成黑色
				z->parent->parent->color='R';  //將parent->parent塗成紅色
				z=z->parent->parent;		   //將current從nodeA移到nodeZ
			}else{                               
				if(z==z->parent->right){     //case2 左旋變case3
					z=z->parent;				   //將current移至current->parent
					root = LEFT_ROTATE(root,z);	   //對新的current進行Left Rotation
				} 
				z->parent->color='B';        //case3		  //將parent塗成黑色
				z->parent->parent->color='R';				  //將parent->parent塗成紅色
				root = RIGHT_ROTATE(root,z->parent->parent);  //對parent->parentnode(Z)進行Right Rotation
			}
		}else {
			RBNode *y=z->parent->parent->left;
			if(y->color=='R'){
				z->parent->color='B';
				y->color='B';
				z->parent->parent->color='R';
				z=z->parent->parent;
			}else{
				if(z==z->parent->left){
					z=z->parent;
					root = RIGHT_ROTATE(root,z);
				}
				z->parent->color='B';
				z->parent->parent->color='R';
				root = LEFT_ROTATE(root,z->parent->parent);
			}
		}
	}
	root->color='B';
	return root;
}
//插入一個節點 
RBNode *RB_INSERT(RBNode *root,RBNode *z){
	RBNode *x=root,*y=nil;
	while(x!=nil){
		y=x;
		if(z->value<x->value){
			x=x->left;
		}else{
			x=x->right;
		}
	}
	z->parent=y;
	if(y==nil){
		root=z;
	}else if(z->value<y->value){
		y->left=z;
	}else{
		y->right=z;
	}
	z->left=nil;
	z->right=nil;
	z->color='R';
	root = RB_INSERT_FIXUP(root,z);
	return root;
}
//中序遍歷樹
void inOrder(RBNode	*tree) {
	if (tree != nil) {
		inOrder(tree->left);
		printf("%d,%c\n",tree->value,tree->color);
		inOrder(tree->right);
	}
} 
void postOrder(RBNode	*tree) {
	if (tree != nil) {
		postOrder(tree->left);
		postOrder(tree->right);
		printf("%d,%c\n",tree->value,tree->color);
	}
} 
//選擇一個節點
RBNode	*selectNode(RBNode	*root,int key){
	RBNode	*temp = root;
	while (temp!=nil) {
		if (temp->value	>key) {
			temp=temp->left;
		}else if (temp->value<key) {
			temp=temp->right;
		}else {
			return temp;
		}
	}
	return temp;
} 
int main(int argc, char *argv[])
{
	nil=(RBNode *)malloc(sizeof(RBNode));
	nil->color='B';
	RBNode *root = nil;
	FILE * pfile;
	int a[30];
	int n=0;
	pfile=fopen("rbtree.txt","r");
	if(pfile!=NULL){
		while(!feof(pfile)){
			fscanf(pfile," %d ",&a[n]);
			n++;
		}
		//int a[] = { 12, 4, 13, 2, 14, 7, 6, 10, 5};
		int i = 0;
		for (i=0; i < n; i++) {
			RBNode *z = (RBNode *)malloc(sizeof(RBNode));
			z->value = a[i];
			root = RB_INSERT(root, z);
		}
		RBNode *temp =root;
		printf("inOrder:\n");
		inOrder(temp);
		printf("postOrder:\n");
		postOrder(temp);
		printf("Height:%d\n",Height(temp)-1);
		while(1){
			int c,t;
			RBNode *y;
			printf(" 1. insert 2.delete:");
			scanf("%d",&c);
			switch (c){
			case 1:
				printf("insert:");
				scanf("%d",&t);
				if(t==-1)break;
				y = (RBNode *)malloc(sizeof(RBNode));
				y->value = t;
				root = RB_INSERT(root, y);
				break;
			case 2:
				printf("delete:");
				scanf("%d",&t);
				if(t==-1)break;
				RBNode *deleteNode = selectNode(root,t);
				root = RB_DELETE(root,deleteNode);
				break;
			}
			printf("inOrder:\n");
			inOrder(temp);
			printf("postOrder:\n");
			postOrder(temp);
			printf("Height:%d\n",Height(temp)-1);

		}
		//printf("\n測試刪除根節點：\n");
		//RBNode *deleteNode = selectNode(root,12);
		//root = RB_DELETE(root,deleteNode);
		//inOrder(root);
	}
	system("pause");
	return 0;
}