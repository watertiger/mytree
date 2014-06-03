/**
 * @file balancebstree.c
 * @brief realize balance binary search tree's basic oprations.
 * @author watertiger <darkwkt@gmail.com>
 * @date 2014-06-03
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "balancebstree.h"

#define INIT_SIZE 10

/**
 * @brief bbst_search1 - searching key in balance binary search tree.
 * @param proot pointer to the root of balance binary search tree.
 * @param key the enum to search.
 * @return pbbst pointer to the node of balance binary search tree if found,
 *	   NULL pointer if key not found.
 * 
 * 在平衡二叉排序树b中查找x的过程为：
 *	 1.若b是空树，则搜索失败，否则：
 *	 2.若x等于b的根节点的数据域之值，则查找成功；否则：
 *	 3.若x小于b的根节点的数据域之值，则搜索左子树；否则：
 *	 4.查找右子树。 
 *
 */
pbbst
bbst_search1( pbbst proot, int key )
{
	if ( !proot || ( key == proot->data) )
		return proot;
	if ( key < proot->data )
		return bbst_search1( proot->lc, key );
	else 
		return bbst_search1( proot->rc, key );
}

/**
 * @brief bbst_search2 - searching key in balance binary search tree.
 * @param proot pointer to the root of balance binary search tree.
 * @param key the enum to search.
 * @param p pointer to the pointer to the balance binary search tree node if found,
 *	    otherwise pointer to the last comparison node.
 * @return 1 for succeed,
 *	   0 for failure.
 *
 * the pointer p must be NULL when first called, it's used for returning the
 * pointer needed or last compared.
 *
 * 在平衡二叉排序树b中查找x的过程为：
 *	 1.若b是空树，则搜索失败，否则：
 *	 2.若x等于b的根节点的数据域之值，则查找成功；否则：
 *	 3.若x小于b的根节点的数据域之值，则搜索左子树；否则：
 *	 4.查找右子树。 
 */
int
bbst_search2( pbbst proot, int key, pbbst *p )
{
	if ( !proot ) 
		return 0;

	*p = proot;
	if ( key == proot->data )
		return 1;
	if ( key < proot-> data )
		return bbst_search2( proot->lc, key, p );
	else 
		return bbst_search2( proot->rc, key, p );
}

/**
 * @brief r_rotate - rotate the binary sort tree to the right.
 * @param p pointer to the pointer to the root of balance binary search subtree.
 * @return none.
 * 
 */
void
r_rotate( pbbst *p )
{
	pbbst plc = NULL;

	plc = (*p)->lc;
	(*p)->lc = plc->rc;
	plc->rc = (*p);
	(*p) = plc;

	return;
}

/**
 * @brief l_rotate - rotate the binary sort tree to the left.
 * @param p pointer to the pointer to the root of balance binary search subtree.
 * @return none.
 * 
 */
void
l_rotate( pbbst *p )
{
	pbbst prc = NULL;

	prc = (*p)->rc;
	(*p)->rc = prc->lc;
	prc->lc = (*p);
	(*p) = prc;

	return;
}

/**
 * @brief left_balance - handle left subtree to keep balance.
 * @param p pointer to the pointer to the root of balance binary search subtree.
 * @return none.
 * 
 * 处理LL型和LR型的两种情形，处理后保证子树平衡。
 */
void
left_balance( pbbst *p )
{
	pbbst plc = (*p)->lc;
	pbbst plcrc = NULL;
	
	switch( plc->bf ) {
		case LH:(*p)->bf = EH;
			plc->bf = EH;
			r_rotate( p );
			break;
		case RH:plcrc = plc->rc;
			switch ( plcrc->bf ) {
				case LH:(*p)->bf = RH;plc->bf = EH;break;
				case EH:(*p)->bf = EH;plc->bf = EH;break;
				case RH:(*p)->bf = EH;plc->bf = LH;break;
			}
			plcrc->bf = EH;
			l_rotate( &(*p)->lc );
			r_rotate( p ); 
			break;
		default:printf("Error happenned.\n");
	}

	return;
}

/**
 * @brief right_balance- handle right subtree to keep balance.
 * @param p pointer to the pointer to the root of balance binary search subtree.
 * @return none.
 * 
 * 处理RR型和RL型的两种情形，处理后保证子树平衡。
 */
void
right_balance( pbbst *p )
{
	pbbst prc = (*p)->rc;
	pbbst prclc = NULL;
	
	switch( prc->bf ) {
		case RH:(*p)->bf = EH;
			prc->bf = EH;
			l_rotate( p );
			break;
		case LH:prclc = prc->lc;
			switch ( prclc->bf ) {
				case RH:(*p)->bf = LH;prc->bf = EH;break;
				case EH:(*p)->bf = EH;prc->bf = EH;break;
				case LH:(*p)->bf = EH;prc->bf = RH;break;
			}
			prclc->bf = EH;
			r_rotate( &(*p)->rc );
			l_rotate( p ); 
			break;
		default:printf("Error happenned.\n");
	}

	return;
}

/**
 * @brief bbst_insert - insert an enum into balance binary search tree.
 * @param proot pointer to the pointer to the root of balance binary search tree.
 * @param e the enum to insert.
 * @param tf flag to record whether tree is growing taller.
 * @return 1 for succeed,
 *	   0 for failure.
 * 
 */
int
bbst_insert( pbbst *proot, int e, int *tf )
{
	pbbst p = NULL;
	pbbst pe = NULL;

	if ( !(*proot) ) {
		(*proot) = ( pbbst )malloc( sizeof(bbst_node) );
		if ( !(*proot) ) {
			printf("No Memory.\n");
			return 0;
		}
		(*proot)->data = e;
		(*proot)->bf = EH;
		(*proot)->lc = NULL;
		(*proot)->rc = NULL;
		(*tf) = 1;
	} else {
		if ( e == (*proot)->data ) {
			*tf = 0;
			return 0;/*存在，无需插入*/
		} else if (e < (*proot)->data ) {
			if ( !bbst_insert( &(*proot)->lc, e, tf) )
				return 0;
			if ( *tf ) {
				switch ( (*proot)->bf ) {
					case LH:left_balance( proot );
						*tf = 0;
						break;
					case EH:(*proot)->bf = LH;
						*tf = 1;
						break;
					case RH:(*proot)->bf = EH;
						*tf = 0;
						break;
				}
			}
		} else {
			if ( !bbst_insert( &(*proot)->rc, e, tf) ) 
				return 0;
			if ( *tf ) {
				switch ( (*proot)->bf ) {
					case LH:(*proot)->bf = EH;
						*tf = 0;
						break;
					case EH:(*proot)->bf = RH;
						*tf = 1;
						break;
					case RH:right_balance( proot );
						*tf = 0;
						break;
				}
			}
		}
	}
	
	return 1;
/*
	if ( bst_search2( *proot, e, &p ) ) {
		return 0;
	} else {
		pe = ( pbbst )malloc( sizeof( bbst_node ) );
		if ( !pe ) {
			printf("No Memory!!\n");
			return 0;
		}
		pe->data = e;
		pe->bf = EH;
		pe->lc = NULL;
		pe->rc = NULL;
		
		if ( !p ) {
			(*proot) = pe;
			
		} else if ( e < p->data ) {
			p->lc = pe;
			switch ( p->bf ) {
				case LH:/*do something*;break;
				case EH: p->bf = LH;/*do something*;break;
				case RH: p->bf = EH;break;
			}
		} else {
			p->rc = pe;
			switch ( p->bf ) {
				case LH: p->bf = EH;break;
				case EH: p->bf = LH;/*do something*;break;
				case RH: /*do something*;break;
			}
		}

		return 1;
	}
*/
}

/**
 * @brief bbst_show - show all node's information of balance binary search tree.
 * @param proot pointer to the node of balance binary search tree.
 * @param parent pointer to the parent node of node which pointer proot point to
 * @return none.
 */
void
bbst_show( pbbst proot, pbbst parent )
{
	if ( proot ) {
		printf("Current:%-4d(%+2d). ", proot->data, proot->bf);
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
		bbst_show( proot->lc, proot );
		bbst_show( proot->rc, proot );
	}

	return;
}

int
main()
{
	int array[INIT_SIZE];
	int i = 0;
	int taller_flag = 0;
	pbbst proot = NULL; /* 此处一定要显式置NULL，否则调用会出错，因为是根 */
	pbbst pfind = NULL;

	srand( (unsigned int)time(NULL) );
	for ( i = 0; i < INIT_SIZE; i++ ) {
		array[i] = rand() % 100;
		printf("%-4d", array[i]);
	}
	printf("\n");
	/* 创建平衡二叉搜索树 */
	for ( i = 0; i < INIT_SIZE; i++ ) {
		bbst_insert( &proot, array[i], &taller_flag );
	}
	bbst_show( proot, NULL );
	printf("\n");
	/* 查找节点 */
	pfind = bbst_search1( proot, array[5] );
	if ( pfind ) 
		printf("Node found. it's %-4d(%+2d).\n", pfind->data, pfind->bf);
	else
		printf("Node not exists.\n");
	pfind = bbst_search1( proot, 101 );
	if ( pfind ) 
		printf("Node found. it's %-4d(%+2d).\n", pfind->data, pfind->bf);
	else
		printf("Node not exists.\n");

	return 0;
}
