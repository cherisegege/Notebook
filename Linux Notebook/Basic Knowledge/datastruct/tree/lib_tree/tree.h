#ifndef TREE_H
#define TREE_H

//node
typedef struct node_st {
	void *data;
	struct node_st *left;
	struct node_st *right;
}node_t;

typedef struct head_st {
	node_t node_h;
	int size;
}head_t;

typedef int (*cmp_t)(const void *data1, const void *data2);
typedef int (*cmp_tt)(const void *data1, int key);
typedef void (*prt_t)(const void *data);

head_t *tree_init(int size);

int tree_insert_r(node_t **root, void *data, int size, cmp_t cmp);

int tree_insert(head_t *head, void *data, cmp_t cmp);

void __tree_draw(struct node_st *root, int index, prt_t prt);

void tree_draw(struct node_st *root, prt_t prt);

void tree_traval_mid(node_t *root, prt_t prt);

int tree_delete(struct node_st **root, int key, cmp_tt cmp);

struct stu_st *tree_search(struct node_st *root, int key, cmp_tt cmp);

#endif

