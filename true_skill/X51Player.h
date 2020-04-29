#pragma once
#include "TrueSkillTeam.h"

enum FinishLevel
{
	FL_Miss = -1,
	FL_PERFECT,
	FL_Great,
	FL_Good,
	FL_Bad,
	FL_Count,

};

const int Key_Count = 9; 
class X51Player : public TrueSkillPlayer
{
public:
	X51Player();
	~X51Player();

	FinishLevel RandomResults( int key);

	bool operator< (const X51Player& rhs);

public:
	int m_index;
	int m_key_finish_rating[9]; // ÿ�ֵȼ��İ�������ɵĸ���
	int m_finish_level_rating[FL_Count]; // ������ɵİ�����������۵ĸ���

	int m_match_times;
	double m_avg_mu;
};

