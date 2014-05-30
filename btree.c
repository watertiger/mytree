/**
 * @file btree.c
 * @brief realize binary tree's basic oprations.
 * @author watertiger <darkwkt@gmail.com>
 * @date 2014-05-27
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "btree.h"

/**
 * @brief create_btree_by_level - creating binary tree by input.
 * @param p pointer to input string.
 * @param i index of tree
 * @return pbtree pointer to head of binary tree.
 *
 * The root's index is 1. But array's first enum index is 0, so
 * makes tree index minus 1 to match array.
 * 
 * this function is creating the tree by level tranversing.
 */
pbtree
create_btree_by_level( char *p , int i)
{
	pbtree proot = NULL;
	
	if( i > strlen(p) )
		return NULL;
	if ( '$' == *(p+i-1) ) {
		return proot;
	} else {
		proot = ( pbtree )malloc( sizeof(btree_node) );
		if( !proot ) {
			printf("No memory.\n");
			return proot;
		}
		proot->data = *(p+i-1);
		proot->lc = create_btree_by_level( p, 2*i);
		proot->rc = create_btree_by_level( p, 2*i+1);
	}

	return proot;
}

/**
 * @brief create_btree_by_preorder - creating binary tree by input.
 * @param p pointer to the pointer to input string.
 * @return pbtree pointer to head of binary tree.
 *
 */
pbtree
create_btree_by_preorder( char **p )
{
	pbtree proot = NULL;
	
	if( !(**p) )
		return NULL;
	if ( '$' == **p ) {
		(*p)++;
		return proot;
	} else {
		proot = ( pbtree )malloc( sizeof(btree_node) );
		if( !proot ) {
			printf("No memory.\n");
			return proot;
		}
		proot->data = **p;
		(*p)++;
		proot->lc = create_btree_by_preorder( p );
		proot->rc = create_btree_by_preorder( p );
	}

	return proot;
}

/**
 * @brief destroy_btree - destroy binary tree, free it's space.
 * @param proot pointer to the pointer to the root of binary tree.
 * @return 0 for success,
 *	   other for failure.
 */
int
destroy_btree( pbtree *proot )
{
	if( *proot ) {
		destroy_btree( &((*proot)->lc) );
		destroy_btree( &((*proot)->rc) );
		free( *proot );
		(*proot) = NULL;
	}
	
	return 0;
}

/**
 * @brief preorder_tranverse - visit tree's node by D-L-R order.
 * @param proot pointer to the root of binary tree.
 * @return none.
 *
 * D - indicate the root node
 * L - indicate the left subtree
 * R - indicate the right subtree
 * First, visit the root of binary tree, then tranverse left 
 * subtree, and last tranverse right subtree
 */
void
preorder_tranverse( pbtree proot )
{
	if ( proot ) {
		printf("%c  ", proot->data);
		preorder_tranverse( proot->lc );
		preorder_tranverse( proot->rc );
	} else {
		printf("$  ");
	}

	return;
}

/**
 * @brief inorder_tranverse - visit tree's node by L-D-R order.
 * @param proot pointer to the root of binary tree.
 * @return none.
 *
 * First tranverse left subtree, then visit root of binary tree, 
 * and last tranverse right subtree
 */
void
inorder_tranverse( pbtree proot )
{
	if ( proot ) {
		preorder_tranverse( proot->lc );
		printf("%c  ", proot->data);
		preorder_tranverse( proot->rc );
	} else {
		printf("$  ");
	}

	return;
}

/**
 * @brief postorder_tranverse - visit tree's node by L-R-D order.
 * @param proot pointer to the root of binary tree.
 * @return none.
 *
 * First tranverse left subtree, then tranverse right subtree,
 * and last visit root of binary tree. 
 */
void
postorder_tranverse( pbtree proot )
{
	if ( proot ) {
		preorder_tranverse( proot->lc );
		preorder_tranverse( proot->rc );
		printf("%c  ", proot->data);
	} else {
		printf("$  ");
	}

	return;
}

/**
 * @brief btree_queue_init - initialize the binary tree queue.
 * @param q pointer to binary tree queue.
 * @return none.
 *
 * used for levelorder tranverse.
 */
void
btree_queue_init( pbtree_queue q )
{
	int i = 0;

	q->front = 0;
	q->rear = 0;
	for ( ; i < QUEUE_LEN; i++ )
		q->queue[i] = 0;

	return;
}

/**
 * @brief btree_queue_empty - check whether the binary tree queue is empty.
 * @param q pointer to binary tree queue.
 * @return none.
 *
 */
int
btree_queue_empty( pbtree_queue q )
{
	if ( q->front == q->rear )
		return 1;

	return 0;
}

/**
 * @brief btree_queue_insert - insert a enum into binary tree queue.
 * @param q pointer to the queue
 * @param p pointer to the binary tree node
 *
 * used for inserting a binary tree node into the tree queue.
 */
void
btree_queue_insert( pbtree_queue q, pbtree p )
{
	q->queue[q->rear++] = p;

	return;
}

/**
 * @brief btree_queue_delete - delete a enum from binary tree queue.
 * @param q pointer to the queue
 *
 * used for deleting a binary tree node from the tree queue.
 */
pbtree
btree_queue_delete( pbtree_queue q )
{
	pbtree p;
	p = q->queue[q->front];
	q->queue[q->front++] = 0;

	return p;
}

/**
 * @brief levelorder_tranverse - visit tree's node by level order.
 * @param proot pointer to the root of binary tree.
 * @return none.
 *
 * First visit the root node, ie, the first level, then visit root's
 * left and right child, that is, the second level, and then root's
 * children's children util the Nth level.
 */
void
levelorder_tranverse( pbtree proot, pbtree_queue q)
{
	pbtree p = NULL;

	if ( proot ) {
		printf("%c  ", proot->data);
		btree_queue_insert( q, proot->lc);
		btree_queue_insert( q, proot->rc);
	} else {
		printf("$  ");
	}
	if ( !proot && btree_queue_empty(q) ) 
		return;
	p = btree_queue_delete( q );
	levelorder_tranverse( p, q);

	return;
}


/**
 * the tree's level relationship, "$" stands for empty subtree.
 *			A
 *		      /   \
 *		     B	   $
 *		   /   \
 *		  C	 D 
 *		/   \  /   \
 *	       $    $ E     F
 *		     / \   / \
 *		    $   G $   $
 *		       / \
 *		      $   $
 */

int
main()
{
	char *s = "ABC$$DE$G$$F$$$";
	pbtree proot = NULL;
	btree_queue bt_queue;

	proot = create_btree_by_preorder( &s );
	if ( !proot ) {
		printf("Error happened.\n");
		return;
	}
	preorder_tranverse( proot );
	printf("\n");
	inorder_tranverse( proot );
	printf("\n");
	postorder_tranverse( proot );
	printf("\n");
	
	btree_queue_init( &bt_queue );
	levelorder_tranverse( proot, &bt_queue);
	printf("\n");

	return 0;
}
