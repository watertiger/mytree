/**
 * @file bsearchtree.c
 * @brief realize binary search tree's basic oprations.
 * @author watertiger <darkwkt@gmail.com>
 * @date 2014-05-31
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bsearchtree.h"

#define INIT_SIZE 10

/**
 * @brief bst_search1 - searching key in binary search tree.
 * @param proot pointer to the root of binary search tree.
 * @param key the enum to search.
 * @return pbst pointer to the node of binary search tree if found,
 *	   NULL pointer if key not found.
 * 
 * 在二叉排序树b中查找x的过程为：
 *	 1.若b是空树，则搜索失败，否则：
 *	 2.若x等于b的根节点的数据域之值，则查找成功；否则：
 *	 3.若x小于b的根节点的数据域之值，则搜索左子树；否则：
 *	 4.查找右子树。 
 *
 */
pbst
bst_search1( pbst proot, int key )
{
	if ( !proot || ( key == proot->data) )
		return proot;
	if ( key < proot->data )
		return bst_search1( proot->lc, key );
	else 
		return bst_search1( proot->rc, key );
}

/**
 * @brief bst_search2 - searching key in binary search tree.
 * @param proot pointer to the root of binary search tree.
 * @param key the enum to search.
 * @param p pointer to the pointer to the binary search tree node if found,
 *	    otherwise pointer to the last comparison node.
 * @return 1 for succeed,
 *	   0 for failure.
 *
 * the pointer p must be NULL when first called, it's used for returning the
 * pointer needed or last compared.
 *
 * 在二叉排序树b中查找x的过程为：
 *	 1.若b是空树，则搜索失败，否则：
 *	 2.若x等于b的根节点的数据域之值，则查找成功；否则：
 *	 3.若x小于b的根节点的数据域之值，则搜索左子树；否则：
 *	 4.查找右子树。 
 */
int
bst_search2( pbst proot, int key, pbst *p )
{
	if ( !proot ) 
		return 0;

	*p = proot;
	if ( key == proot->data )
		return 1;
	if ( key < proot-> data )
		return bst_search2( proot->lc, key, p );
	else 
		return bst_search2( proot->rc, key, p );
}

/**
 * @brief bst_insert - insert an enum into binary search tree.
 * @param proot pointer to the pointer to the root of binary search tree.
 * @param e the enum to insert.
 * @return 1 for succeed,
 *	   0 for failure.
 * 
 * 首先执行查找算法，找出被插结点的父亲结点。
 * 判断被插结点是其父亲结点的左、右儿子。将被插结点作为叶子结点插入。
 * 若二叉树为空。则首先单独生成根结点。
 * 注意：新插入的结点总是叶子结点。
 */
int
bst_insert( pbst *proot, int e )
{
	pbst p = NULL;
	pbst pe = NULL;

	if ( bst_search2( *proot, e, &p ) ) {
		return 0;
	} else {
		pe = ( pbst )malloc( sizeof( bst_node ) );
		if ( !pe ) {
			printf("No Memory!!\n");
			return 0;
		}
		pe->data = e;
		pe->lc = NULL;
		pe->rc = NULL;
		
		if ( !p )
			(*proot) = pe;
		else if ( e < p->data )
			p->lc = pe;
		else
			p->rc = pe;

		return 1;
	}
}

/**
 * @brief delete_node - delete an enum from binary search tree.
 * @param p pointer to the pointer to the node to be deleted.
 * @return 1 for succeed,
 *	   0 for failure.
 *
 * 假设被删结点是*p，其双亲是*f，不失一般性，设*p是*f的左孩子，
 * 下面分三种情况讨论：
 *	1.若*p结点为叶子结点，即PL(左子树)和PR(右子树)均为空树。由于删去叶子结点
 *	  不破坏整棵树的结构，则只需修改其双亲结点的指针即可。
 *	2.若*p结点只有左子树PL或右子树PR，此时只要令PL或PR直接成为其双亲结点*f的
 *	  左子树（当*p是左子树）或右子树（当*p是右子树）即可，作此修改也不破坏二
 *	  叉排序树的特性。
 *	3.若*p结点的左子树和右子树均不空。在删去*p之后，为保持其它元素之间的相对
 *	  位置不变，可按中序遍历保持有序进行调整。比较好的做法是，找到*p的直接前
 *	  驱（或直接后继）*s，用*s来替换结点*p，然后再删除结点*s。 
 */
int delete_node( pbst *p )
{
	pbst q = NULL;
	pbst s = NULL;

	if ( !(*p)->rc ) {
		printf("rc null.\n");
		q = *p;
		(*p) = (*p)->lc;
		if ( q )
			free( q );
	} else if ( !(*p)->lc ) {
		printf("lc null.\n");
		q = *p;
		(*p) = (*p)->rc;
		if ( q )
			free( q );
	} else {
		printf("lc and rc exist.\n");
		q = *p;
		s = (*p)->lc;
		for ( ; s->rc ; ) {
			q = s;
			s = s->rc;
		}
		(*p)->data = s->data;
		if ( q != (*p) )
			q->rc = s->lc;
		else
			q->lc = s->lc;
		free( s );
		s = NULL;
	}

	return 1;
}
/**
 * @brief bst_delete - delete an enum from binary search tree.
 * @param proot pointer to the root of binary search tree.
 * @param key the enum to delete.
 * @return 1 for succeed,
 *	   0 for failure.
 */
int
bst_delete( pbst *proot, int key )
{
	if ( !(*proot) )
		return 0;
	if ( key == (*proot)->data )
		return delete_node( proot );
	else if ( key < (*proot)->data )
		return bst_delete( &(*proot)->lc, key );
	else
		return bst_delete( &(*proot)->rc, key );
}

/**
 * @brief bst_show - show all node's information of binary search tree.
 * @param proot pointer to the node of binary search tree.
 * @param parent pointer to the parent node of node which pointer proot point to
 * @return none.
 */
void
bst_show( pbst proot, pbst parent )
{
	if ( proot ) {
		printf("Current:%-4d", proot->data);
		if ( proot->lc )
			printf("LC:%-4d", proot->lc->data);
		else
			printf("LC:$   ");
		if ( proot->rc )
			printf("RC:%-4d", proot->rc->data);
		else
			printf("RC:$   ");
		if ( parent )
			printf("P:%-4d\n", parent->data);
		else
			printf("P:$   \n");
		bst_show( proot->lc, proot );
		bst_show( proot->rc, proot );
	}

	return;
}

int
main()
{
	int array[INIT_SIZE];
	//int array1[INIT_SIZE] = {37,70,43,52,28,22,34,42,47,31};
	int array1[12] = {34,22,28,31,52,37,70,43,42,47,45,46};
	int i = 0;
	pbst proot = NULL; /* 此处一定要显式置NULL，否则调用会出错，因为是根 */
	pbst proot1 = NULL;
	pbst pfind = NULL;

	srand( (unsigned int)time(NULL) );
	for ( i = 0; i < INIT_SIZE; i++ ) {
		array[i] = rand() % 100;
		printf("%-4d", array[i]);
	}
	printf("\n");
	/* 创建二叉搜索树 */
	for ( i = 0; i < INIT_SIZE; i++ ) {
		bst_insert( &proot, array[i] );
	}
	bst_show( proot, NULL );
	printf("\n");
	/* 查找节点 */
	pfind = bst_search1( proot, array[5] );
	if ( pfind ) 
		printf("Node found. it's %-4d.\n", pfind->data);
	else
		printf("Node not exists.\n");
	pfind = bst_search1( proot, 101 );
	if ( pfind ) 
		printf("Node found. it's %-4d.\n", pfind->data);
	else
		printf("Node not exists.\n");
	/* 插入新节点 */
	bst_insert( &proot, 55 );
	bst_show( proot, NULL );
	printf("\n");

	/* 创建二叉搜索树 */
	for ( i = 0; i < 12; i++ ) {
		bst_insert( &proot1, array1[i] );
	}
	bst_show( proot1, NULL );
	printf("\n");
	/* 删除新节点 */
	bst_delete( &proot1, 52 );
	bst_show( proot1, NULL );
	printf("\n");

	return 0;
}
