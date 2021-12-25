#ifndef MEM_MGR_H
#define MEM_MGR_H



#include <stdint.h>
#include "currProtector.h"




#define PA_GRADE_0		0
#define PA_GRADE_1		1
#define PA_GRADE_2		2

#define IS_PA_GRADE_LEGAL( x )	( (x==PA_GRADE_0) \
							   || (x==PA_GRADE_1) \
							   || (x==PA_GRADE_2))


#pragma pack(1)

typedef struct
{
	uint8_t ckSum;
	uint8_t isValid;
}RecordCkDef;

typedef struct
{
	CurrProtectorCfgDef currProtector;
}UsrAppDataDef;

typedef struct
{
	UsrAppDataDef data;
	RecordCkDef ck;
}UsrAppCrcDef;


typedef struct
{
	uint8_t PA;
	uint8_t PASS[3];
}PassDef;


typedef struct
{
	uint8_t ir1[3];		//额定电流整定值过载保护动作电流Ir1
	uint8_t iWarnPer10;	//负荷电流超限报警整定值(?0.1In)（弃用）
	uint8_t t1S;		//过载保护动作时间t1
	uint8_t ir2DivIr1;	//短路短延时动作电流Ir2 (*Ir1)	
	uint8_t t2S[2];		//短路短延时动作时间t2
	uint8_t ir3DivIr1;	//短路短延时动作电流Ir3 (*Ir1)	
}InBlockDef;

typedef struct
{
	uint8_t IdnAlarmVal[2];					//剩余电流超限报警整定值mA
	uint8_t IdnWarnPer;						//剩余电流预警限值(缺省60%)
	uint8_t IdnWarnS;						//剩余电流超预警/超限限值持续时长（缺省为60s）
}IdnBlockDef;

typedef struct
{
	uint8_t overVol[2];             		//过电压整定值NNN.N
	uint8_t underVol[2];             		//欠电压整定值NNN.N
	uint8_t lackPhaseVol[2];             	//缺相电压整定值NNN.N
}VolBlockDef;

typedef struct
{
	InBlockDef inBlock;						//电流整定参数块
}PARInfoDef;

typedef struct
{
	PARInfoDef data;
	RecordCkDef ck;
}PARInfoCrcDef;

#pragma pack()




extern UsrAppCrcDef	  usrAppCrc;



void MemMgrInit(void);
void CurrParaFresh( const BreakerParaInfoDef *const breakerInfo );



void memSetIr1(uint8_t *data);
void memGetIr1(uint8_t *data);
void memSetT1S(uint8_t *data);
void memGetT1S(uint8_t *data);
void memSetIr2DivIr1(uint8_t *data);
void memGetIr2DivIr1(uint8_t *data);
void memSetT2S(uint8_t *data);
void memGetT2S(uint8_t *data);
void memSetIr3DivIr1(uint8_t *data);
void memGetIr3DivIr1(uint8_t *data);

void SetLongDelayT1S(        uint8_t s );
void SetIr1(    uint32_t ir1 );

void SetIr2DivIr1( uint8_t div );
void SetIr3DivIr1( uint8_t div );

void UpDataIr1(uint8_t* bcdIr1);
void UpdateLongDelayT1S(uint8_t bcdS);
void UpDataIr2DivIr1(uint8_t* bcdDiv);
void UpdateShortDelayT2mS(uint8_t *bcdmS);
void UpDataIr3DivIr1(uint8_t* bcdDiv);


#endif

