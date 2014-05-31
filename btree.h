/**
 * @file btree.h
 * @brief describe tree's defination and basic oprations.
 * @author watertiger <darkwkt@gmail.com>
 * @date 2014-05-27
 */
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief define the binary tree node and it's basic oprations
 *
 *  
 */
typedef struct _binary_tree {
	char data;
	struct _binary_tree *lc, *rc;/* left and right child pointer */
}btree_node, *pbtree;

pbtree create_btree_by_level( char *p, int i);
pbtree create_btree_by_preorder( char **p );
int destroy_btree( pbtree *proot );
void preorder_tranverse( pbtree proot);
void inorder_tranverse( pbtree proot);
void postorder_tranverse( pbtree proot);

/**
 * @brief define the queue used for binary tree.
 *
 * describe it's definition and basic oprations.
 */
#define QUEUE_LEN 100

typedef struct _btree_queue {
	pbtree queue[QUEUE_LEN];
	int front;
	int rear;
}btree_queue, *pbtree_queue;

void btree_queue_init( pbtree_queue q );
int btree_queue_empty( pbtree_queue q );
void btree_queue_insert( pbtree_queue q, pbtree p );
pbtree btree_queue_delete( pbtree_queue q);

void levelorder_tranverse_recursive( pbtree proot, pbtree_queue q);
void levelorder_tranverse_nonrecursive( pbtree proot, pbtree_queue q);
