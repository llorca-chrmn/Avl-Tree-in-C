#include<stdio.h>
#include<stdlib.h>

typedef struct avlNode {
	struct avlNode* left;
	struct avlNode* right;
	int key;
	int height;
}*avl;

void initAVL(avl *tree);
void insertAVL(avl *tree, int data);
void balanceTree(avl *tree);
int getHeight(avl tree);
void rotateAVL(avl *root, int state); 
void updateHeight(avl tree);
int deleteMin(avl *tree);
void deleteAVL(avl * tree, int data);
int searchAVL(avl tree, int data);
void displayInorder(avl tree);

int main(void)
{
	avl tree;
	int choice;
	int val, check;
	
	initAVL(&tree);
	
	do {
		printf("\n\n-----------------AVL----------------\n\n");
		printf("\t1] Insert\n");
		printf("\t2] Delete\n");
		printf("\t3] Search\n");
		printf("\t4] Display In Order\n");
		printf("\t5] Exit\n");
		
		printf("\n\tPlease make a choice: ");
		fflush(stdin);
		scanf("%d", &choice);
		
		switch(choice) {
			case 1:
				printf("\n\tEnter a number to insert: ");
				scanf("%d", &val);
				insertAVL(&tree, val);
				break;
			case 2:
				printf("\n\tEnter a number to delete: ");
				scanf("%d", &val);
				deleteAVL(&tree, val);
				break;
			case 3:
				printf("\n\tWhat will you search for? ");
				scanf("%d", &val);
				check = searchAVL(tree, val);
				if(check == 1) {
					printf("\n\t%d exists in the tree!\n", val);
				} else {
					printf("\n\t%d does not exist in the tree!\n", val);
				}
				break;
			case 4:
				printf("\n\tValues in the tree:\n\t");
				displayInorder(tree);
				break;
			case 5:
				printf("\n\tThank you! Now exiting...");
				break;
			default:
				printf("\n\tInvalid input! Try again!\n");
				
		}
		
		getch();
		system("cls");
	} while(choice != 5);
	
	return 0;
}

void initAVL(avl *tree)
{
	*tree = NULL;
}

void insertAVL(avl *tree, int data)
{
	if(*tree == NULL) {
		*tree = (avl) malloc (sizeof(struct avlNode));
		if(*tree != NULL) {
			(*tree)->left = NULL;
			(*tree)->right = NULL;
			(*tree)->key = data;
			(*tree)->height = 1;
			printf("\n\tInserted Successfully!\n");
		} else {
			printf("\n\tThere was a problem allocating memory.");
		}
	} else if((*tree)->key != data) {
		if((*tree)->key < data) {
			insertAVL(&(*tree)->right, data);
		} else {
			insertAVL(&(*tree)->left, data);
		}
		
		balanceTree(tree);
	} else {
		printf("\n\tValue already exists!");
	}
}

void balanceTree(avl *tree)
{
	int state;
	
	state = 0;
	if(*tree != NULL) {
		if(getHeight((*tree)->left) > getHeight((*tree)->right) + 1) {
			if(getHeight((*tree)->left->left) > getHeight((*tree)->left->right)) {
				rotateAVL(tree, state);
			} else {
				rotateAVL(&(*tree)->left, 1);
				rotateAVL(tree, state);
			}
		} else {
			state++;
		}
		
		if(state == 1) {
			if(getHeight((*tree)->right) > getHeight((*tree)->left) + 1) {
				if(getHeight((*tree)->right->right) > getHeight((*tree)->right->left)) {
					rotateAVL(tree, state);
				} else {
					rotateAVL(&(*tree)->right, 0);
					rotateAVL(tree, state);
				}
			} else {
				state++;
			}
		}
		
		if(state == 2) {
			updateHeight(*tree);
		}
	}
}

int getHeight(avl tree)
{
	int h;
	
	if(tree != NULL) {
		h = tree->height;
	} else {
		h = 0;
	}
	
	return h;
}

void rotateAVL(avl *root, int state)
{
	avl oldRoot, newRoot, oldMiddle;
	
	oldRoot = *root;
	if(state == 0) {
		newRoot = oldRoot->left;
		oldMiddle = newRoot->right;
		
		oldRoot->left = oldMiddle;
		newRoot->right = oldRoot;
	} else {
		newRoot = oldRoot->right;
		oldMiddle = newRoot->left;
		
		oldRoot->right = oldMiddle;
		newRoot->left = oldRoot;
	}
	
	*root = newRoot;
	
	if(state == 0) {
		updateHeight((*root)->right);
	} else {
		updateHeight((*root)->left);
	}
	
	updateHeight(*root);
}

void updateHeight(avl tree)
{
	int max, lh, rh;
	
	lh = getHeight(tree->left);
	rh = getHeight(tree->right);
	
	max = (lh > rh) ? lh : rh;
	tree->height = 1 + max;
}

void displayInorder(avl tree)
{
	if(tree != NULL) {
		displayInorder(tree->left);
		printf("%d ", tree->key);
		displayInorder(tree->right);
	}

}

int deleteMin(avl *tree)
{
	avl oldRoot;
	int min;
	
	min = -1;
	if(*tree != NULL) {
		if((*tree)->left == NULL) {
			oldRoot = *tree;
			min = oldRoot->key;
			*tree = oldRoot->right;
			free(oldRoot);
		} else {
			min = deleteMin(&(*tree)->left);
		}
		
		balanceTree(tree);
	}
	
	return min;
}

void deleteAVL(avl *tree, int data)
{
	avl oldRoot;
	int state;
	
	if(*tree != NULL) {
		if((*tree)->key != data) {
			if((*tree)->key < data) {
				deleteAVL(&(*tree)->right, data);
			} else {
				deleteAVL(&(*tree)->left, data);
			}	
		} else {
			if((*tree)->right != NULL) {
				(*tree)->key = deleteMin(&(*tree)->right);
			} else {
				oldRoot = *tree;
				*tree = (*tree)->left;
				free(oldRoot);
			}
			printf("\n\tDeleted Successfully!\n");
		}
		
		balanceTree(tree);
	} else {
		printf("\n\tWas not able to delete...\n");
	}
}

int searchAVL(avl tree, int data)
{
	int ret;
	
	while(tree != NULL && tree->key != data) {
		if(data > tree->key) {
			tree = tree->right;
		} else {
			tree = tree->left;
		}
	}
	
	if(tree == NULL) {
		ret = -1;
	} else {
		ret = 1;
	}
	
	return ret;
}

