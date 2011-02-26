// TestBTree02.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <cassert>
#include <cstring>
struct CBTreeNode
{
	enum {NODE_SIZE = 100};
	enum {END_POS = NODE_SIZE + 1};
	
	struct CNodePosition
	{
		CBTreeNode* m_pCurNode;
		int			m_nCurIndex;
	};
	struct CNode
	{
		int			m_nkey;
		int			m_nValue;
		CBTreeNode*	m_pSubNode;	
	};

	size_t m_nkeyCount;
	CBTreeNode* m_pParentNode;
	CNode m_aNodeTbl[NODE_SIZE];

	CBTreeNode()
	{
		m_nkeyCount = 0;
	}
	size_t GetKeyCount()
	{
		return m_nkeyCount;
	}
	int Insert(int nKey,int nValue,CBTreeNode* pNode)
	{
		int i = 1;
		while (1)
		{
			if (nKey < m_aNodeTbl[i].m_nkey)
			{
				break;
			}
		}
		for (int r = GetKeyCount();r > i;r--)
		{
			m_aNodeTbl[r + 1] = m_aNodeTbl[r];
		}
		m_aNodeTbl[i].m_nkey = nKey;
		m_aNodeTbl[i].m_nValue = nValue;
		m_aNodeTbl[i].m_pSubNode = pNode;
		m_nkeyCount++;
		return i;
	}
	int Search(int nKey,CNodePosition& rNodePos)
	{
		for (int i = 1;i < m_nkeyCount;i++)
		{
			if (m_aNodeTbl[i].m_nkey == nKey)
			{
				rNodePos.m_nCurIndex = i;
				rNodePos.m_pCurNode = m_aNodeTbl->m_pSubNode;
				return i;
			}
		}
	}
	
};
struct CNodePosition
{
	CBTreeNode* m_pCurNode;
	int			m_nCurIndex;
};
int SearchKey(CBTreeNode* pNode,int nKey)
{
	assert(pNode);
	//小于第一个KEY
	for (int i = 1;i <= pNode->GetKeyCount();i++)
	{
		if (pNode->m_aNodeTbl[i].m_nkey <= nKey)
		{
			return i;
		}
	}
	return 0;
	
}
bool Search(CBTreeNode* pNode,int nKey,int& nValue,CNodePosition& rNodePos)
{

	int nSearchIndex = 0;
	bool bFound = 0;
	CBTreeNode* pCurrentNode = pNode;
	
	

	while ((NULL != pCurrentNode) && !bFound)
	{
		nSearchIndex = SearchKey(pCurrentNode,nKey);
		if (nSearchIndex > 0 && pCurrentNode->m_aNodeTbl[nSearchIndex].m_nkey == nKey)
		{
			rNodePos.m_pCurNode = pCurrentNode;
			rNodePos.m_nCurIndex = nSearchIndex;
			nValue = pCurrentNode->m_aNodeTbl[nSearchIndex].m_nValue;
			bFound = true;
		}
		else
		{
			rNodePos.m_pCurNode = pCurrentNode;
			rNodePos.m_nCurIndex = nSearchIndex;
			pCurrentNode = pCurrentNode->m_aNodeTbl[nSearchIndex].m_pSubNode;
			bFound = false;
		}
	}
	return bFound;
}
int InsertElement(CBTreeNode* pNode,int nKey,int nValue,CBTreeNode* pSubNode)
{
	return pNode->Insert(nKey,nValue,pSubNode);
}
void CreateRootNode(CBTreeNode*& pNode,CBTreeNode* pOriginLeft,CBTreeNode* pOriginRight)
{
	pNode = new CBTreeNode;
	memset(pNode,0,sizeof (CBTreeNode));
	pNode->m_aNodeTbl[0].m_pSubNode = pOriginLeft;
	pNode->m_aNodeTbl[0].m_nkey = 1;
	pNode->m_aNodeTbl[1].m_pSubNode = pOriginRight;
	pNode->m_aNodeTbl[1].m_nkey = ??
	pNode->m_nkeyCount = 1;
	pNode->m_pParentNode = NULL;
	
}
int Insert(CBTreeNode*& pNode,int nKey,int nValue)
{
	CNodePosition oNodePos;
	int nVal = 0;
	if (Search(pNode,nKey,nVal,oNodePos))
		return -1;
	CBTreeNode* pParentNode = oNodePos.m_pCurNode;
	while (pParentNode)
	{
		InsertElement(oNodePos.m_pCurNode,nKey,nValue,pParentNode);
	}
	if (NULL == pParentNode)
	{

	}
	
}
int _tmain(int argc, _TCHAR* argv[])
{
	CBTreeNode* pNode = NULL;
	Insert(pNode,1,1);
	return 0;
}

