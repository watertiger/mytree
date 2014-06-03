/**
 * @file balancebstree.h
 * @brief describe balance binary search tree's defination and basic oprations.
 * @author watertiger <darkwkt@gmail.com>
 * @date 2014-06-03
 */
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief define the balanced binary search tree node and it's basic oprations
 *
 * 平衡二叉树（Balanced Binary Tree 或 Height-Balanced Tree）又称为AVL树。
 * 	它或是一棵空树，或者具有下列性质的二叉树：
 *	1、它的左右子树都是平衡二叉树，
 *	2、且左子树和右子树的深度差的绝对值不超过1。
 * 定义平衡因子BF(Balance Factor)为左子树深度减去右子树深度，那么BF的值
 * 只可能为 -1, 0, 1.
 *
 * 针对二叉排序而言，平衡二叉排序树还需要满足二叉排序树的定义。
 *  
 */
#define LH +1
#define EH  0
#define RH -1

typedef struct _balance_binary_search_tree {
	int data;
	int bf;
	struct _balance_binary_search_tree *lc, *rc;/* left and right child pointer */
}bbst_node, *pbbst;

pbbst bbst_search1( pbbst proot, int key );
int bbst_search2( pbbst proot, int key, pbbst *p);
int bbst_insert( pbbst *proot, int e, int *tf );
int bbst_delete( pbbst *proot, int key);

