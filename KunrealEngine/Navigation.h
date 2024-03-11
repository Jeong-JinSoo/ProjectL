#pragma once

#include "DetourNavMesh.h"
#include "Recast.h"

/// �̰͵��� ���� ���� �ϰ��� �� ����� ���� ���� �����Դϴ�.
/// ����ڴ� �ڽ��� �䱸�� ���� �̸� �����ؾ� �մϴ�.
///
/// ���÷� ������ ���� enumŸ���� �״�� ���Խ��ϴ�.
/// ���� �� enum �������� ���� ������ �ϴ��� �𸣴� ��Ȳ�̹Ƿ�, �ϴ� �Űܼ� ���Ǵ� ����� ���� �м��Ұ��Դϴ�.
enum SamplePolyAreas
{
	SAMPLE_POLYAREA_GROUND,
	SAMPLE_POLYAREA_WATER,
	SAMPLE_POLYAREA_ROAD,
	SAMPLE_POLYAREA_DOOR,
	SAMPLE_POLYAREA_GRASS,
	SAMPLE_POLYAREA_JUMP
};
enum SamplePolyFlags
{
	SAMPLE_POLYFLAGS_WALK = 0x01,		// �ȱ� �ɷ� (����, Ǯ, ����)
	SAMPLE_POLYFLAGS_SWIM = 0x02,		// ���� �ɷ� (��)
	SAMPLE_POLYFLAGS_DOOR = 0x04,		// �� ��� �ɷ�
	SAMPLE_POLYFLAGS_JUMP = 0x08,		// ���� �ɷ�
	SAMPLE_POLYFLAGS_DISABLED = 0x10,	// ��Ȱ��ȭ�� ������
	SAMPLE_POLYFLAGS_ALL = 0xffff		// ��� �ɷ�
};
enum SamplePartitionType
{
	SAMPLE_PARTITION_WATERSHED,
	SAMPLE_PARTITION_MONOTONE,
	SAMPLE_PARTITION_LAYERS
};

class Navigation
{
public:
	Navigation();
	~Navigation();

public:
	void Initialize();
	void Terminate();

	bool handleBuild();

private:
	class dtNavMesh* _navMesh;
	class dtNavMeshQuery* _navQuery;

	bool _keepInterResults;
	float _totalBuildTimeMs;

	unsigned char* _triareas;
	rcHeightfield* _solid;
	rcCompactHeightfield* _chf;
	rcContourSet* _cset;
	rcPolyMesh* _pmesh;
	rcConfig _cfg;
	rcPolyMeshDetail* _dmesh;
	rcContext* _ctx;

	unsigned char _navMeshDrawFlags;

	float _cellSize;
	float _cellHeight;
	float _agentHeight;
	float _agentRadius;
	float _agentMaxClimb;
	float _agentMaxSlope;
	float _regionMinSize;
	float _regionMergeSize;
	float _edgeMaxLen;
	float _edgeMaxError;
	float _vertsPerPoly;
	float _detailSampleDist;
	float _detailSampleMaxError;
	int _partitionType;

	bool _filterLowHangingObstacles;
	bool _filterLedgeSpans;
	bool _filterWalkableLowHeightSpans;
};                                             