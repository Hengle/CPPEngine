

//-----------------------------------------------------------------------------
// File: RenderQueue.cpp
//
//  Terrain Demo.
//
// Copyright (c) 2000 ATI Technologies Inc. All rights reserved.
//-----------------------------------------------------------------------------

#include <memory.h>
#include "Helper.h"
#include "RenderQueue.h"
#include "DXErrors.h"

CRenderQueue::CRenderQueue()
{
	for(unsigned long i = 0; i < MAX_MATERIALS; i++)
	{
		for(unsigned long j = 0; j < MAX_VB_QUEUES; j++)
		{
			m_Queues[i][j].dwVBID = -1;
			m_Queues[i][j].dwListCount = 0;
			m_Queues[i][j].pLists = new CTriangleList* [MAX_QUEUE_SIZE];
		}
	}
}

CRenderQueue::~CRenderQueue()
{
	for(unsigned long i = 0; i < MAX_MATERIALS; i++)
	{
		for(unsigned long j = 0; j < MAX_VB_QUEUES; j++)
		{
			SAFE_DELETE_ARRAY(m_Queues[i][j].pLists);
		}
	}
}

void CRenderQueue::ClearQueue()
{
	for(unsigned long i = 0; i < MAX_MATERIALS; i++)
	{
		for(unsigned long j = 0; j < MAX_VB_QUEUES; j++)
		{
			m_Queues[i][j].dwVBID = -1;
			m_Queues[i][j].dwListCount = 0;
		}
		m_dwVBQueueCount[i] = 0;
	}
}

int CRenderQueue::AddToQueue(CTriangleList *tlist)
{
	unsigned long mID = tlist->m_dwMaterialID;
	unsigned long vbID = tlist->m_dwVBID;
	unsigned long i;

	bool queueFound = false;
	for(i = 0; i < m_dwVBQueueCount[mID]; i++)
	{
		////LOG_PRINT("m_Queues[mID][i].dwVBID = %x\n", m_Queues[mID][i].dwVBID);
		////LOG_PRINT("vbID = %x\n", vbID);
		if (m_Queues[mID][i].dwVBID == vbID)
		{
			queueFound = true;
			break;
		}
	}

	if (!queueFound)
	{
		i = m_dwVBQueueCount[mID]++;
		m_Queues[mID][i].dwVBID = vbID;
	}

	// Batch by VB and material
	m_Queues[mID][i].pLists[m_Queues[mID][i].dwListCount++] = tlist;

	return 1;
}

int CRenderQueue::Render(CVertexStore *pVS)
{

	//LOG_FNLN;
	CTriangleList **lists;
	int hr=-1;

	for(unsigned long i = 0; i < MAX_QUEUES; i++)
	{		
		
		if (m_dwVBQueueCount[i] > 0)
		{
			//LOG_FNLN;
			//LOG_PRINT("i=%d\n", i);
			CMaterial* p=g_MMan.GetMaterial(i);
			//LOG_PRINT("p=%x\n", p);
			if(p)
			{	
				//LOG_FNLN;
				hr=g_MMan.GetMaterial(i)->SetMaterial();
				//LOG_PRINT("hr=%d\n", hr);
			}
		}

		
		for(unsigned long j = 0; j < m_dwVBQueueCount[i]; j++)
		{
			
			lists = m_Queues[i][j].pLists;
			
			for(unsigned long k = 0; k < m_Queues[i][j].dwListCount; k++)
			{
				
				lists[k]->Render(pVS, hr);
			}

		}
	}

	
	return 1;
}

unsigned long CRenderQueue::CountPolys()
{
	unsigned long dwCount = 0;
	CTriangleList **lists;

	for(unsigned long i = 0; i < MAX_QUEUES; i++)
	{
		for(unsigned long j = 0; j < m_dwVBQueueCount[i]; j++)
		{
			lists = m_Queues[i][j].pLists;
			for(unsigned long k = 0; k < m_Queues[i][j].dwListCount; k++)
				dwCount += lists[k]->m_dwIndNum;
		}
	}

	return dwCount / 3;
}

