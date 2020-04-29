#include "pch.h"
#include "TrueSkillTeam.h"


TrueSkillTeam::TrueSkillTeam() :
	m_rank(0)
{
}

TrueSkillTeam::TrueSkillTeam(int rank) :
	m_rank(rank)
{
}


TrueSkillTeam::~TrueSkillTeam()
{
}

bool TrueSkillTeam::operator< (const TrueSkillTeam& rsh)
{
	return m_rank < rsh.m_rank;
}

void TrueSkillTeam::AddPlayer(TrueSkillPlayer* player)
{
	m_PlayersInTeam.push_back(player);
}

bool TrueSkillTeam::isValid()
{
	return !m_PlayersInTeam.empty();
}

bool TrueSkillTeam::GetMaxValuePlayer(TrueSkillPlayer* max_skill_player)
{
	if (!isValid())
	{
		return false;
	}

	max_skill_player = m_PlayersInTeam[0];
	std::vector<TrueSkillPlayer*>::iterator iter = m_PlayersInTeam.begin();
	++iter;
	for (; iter != m_PlayersInTeam.end(); ++iter)
	{
		if ((*iter)->GetSkillValue() > max_skill_player->GetSkillValue())
		{
			max_skill_player = *iter;
		}
	}
	return true;
}