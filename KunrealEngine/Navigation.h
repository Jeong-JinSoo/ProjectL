#pragma once
#include <windows.h>
#include <vector>
#include <directxmath.h>

#include "DetourNavMesh.h"
#include "Recast.h"
#include "InputGeom.h"
#include "ChunkyTriMesh.h"
#include "DetourNavMeshQuery.h"
#include "DetourTileCache.h"

/// �̰͵��� ���� ���� �ϰ��� �� ����� ���� ���� �����Դϴ�.
/// ����ڴ� �ڽ��� �䱸�� ���� �̸� �����ؾ� �մϴ�.
///
/// ���÷� ������ ���� enumŸ���� �״�� ���Խ��ϴ�.
/// ���� �� enum �������� ���� ������ �ϴ��� �𸣴� ��Ȳ�̹Ƿ�, �ϴ� �Űܼ� ���Ǵ� ����� ���� �м��Ұ��Դϴ�.
/// 

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

enum TestType
{
	TEST_PATHFIND,
	TEST_RAYCAST
};

static const int NAVMESHSET_MAGIC = 'M' << 24 | 'S' << 16 | 'E' << 8 | 'T'; //'MSET';
static const int NAVMESHSET_VERSION = 1;

static const int TILECACHESET_MAGIC = 'T' << 24 | 'S' << 16 | 'E' << 8 | 'T'; //'TSET';
static const int TILECACHESET_VERSION = 1;
static const int EXPECTED_LAYERS_PER_TILE = 4;
static const int MAX_LAYERS = 32;

struct NavMeshSetHeader
{
	int magic;
	int version;
	int numTiles;
	dtNavMeshParams params;
};

struct NavMeshTileHeader
{
	dtTileRef tileRef;
	int dataSize;
};

struct Test
{
	Test() :
		type(),
		spos(),
		epos(),
		nspos(),
		nepos(),
		radius(0),
		includeFlags(0),
		excludeFlags(0),
		expand(false),
		straight(0),
		nstraight(0),
		polys(0),
		npolys(0),
		findNearestPolyTime(0),
		findPathTime(0),
		findStraightPathTime(0),
		next(0)
	{
	}

	~Test()
	{
		delete[] straight;
		delete[] polys;
	}

	TestType type;
	float spos[3];
	float epos[3];
	float nspos[3];
	float nepos[3];
	float radius;
	unsigned short includeFlags;
	unsigned short excludeFlags;
	bool expand;

	float* straight;
	int nstraight;
	dtPolyRef* polys;
	int npolys;

	int findNearestPolyTime;
	int findPathTime;
	int findStraightPathTime;

	Test* next;
private:
	// Explicitly disabled copy constructor and copy assignment operator.
	Test(const Test&);
	Test& operator=(const Test&);
};

// �� PathFIndbox�� �׺�Ž��� �迭�� �����ϱ� ���ϵ��� ���� ��ҵ��� ���� ��
struct PathFindbox
{
	class dtNavMesh* _navMesh;
	class dtNavMeshQuery* _navQuery;

	// PathFind�� ���� �ʿ��� �κе�
	static const int MAX_POLYS = 256;
	static const int MAX_SMOOTH = 2048;

	dtPolyRef _startRef;
	dtPolyRef _endRef;
	dtPolyRef _parent[MAX_POLYS];
	float _startPos[3];
	float _endPos[3];
	dtQueryFilter _filter;
	dtPolyRef _path[MAX_POLYS];
	int _pathCount;
	float _polyPickExt[3];

	// Straight-pathfind�� ���� �ʿ��� �κе�
	float _straightPath[MAX_POLYS * 3];
	unsigned char _straightPathFlags[MAX_POLYS];
	dtPolyRef _straightPathPolys[MAX_POLYS];
	int _nstraightPath;

	// Raycast-pathfind�� ���� �ʿ��� �κе�
	dtRaycastHit _hit;
	dtPolyRef _RaycastPathPolys;
	float _hitPos[3];

	PathFindbox();
	~PathFindbox();
};

	class Navigation
	{
	private:
		Navigation();
		~Navigation();

	public:
		// �̱��� ��ü ��ȯ �Լ�
		static Navigation& GetInstance();
		// �ʱ�ȭ �Լ�.
		void Initialize();
		// ���� �Լ�
		void Release();

		// path ���� �̹� ����� �׺�Ž� ������ �о�ɴϴ�. .bin����
		void LoadAll(const char* path, int index);
		// �׺�Ž��� ���� ���� �ϱ�
		bool HandleBuild();
		// �׺�Ž��� ������Ʈ �Ѵ�.
		void handleUpdate(const float dt);
		
		// ��ֹ��� �߰��Ѵ�.
		// pos = ��ֹ� ��ġ / radius = ��ֹ� ũ�� / height = ��ֹ� ������
		void addTempObstacle(const float* pos, float radius, float height);
		// Ư�� ��ֹ��� �����Ѵ�.
		void removeTempObstacle(const float* sp, const float* sq);
		// ��� ��ֹ��� �����Ѵ�.
		void clearAllTempObstacles();

		// ������� Ž�� �Լ�
		std::vector<std::pair<DirectX::XMFLOAT3, DirectX::XMFLOAT3>> FindStraightPath(int index);
		// �̹� Ž���� ��θ� �������� �Լ�
		std::vector<std::pair<DirectX::XMFLOAT3, DirectX::XMFLOAT3>> GetPath(int index);
		// Raycast Ž�� �Լ�. (������ο� �ε����°� �ִٸ� �ű������ ��� ǥ��
		DirectX::XMFLOAT3 FindRaycastPath(int index);

		// startpos �� endpos�� �Է��ϴ� �Լ�. float[3] ����
		void SetSEpos(int index, float sx, float sy, float sz, float ex, float ey, float ez);
		// startpos �� endpos�� �Է��ϴ� �Լ�. XMFLOAT3 ����
		void SetSEpos(int index, DirectX::XMFLOAT3 startPosition, DirectX::XMFLOAT3 endPosition);
		// startpos�� �Է��ϴ� �Լ�. float[3] ����
		void SetStartpos(int index, float x, float y, float z);
		// startpos�� �Է��ϴ� �Լ�. XMFLOAT3 ����
		void SetStartpos(int index, DirectX::XMFLOAT3 position);
		// endpos�� �Է��ϴ� �Լ�. float[3] ����
		void SetEndpos(int index, float x, float y, float z);
		// endpos�� �Է��ϴ� �Լ�. XMFLOAT3 ����
		void SetEndpos(int index, DirectX::XMFLOAT3 position);

	private:
		int rasterizeTileLayers(const int tx, const int ty, const rcConfig& cfg, struct TileCacheData* tiles, const int maxTiles);
		void getTilePos(const float* pos, int& tx, int& ty);
		void renderCachedTile(const int tx, const int ty, const int type);
		void renderCachedTileOverlay(const int tx, const int ty, double* proj, double* model, int* view);
		// sp, sq�� ������� ��ֹ��� ref ���� ��ȯ�ϴ� �Լ�. sp,sq�� ���� ���� ���� �𸣰���
		dtObstacleRef hitTestObstacle(const dtTileCache* tc, const float* sp, const float* sq);
		static bool isectSegAABB(const float* sp, const float* sq,
								const float* amin, const float* amax,
								float& tmin, float& tmax);
	private:
		class InputGeom* _geom;
		PathFindbox _package[5];

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

		// solo_mesh -> temp+obstacle�� �ٲٸ鼭 �߰��� �κ�
		struct LinearAllocator* _talloc;
		struct FastLZCompressor* _tcomp;
		struct MeshProcess* _tmproc;

		class dtTileCache* _tileCache;

		float _cacheBuildTimeMs;
		int _cacheCompressedSize;
		int _cacheRawSize;
		int _cacheLayerCount;
		unsigned int _cacheBuildMemUsage;

		enum DrawMode
		{
			DRAWMODE_NAVMESH,
			DRAWMODE_NAVMESH_TRANS,
			DRAWMODE_NAVMESH_BVTREE,
			DRAWMODE_NAVMESH_NODES,
			DRAWMODE_NAVMESH_PORTALS,
			DRAWMODE_NAVMESH_INVIS,
			DRAWMODE_MESH,
			DRAWMODE_CACHE_BOUNDS,
			MAX_DRAWMODE
		};

		DrawMode _drawMode;

		int _maxTiles;
		int _maxPolysPerTile;
		float _tileSize = 48;
	};