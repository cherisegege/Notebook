#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "tree.h"

head_t *tree_init(int size)
{
	head_t *me = malloc(sizeof(*me));	
	if(NULL == me){
		return NULL;
	}
	memset(me, '\0', sizeof(*me));	
	me->size = size;
	me->node_h.right = me->node_h.left = me->node_h.data = NULL;

	return me;

}

/*功能：向二叉树插入一个新的结点
 *@root:存储指向根结点地址的指针变量的地址
 *@stu:存储的数据
 * */
int tree_insert_r(node_t **root, void *data, int size, cmp_t cmp)
{
	node_t *new = NULL;	

	if (*root == NULL) {
		new = malloc(sizeof(*new));
		if (new == NULL)
			return -1;
		new->data = malloc(size);
		if(NULL == new->data){
			free(new);
			return 1;
		}
		memcpy(new->data, data, size);
		new->left = new->right = NULL;
		*root = new;
		return 0;
	}
	if (cmp(data, (*root)->data) <= 0)
		return tree_insert_r(&(*root)->left, data, size, cmp);
 	else 
		return tree_insert_r(&(*root)->right, data, size, cmp);
}

/*二叉树的中序遍历结果
 *@root:根结点的地址
 * */
void tree_traval_mid(node_t *root, prt_t prt)
{
	if (root == NULL)	
		return;
	tree_traval_mid(root->left, prt);	
	prt(root->data);
	tree_traval_mid(root->right, prt);	
}

int tree_insert(head_t *head, void *data, cmp_t cmp)
{
	node_t *new = NULL;
	node_t **root = &head->node_h.left;	

	new = malloc(sizeof(*new));
	if (new == NULL)
		return -1;
	new->data = malloc(head->size);
	if(NULL == new->data){
		free(new);
		return 1;
	}
	memcpy(new->data, data, head->size);
	new->left = new->right = NULL;

	while (*root != NULL) {      
		if (cmp((*root)->data, data) >= 0)
			root = &(*root)->left; 
		else 
			root = &(*root)->right; 
	}	
	*root = new;  //因为没有直接赋给left,而是赋给了root, root只是left的一个副本，通过值传递方式赋值，所以它的存在形式和函数参数是一样的。所以需要用二级指针进行回传修改它的地址值。

	return 0;
}

/*
 *打印二叉树
 *		右子树---》根结点---》左子树
 */ 
void __tree_draw(struct node_st *root, int index, prt_t prt)
{
	int i;

	if (root == NULL)
		return ;
	__tree_draw(root->right, index+1, prt);
	for (i = 0; i < index; i++) 
		printf("      ");
	prt(root->data);
	__tree_draw(root->left, index+1, prt);	
}

void tree_draw(struct node_st *root, prt_t prt)
{
	__tree_draw(root, 0, prt);
}

/*
 * 删除结点
 */
static struct node_st *find_max(struct node_st *root)
{
	if (root->right == NULL)
		return root;
	return find_max(root->right);
}
static void __tree_delete(struct node_st **root)
{
	struct node_st *cur = *root;
	struct node_st *l = cur->left;
	struct node_st *r = cur->right;	

	if (l == NULL) {
		*root = r;	
	} else {	
		*root = l;
		find_max(l)->right = r;
	}
	cur->left = cur->right = NULL;
	free(cur);

	return ;
}
/*
 * 从树中删除一个结点
 * @root:存储树根的地址的指针变量的地址
 * @key:要删除结点
 */ 
int tree_delete(struct node_st **root, int key, cmp_tt cmp)
{
	if (*root == NULL)
		return -1;
	if (cmp((*root)->data, key) == 0) {	
		__tree_delete(root);
		return 0;
	}
	if (cmp((*root)->data, key) > 0)
		return tree_delete(&(*root)->left, key, cmp);
	else
		return tree_delete(&(*root)->right, key, cmp);
}

/*
 *树的搜索
 */
struct stu_st *tree_search(struct node_st *root, int key, cmp_tt cmp)
{
	if (root == NULL)	
		return NULL;
	if (cmp(root->data, key) == 0)
		return root->data;
	if (cmp(root->data, key) < 0)
		return tree_search(root->right, key, cmp);
	else 
		return tree_search(root->left, key, cmp);
}

