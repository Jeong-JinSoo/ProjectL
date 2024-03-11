#include <math.h>
#include <stdio.h>
#include <string.h>

#include "Navigation.h"
#include "DetourNavMeshBuilder.h"
#include "DetourNavMeshQuery.h"
#include "RecastDump.h"

// �����ϱ�� �밭�� ����?
// rcCongig = �׳� �޽� ��� ���� �Ұ����� ���� ����

bool Navigation::handleBuild()
{
	const float* bmin = 0;
	const float* bmax = 0;
	const float* verts = 0;
	const int nverts = 0;
	const int* tris = 0;
	const int ntris = 0;

	//
	// 1�ܰ�. ���� ������ �ʱ�ȭ�մϴ�.
	//

	// GUI���� ���� ���� �ʱ�ȭ

	/// m_config �� Ÿ���� rcConfig �ε� rc�� �� api���� recast�� ���ڷ� �ٰ� ���̹Ƿ�
	/// ���� recastConfiguration ���� �ǹ��ΰ� ����.
	/// rcConfig�� ������ Recast.h�� ���� �� ���� ������ �� ���� 
	memset(&_cfg, 0, sizeof(_cfg));	// m_cfg��ŭ�� �޸𸮸� 0���� �ʱ�ȭ
	_cfg.cs = _cellSize;
	_cfg.ch = _cellHeight;
	_cfg.walkableSlopeAngle		= _agentMaxSlope;
	_cfg.walkableHeight			= (int)ceilf(_agentHeight / _cfg.ch);
	_cfg.walkableClimb			= (int)floorf(_agentMaxClimb / _cfg.ch);
	_cfg.walkableRadius			= (int)ceilf(_agentRadius / _cfg.cs);
	_cfg.maxEdgeLen				= (int)(_edgeMaxLen / _cellSize);
	_cfg.maxSimplificationError = _edgeMaxError;
	_cfg.minRegionArea			= (int)rcSqr(_regionMinSize);		// Note: area = size*size
	_cfg.mergeRegionArea		= (int)rcSqr(_regionMergeSize);	// Note: area = size*size
	_cfg.maxVertsPerPoly		= (int)_vertsPerPoly;
	_cfg.detailSampleDist		= _detailSampleDist < 0.9f ? 0 : _cellSize * _detailSampleDist;
	_cfg.detailSampleMaxError	= _cellHeight * _detailSampleMaxError;

	// ������̼��� ����� ������ �����մϴ�.
	// ���⼭�� �Է� �޽��� �ٿ�尡 ��������
	// ������ ����� ���� ���� ������ ������ �� �ֽ��ϴ�.
	rcVcopy(_cfg.bmin, bmin);					// ���� recast.h�� �����ϴ� �Լ�.
	rcVcopy(_cfg.bmax, bmax);
	rcCalcGridSize(_cfg.bmin, _cfg.bmax, _cfg.cs, &_cfg.width, &_cfg.height);

	// ���� �ð� ������ �缳���մϴ�.
	_ctx->resetTimers();

	// ���� ���μ����� �����մϴ�.	
	_ctx->startTimer(RC_TIMER_TOTAL);

	_ctx->log(RC_LOG_PROGRESS, "Building navigation:");
	_ctx->log(RC_LOG_PROGRESS, " - %d x %d cells", _cfg.width, _cfg.height);
	_ctx->log(RC_LOG_PROGRESS, " - %.1fK verts, %.1fK tris", nverts / 1000.0f, ntris / 1000.0f);

	//
	// 2�ܰ�. �Է� ������ ������ ������ȭ�մϴ�.
	//

	// �Է� �����͸� ������ȭ�� ���� ����Ʈ�ʵ带 �Ҵ��մϴ�.
	_solid = rcAllocHeightfield();
	if (!_solid)
	{
		_ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'solid'.");
		return false;
	}
	if (!rcCreateHeightfield(_ctx, *_solid, _cfg.width, _cfg.height, _cfg.bmin, _cfg.bmax, _cfg.cs, _cfg.ch))
	{
		_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not create solid heightfield.");
		return false;
	}

	// �ﰢ�� ���� ������ ���� �� �ִ� �迭�� �Ҵ��մϴ�.
	// ó���ؾ� �ϴ� �޽ð� ���� �� �ִ� ���, ó���ؾ� �ϴ� �ִ� Ʈ���̾ޱ� ���� ������
	// ó���ؾ� �ϴ� Ʈ���̾ޱ��� �ִ� ������ ���� �� �ִ� �迭�� �Ҵ��մϴ�.
	_triareas = new unsigned char[ntris];
	if (!_triareas)
	{
		_ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'm_triareas' (%d).", ntris);
		return false;
	}

	// ���⿡ ���� ���� �� �ִ� �ﰢ���� ã�� ������ȭ�մϴ�.
	// �Է� �����Ͱ� ���� ���� �޽��� ��� ���⿡�� ��ȯ�Ͽ� �� �޽���
	// �� �޽��� are ������ ����Ͽ� ������ȭ�մϴ�.
	memset(_triareas, 0, ntris * sizeof(unsigned char));
	rcMarkWalkableTriangles(_ctx, _cfg.walkableSlopeAngle, verts, nverts, tris, ntris, _triareas);
	if (!rcRasterizeTriangles(_ctx, verts, nverts, tris, _triareas, ntris, *_solid, _cfg.walkableClimb))
	{
		_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not rasterize triangles.");
		return false;
	}

	if (!_keepInterResults)
	{
		delete[] _triareas;
		_triareas = 0;
	}

	//
	// 3�ܰ�. �ȱ� ������ ǥ���� ���͸��մϴ�.
	//

	// ��� ������Ʈ���� ������ȭ�Ǹ� ���͸��� �ʱ� �н��� �����մϴ�.
	// �������� ������ȭ�� ���� �߻��ϴ� ���ʿ��� ����θ� �����ϰ�
	// ĳ���Ͱ� �� ���� �� ���� ������ ���͸��մϴ�.
	if (_filterLowHangingObstacles)
		rcFilterLowHangingWalkableObstacles(_ctx, _cfg.walkableClimb, *_solid);
	if (_filterLedgeSpans)
		rcFilterLedgeSpans(_ctx, _cfg.walkableHeight, _cfg.walkableClimb, *_solid);
	if (_filterWalkableLowHeightSpans)
		rcFilterWalkableLowHeightSpans(_ctx, _cfg.walkableHeight, *_solid);


	//
	// 4�ܰ�. ���� ������ ǥ���� �ܼ��� �������� �����մϴ�.
	//

	// �������� �� ������ ó���� �� �ֵ��� ���� �ʵ带 �����մϴ�.
	// �̷��� �ϸ� �� ���� ĳ�� �ϰ��� �����ͻӸ� �ƴ϶� �ȱ� ������ �� ������ �̿���
	// ������ �ϰ��� �����͸� �� ���� ����� �� �ֽ��ϴ�.
	_chf = rcAllocCompactHeightfield();
	if (!_chf)
	{
		_ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'chf'.");
		return false;
	}
	if (!rcBuildCompactHeightfield(_ctx, _cfg.walkableHeight, _cfg.walkableClimb, *_solid, *_chf))
	{
		_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not build compact data.");
		return false;
	}

	if (!_keepInterResults)
	{
		rcFreeHeightField(_solid);
		_solid = 0;
	}

	// ������Ʈ �ݰ濡 ���� �ȱ� ������ ������ ħ���մϴ�.
	if (!rcErodeWalkableArea(_ctx, _cfg.walkableRadius, *_chf))
	{
		_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not erode.");
		return false;
	}

//	InputGeom.h�� �ʿ��� �κ�... �� API�� ���ԵǴ� �η��� �ƴ϶� ����
// 	const ConvexVolume* vols = m_geom->getConvexVolumes();
// 	for (int i = 0; i < m_geom->getConvexVolumeCount(); ++i)
// 		rcMarkConvexPolyArea(m_ctx, vols[i].verts, vols[i].nverts, vols[i].hmin, vols[i].hmax, (unsigned char)vols[i].area, *m_chf);


	// ���߿� ������ �˰����� ����Ͽ� ���� ������ ������ �ﰢ ������ �� �ֵ��� ���� �ʵ带 �����մϴ�.
	// ���� ������� 3������ ������, ���� ������� �ֽ��ϴ�:
	// 1) ���� ��Ƽ�Ŵ�
	// - �������� ��ĳ��Ʈ ��Ƽ�Ŵ�
	// - ���� ���� �׼����̼��� �����մϴ�.
	// - ���� ���� ����
	// - ����Ʈ�ʵ带 �����̳� ��ħ�� ���� ���� �������� �����մϴ�.
	// - �� �޼��尡 ���۰� ��ħ�� �����ϴ� �ڳ� ���̽��� �� ���� �ֽ��ϴ�.
	// ���� ��ֹ��� ū ���� ������ ����� �� ������ ���� �� �ֽ��ϴ�(�ﰢ �������� ó���� �� ����).
	// - ���� ������ ����(��: ���)�� �ִ� ��� ��ħ�� �߻��� �� ������, �̷� ���� �ﰢ ������ ������ �� �ֽ��ϴ�.
	// * �Ϲ������� ����޽��� �̸� ����ϴ� ���� ���� ������, ���� ���� ������ �ִ� ��� �� ����� ����մϴ�.
	// 2) ����� ��Ƽ�Ŵ�
	// - ���� ����
	// - ����Ʈ�ʵ带 ���۰� ��ħ�� ���� �������� �����մϴ�(����).
	// - ��� ���� �ٰ����� �����Ͽ� ������ ��ȸ ��θ� �����մϴ�.
	// * ���� ����޽� ������ ���Ѵٸ� �� �ɼ��� ����ϼ���.
	// 3) ���̾� ����
	// - �ſ� ����
	// - ���� �ʵ带 ��ġ�� �ʴ� �������� �����մϴ�.
	// - ���ۿ� ��ó�ϱ� ���� �ﰢ ���� �ڵ忡 �����մϴ� (���� ����� ��Ƽ�Ŵ׺��� �����ϴ�).
	// - ����� ��Ƽ�Ŵ׺��� �� ���� Ʈ���̾ޱ��� �����մϴ�.
	// - �м��� ������ �ڳ� ���̽��� �����ϴ�.
	// - ������ �ణ ���� ���� �׼����̼��� ������ �� �ֽ��ϴ�(����溸�ٴ� �����ϴ�).
	// ���� ��ֹ��� �ִ� ���� ���� ������ �ִ� ���(Ÿ���� ����ϴ� ��� ���� ����)
	// * �߰� �� ���� ũ���� Ÿ���� �ִ� Ÿ���� ����޽ÿ� ����ϱ� ���� �����Դϴ�.

	if (_partitionType == SAMPLE_PARTITION_WATERSHED)
	{
		// �ȱ� ������ ǥ���� ���� �Ÿ� �ʵ带 ����Ͽ� ���� ������ �غ��մϴ�.
		if (!rcBuildDistanceField(_ctx, *_chf))
		{
			_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not build distance field.");
			return false;
		}

		// �ȱ� ���� ǥ���� ������ ���� �ܼ��� �������� �����մϴ�.
		if (!rcBuildRegions(_ctx, *_chf, 0, _cfg.minRegionArea, _cfg.mergeRegionArea))
		{
			_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not build watershed regions.");
			return false;
		}
	}
	else if (_partitionType == SAMPLE_PARTITION_MONOTONE)
	{
		// �ȱ� ���� ǥ���� ������ ���� �ܼ��� �������� �����մϴ�.
		// ����� ��Ƽ�Ŵ׿��� ���Ͻ��ʵ尡 �ʿ����� �ʽ��ϴ�.
		if (!rcBuildRegionsMonotone(_ctx, *_chf, 0, _cfg.minRegionArea, _cfg.mergeRegionArea))
		{
			_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not build monotone regions.");
			return false;
		}
	}
	else // ����_��Ƽ��_���̾�
	{
		// �ȱ� ���� ǥ���� ������ ���� �ܼ��� �������� �����մϴ�.
		if (!rcBuildLayerRegions(_ctx, *_chf, 0, _cfg.minRegionArea))
		{
			_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not build layer regions.");
			return false;
		}
	}

	//
	// 5�ܰ�. ���� ������ �����ϰ� �ܼ�ȭ�մϴ�.
	//

	// �������� ����ϴ�.
	_cset = rcAllocContourSet();
	if (!_cset)
	{
		_ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'cset'.");
		return false;
	}
	if (!rcBuildContours(_ctx, *_chf, _cfg.maxSimplificationError, _cfg.maxEdgeLen, *_cset))
	{
		_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not create contours.");
		return false;
	}

	//
	// 6�ܰ�. ���������� �ٰ��� �޽��� ����ϴ�.
	//

	// ���������� �ٰ��� ���� �޽��� ����ϴ�.
	_pmesh = rcAllocPolyMesh();
	if (!_pmesh)
	{
		_ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'pmesh'.");
		return false;
	}
	if (!rcBuildPolyMesh(_ctx, *_cset, _cfg.maxVertsPerPoly, *_pmesh))
	{
		_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not triangulate contours.");
		return false;
	}

	//
	// 7�ܰ�. �� �ٰ����� �뷫���� ���̿� ������ �� �ִ� ������ �޽��� ����ϴ�.
	//

	_dmesh = rcAllocPolyMeshDetail();
	if (!_dmesh)
	{
		_ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'pmdtl'.");
		return false;
	}

	if (!rcBuildPolyMeshDetail(_ctx, *_pmesh, *_chf, _cfg.detailSampleDist, _cfg.detailSampleMaxError, *_dmesh))
	{
		_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not build detail mesh.");
		return false;
	}

	if (!_keepInterResults)
	{
		rcFreeCompactHeightfield(_chf);
		_chf = 0;
		rcFreeContourSet(_cset);
		_cset = 0;
	}

	// ���� ������̼� �޽� �����Ͱ� �غ�Ǿ����Ƿ� m_pmesh���� �׼����� �� �ֽ��ϴ�.
	// �����Ϳ� �׼����ϴ� ����� duDebugDrawPolyMesh �Ǵ� dtCreateNavMeshData ������ �����ϼ���.

	//
	// (���� ����) 8�ܰ�. ��ĳ��Ʈ ���� �޽ÿ��� ������ �����͸� �����մϴ�.
	//

	// GUI�� ������� �ִ� ����Ʈ�� ����� ó���� �� �ִ� �ͺ��� �� ���� ����� �� �ֽ��ϴ�.
	// ������ �ʰ����� �ʴ� ��쿡�� ��ȸ ����޽ø� �����մϴ�.
	if (_cfg.maxVertsPerPoly <= DT_VERTS_PER_POLYGON)
	{
		unsigned char* navData = 0;
		int navDataSize = 0;

		// �������� ���� �÷��׸� ������Ʈ�մϴ�.
		for (int i = 0; i < _pmesh->npolys; ++i)
		{
			if (_pmesh->areas[i] == RC_WALKABLE_AREA)
				_pmesh->areas[i] = SAMPLE_POLYAREA_GROUND;

			if (_pmesh->areas[i] == SAMPLE_POLYAREA_GROUND ||
				_pmesh->areas[i] == SAMPLE_POLYAREA_GRASS	||
				_pmesh->areas[i] == SAMPLE_POLYAREA_ROAD)
			{
				_pmesh->flags[i] = SAMPLE_POLYFLAGS_WALK;
			}
			else if (_pmesh->areas[i] == SAMPLE_POLYAREA_WATER)
			{
				_pmesh->flags[i] = SAMPLE_POLYFLAGS_SWIM;
			}
			else if (_pmesh->areas[i] == SAMPLE_POLYAREA_DOOR)
			{
				_pmesh->flags[i] = SAMPLE_POLYFLAGS_WALK | SAMPLE_POLYFLAGS_DOOR;
			}
		}

		/// �̺κ��� ���� �ִ� ���
		/// InputGeom�� ���� ���� obj�� �а�ͼ� ������ִ� ģ���� �̺κи� �� �ذ��� �شٸ� ������ �� ������ ������
		/// �̰� �ѹ� �������̶� �̾߱⸦ �� �غ��� �ҵ�
		dtNavMeshCreateParams params;
		memset(&params, 0, sizeof(params));
		params.verts			= _pmesh->verts;
		params.vertCount		= _pmesh->nverts;
		params.polys			= _pmesh->polys;
		params.polyAreas		= _pmesh->areas;
		params.polyFlags		= _pmesh->flags;
		params.polyCount		= _pmesh->npolys;
		params.nvp				= _pmesh->nvp;
		params.detailMeshes		= _dmesh->meshes;
		params.detailVerts		= _dmesh->verts;
		params.detailVertsCount = _dmesh->nverts;
		params.detailTris		= _dmesh->tris;
		params.detailTriCount	= _dmesh->ntris;
		params.offMeshConVerts	= m_geom->getOffMeshConnectionVerts();
		params.offMeshConRad	= m_geom->getOffMeshConnectionRads();
		params.offMeshConDir	= m_geom->getOffMeshConnectionDirs();
		params.offMeshConAreas	= m_geom->getOffMeshConnectionAreas();
		params.offMeshConFlags	= m_geom->getOffMeshConnectionFlags();
		params.offMeshConUserID = m_geom->getOffMeshConnectionId();
		params.offMeshConCount	= m_geom->getOffMeshConnectionCount();
		params.walkableHeight	= _agentHeight;
		params.walkableRadius	= _agentRadius;
		params.walkableClimb	= _agentMaxClimb;
		rcVcopy(params.bmin, _pmesh->bmin);
		rcVcopy(params.bmax, _pmesh->bmax);
		params.cs = _cfg.cs;
		params.ch = _cfg.ch;
		params.buildBvTree = true;

		if (!dtCreateNavMeshData(&params, &navData, &navDataSize))
		{
			_ctx->log(RC_LOG_ERROR, "Could not build Detour navmesh.");
			return false;
		}

		_navMesh = dtAllocNavMesh();
		if (!_navMesh)
		{
			dtFree(navData);
			_ctx->log(RC_LOG_ERROR, "Could not create Detour navmesh");
			return false;
		}

		dtStatus status;

		status = _navMesh->init(navData, navDataSize, DT_TILE_FREE_DATA);
		if (dtStatusFailed(status))
		{
			dtFree(navData);
			_ctx->log(RC_LOG_ERROR, "Could not init Detour navmesh");
			return false;
		}

		status = _navQuery->init(_navMesh, 2048);
		if (dtStatusFailed(status))
		{
			_ctx->log(RC_LOG_ERROR, "Could not init Detour navmesh query");
			return false;
		}
	}

	_ctx->stopTimer(RC_TIMER_TOTAL);

	// ���� ��踦 ǥ���մϴ�.
	duLogBuildTimes(*_ctx, _ctx->getAccumulatedTime(RC_TIMER_TOTAL));
	_ctx->log(RC_LOG_PROGRESS, ">> Polymesh: %d vertices  %d polygons", _pmesh->nverts, _pmesh->npolys);

	_totalBuildTimeMs = _ctx->getAccumulatedTime(RC_TIMER_TOTAL) / 1000.0f;

// 	if (m_tool)
// 		m_tool->init(this);
// 	initToolStates(this);

	return true;
}
