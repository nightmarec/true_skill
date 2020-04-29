#include "pch.h"
#include "X51Player.h"
#include <vcruntime_string.h>
#include <cstdlib> 
#include <ctime>


X51Player::X51Player()
	: TrueSkillPlayer(25, 25.0 / 3.0), 
	m_match_times(0), 
	m_index(0), 
	m_avg_mu(0.0)
{
	memset(m_key_finish_rating, 0, sizeof(m_key_finish_rating));
	memset(m_finish_level_rating, 0, sizeof(m_finish_level_rating));
}


X51Player::~X51Player()
{
}


FinishLevel X51Player::RandomResults(int key)
{
	if (key <=0 || key > Key_Count)
	{
		return FL_Miss;
	}
	srand((unsigned)time(NULL));

	int key_finish = rand() % 100;
	if (key_finish >= m_key_finish_rating[key])
	{
		return FL_Miss;
	}

	int lev_rand = rand() % 100;

	int lev = FL_PERFECT;
	for (; lev < FL_Count; ++lev)
	{
		if (lev_rand < m_finish_level_rating[lev])
		{
			break;
		}

		lev_rand -= m_finish_level_rating[lev];
	}

	return (FinishLevel)lev;
}

bool X51Player::operator< (const X51Player& rhs)
{
	return m_index < rhs.m_index;
}