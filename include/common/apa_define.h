#ifndef APA_DEFINE_H
#define APA_DEFINE_H

#include <stdint.h>
#include <vector>

#define RECTPointNum			4		//车位框点数
#define APS_DEFAULT_DIST		99999	//APS算法中默认距离值
#define APS_INVAILD_LABEL		-999	//APS算法中无效LABEL

// typedef SApaPoint_I POINT_I;

typedef struct SApaPoint_I
{
    int x;
    int y;
}SApaPoint_I;

typedef enum
{
    APA_INNER_STATUS_INIT_ING = 0,
    APA_INNER_STATUS_INIT_OK,
    APA_INNER_STATUS_INIT_FAILED,
    APA_INNER_STATUS_START_ING,
    APA_INNER_STATUS_START_OK,
    APA_INNER_STATUS_START_FAILED,
    APA_INNER_STATUS_STOP_ING,
    APA_INNER_STATUS_STOP_OK,
    APA_INNER_STATUS_STOP_FAILED,
    APA_INNER_STATUS_DESTORY_ING,
    APA_INNER_STATUS_DESTORY_OK,
    APA_INNER_STATUS_DESTORY_FAILED,
} APA_INNER_STATUS;

typedef enum
{
	E_SCENE_DEFAULT = 0,//默认，未知状态
	E_SCENE_NORMAL,		//正常场景
	E_SCENE_FLOORTILE	//地砖地
}ERECT_SCENE_TYPE;

enum ERECT_EDGE_SOD_TYPE //车位上下空间的SOD类型
{
	E_EDGESOD_DEFAULT = 0,			//默认，上下空间都未知情况
	E_EDGESOD_U_UNKNOW_D_EMPTY,		//上部空间未知，下部分为空
	E_EDGESOD_U_UNKNOW_D_SOD,		//上部空间未知，下部分为SOD
	E_EDGESOD_U_EMPTY_D_UNKNOW,		//上部空间为空，下部分为未知
	E_EDGESOD_U_EMPTY_D_EMPTY,		//上部空间为空，下部分为空
	E_EDGESOD_U_EMPTY_D_SOD,		//上部空间为空，下部分为SOD
	E_EDGESOD_U_SOD_D_UNKNOW,		//上部空间为SOD，下部分为未知
	E_EDGESOD_U_SOD_D_EMPTY,		//上部空间为SOD，下部分为空
	E_EDGESOD_U_SOD_D_SOD			//上部空间为SOD，下部分为SOD
};

enum ERECT_SOD_TYPE //车位sod类型
{
	RECTSOD_DEFAULT = -999,			//默认值
	RECTSOD_Empty = 0,				//空车位
	RECTSOD_Obstacle = 1,			//障碍物车位
	RECTSOD_UNConfirm = -1,			//不确认类型车位
	RECTSOD_INIT = 2,				//初始化车位
	RECTSOD_EXCLUDEINIT = 3,		//算法内部用
	RECTSOD_PARLLE_HAS_ROADEDGE = 4 //水平车位有路沿
};

enum E_RECT_TYPE
{
	E_RECT_VISION = 0,	//视觉车位
	E_RECT_ULTRA,		//超声波车位
	E_RECT_COMPLEX,		//超声波复合车位
	E_RECT_COMPLEX_REC,	//超声波推荐复合车位
	E_RECT_FUSE			//融合车位（视觉和超声波融合后的车位，暂时无用，待定）
};

enum //车位上下突出空间的SOD类型
{
    E_EXTRUDED_DEFAULT = 0,         //默认，上下空间都未知情况
    E_EXTRUDED_U_UNKNOW_D_EMPTY,    //上部空间未知，下部分为空
    E_EXTRUDED_U_UNKNOW_D_SOD,      //上部空间未知，下部分为SOD
    E_EXTRUDED_U_EMPTY_D_UNKNOW,    //上部空间为空，下部分为未知
    E_EXTRUDED_U_EMPTY_D_EMPTY,     //上部空间为空，下部分为空
    E_EXTRUDED_U_EMPTY_D_SOD,       //上部空间为空，下部分为SOD
    E_EXTRUDED_U_SOD_D_UNKNOW,      //上部空间为SOD，下部分为未知
    E_EXTRUDED_U_SOD_D_EMPTY,       //上部空间为SOD，下部分为空
    E_EXTRUDED_U_SOD_D_SOD          //上部空间为SOD，下部分为SOD
};

struct SApaPSRect
{
	SApaPoint_I pt[RECTPointNum];//坐标点，单位:mm
	int PStype;					 //车位形状类型，对应枚举PStype
	int label;					 //车位标号，唯一值
	int iSodType;				 //车位内障碍物状态，对应枚举ERECT_SOD_TYPE
	int iRectType;				 //车位类型，对应枚举E_RECT_TYPE
	int iOtherSideSOD; 			 //对象空间是否有障碍物，-1未知情况，1有障碍物，0无障碍物
	bool isDriveSpaceEnough;	 //泊车行驶空间是否足够，1为空间足够；0为不足
	int iDownSlotSOD;			 //车位上、下方障碍物存在情况，对应枚举ERECT_EDGE_SOD_TYPE
	int level;					 //跟踪值还是检测值 2为检测值，1为跟踪值
	int iMinOtherSideDist;		 //对向泊车空间障碍物最近距离,单位mm，默认99999,车位坐标距离障碍物的距离
	// unsigned char iRoadEdgeType; //视觉水平车位底部障碍物类型，0没有障碍物，1路沿，2大墙，99other
	int iRoadEdgeDist;			 //水平车位AB点距离路沿距离，单位mm，默认99999
	int iSceneType;				 //场景类型，对应枚举ERECT_SCENE_TYPE
	int iHasGL;					 //是否有地锁，-1为默认值，1为有地锁，0为没地锁
	int flag;					 //desay算法内部用，
    int iExtrudeSOD;			 //车位相邻障碍物突出
	SApaPSRect()
	{
		iSceneType			= E_SCENE_DEFAULT;
		iHasGL				= -1;
		// iRoadEdgeType       = 99;
		iRoadEdgeDist 		= APS_DEFAULT_DIST;
		iMinOtherSideDist 	= APS_DEFAULT_DIST;
		level 				= 0;
		iDownSlotSOD 		= E_EDGESOD_DEFAULT;
		iSodType 			= RECTSOD_UNConfirm;
		iRectType 			= E_RECT_VISION;
    	iExtrudeSOD         = E_EXTRUDED_DEFAULT;
		PStype				= 1;
		label 				= 0;
		iOtherSideSOD 		= -1;
		isDriveSpaceEnough 	= false;

		for(int i = 0; i < RECTPointNum; i++)
		{
			pt[i].x = 0;
			pt[i].y = 0;
		}
	}
};

struct apaSlotInfo
{
    SApaPSRect rectInfo;
    
    int detect_frame_count;
    int detect_as_occupy_count;
    bool is_reliable;
    apaSlotInfo()
    : detect_frame_count(0)
    , detect_as_occupy_count(0)
    , is_reliable(false)
    {

    }
};

struct apaSlotListInfo
{
    std::vector<apaSlotInfo>     WorldoutRect;       //表示车位的点集
    unsigned int                AbsoluteDeltaX;     //当前帧获取的dr值(原始的，从UartData2中获取)
    unsigned int                AbsoluteDeltaY;     //当前帧获取的dr值(原始的，从UartData2中获取)
    unsigned short              AbsoluteDeltaAngle; //当前帧获取的dr值(原始的，从UartData2中获取)
    unsigned short              UartCount;          //当前帧的uart count号(原始的，从UartData2中获取)
    int64_t                     ullFrameId; 		//当前帧的摄像头帧号 //2021-05-12 新增
	unsigned long long          ulAlgStartTime; 	//算法开始时间      //2021-05-12 新增
    unsigned long long          ulAlgEndTime; 	    //算法结束时间      //2021-05-12 新增

    apaSlotListInfo()
    : AbsoluteDeltaX(0)
    , AbsoluteDeltaY(0)
    , AbsoluteDeltaAngle(0)
    , UartCount(0)    
    , ullFrameId(0)
    , ulAlgStartTime(0)
    , ulAlgEndTime(0)
    {
        for (int i = 0; WorldoutRect.size();i++){
            WorldoutRect[i].rectInfo.pt[0].x = 0;
            WorldoutRect[i].rectInfo.pt[0].y = 0;

            WorldoutRect[i].rectInfo.pt[1].x = 0;
            WorldoutRect[i].rectInfo.pt[1].y = 0;

            WorldoutRect[i].rectInfo.pt[2].x = 0;
            WorldoutRect[i].rectInfo.pt[2].y = 0;

            WorldoutRect[i].rectInfo.pt[3].x = 0;
            WorldoutRect[i].rectInfo.pt[3].y = 0;


            WorldoutRect[i].detect_frame_count = 0;
            WorldoutRect[i].detect_as_occupy_count = 0;
            WorldoutRect[i].is_reliable = false;            

        }
    }
};


#endif
