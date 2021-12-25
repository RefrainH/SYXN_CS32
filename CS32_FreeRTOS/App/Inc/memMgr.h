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
	uint8_t ir1[3];		//���������ֵ���ر�����������Ir1
	uint8_t iWarnPer10;	//���ɵ������ޱ�������ֵ(?0.1In)�����ã�
	uint8_t t1S;		//���ر�������ʱ��t1
	uint8_t ir2DivIr1;	//��·����ʱ��������Ir2 (*Ir1)	
	uint8_t t2S[2];		//��·����ʱ����ʱ��t2
	uint8_t ir3DivIr1;	//��·����ʱ��������Ir3 (*Ir1)	
}InBlockDef;

typedef struct
{
	uint8_t IdnAlarmVal[2];					//ʣ��������ޱ�������ֵmA
	uint8_t IdnWarnPer;						//ʣ�����Ԥ����ֵ(ȱʡ60%)
	uint8_t IdnWarnS;						//ʣ�������Ԥ��/������ֵ����ʱ����ȱʡΪ60s��
}IdnBlockDef;

typedef struct
{
	uint8_t overVol[2];             		//����ѹ����ֵNNN.N
	uint8_t underVol[2];             		//Ƿ��ѹ����ֵNNN.N
	uint8_t lackPhaseVol[2];             	//ȱ���ѹ����ֵNNN.N
}VolBlockDef;

typedef struct
{
	InBlockDef inBlock;						//��������������
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

