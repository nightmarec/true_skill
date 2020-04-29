#include "pch.h"
#include "SimulationMatch.h"
#include <list>
#include "X51Player.h"
#include "trueskill.h"
#include <algorithm>
#include <iostream>
#include <iosfwd>
#include <fstream>
#include <string>
#include <stdlib.h>
const int MATCH_PLAYER_NUM = 6;

struct MatchResult
{
	MatchResult() : score(0)
	{}

	bool operator < (const MatchResult& rhs)
	{
		return score > rhs.score;
	}

	X51Player player;
	int score;
};

SimulationMatch::SimulationMatch()
{
	ReadMatchLevel(m_match_levels, "./match_level.txt");
}


SimulationMatch::~SimulationMatch()
{
}

bool SimulationMatch::CreatePlayers(int num)
{
	return m_factory.CreatePlayer(num);
}

void SimulationMatch::SimulationTrueSkillMatch(int num)
{
	std::string record_file = "LevPlayer.csv";
	std::string notice_file = "add_player.csv";
	std::string result_file = "result.csv";

	for (int match_round = 0; match_round < num; ++match_round)
	{
		TrueSkillOneRoundMatch();
		std::cout << "match_round " << match_round + 1 << std::endl;
		//记录玩家数值变化
		
		for (std::map<int, int>::iterator iter = m_factory.m_record_player_id.begin(); iter != m_factory.m_record_player_id.end(); ++iter)
		{
			WriteRecordPlayer(record_file, iter->first, iter->second, match_round);
		}

		
		for (std::map<int, int>::iterator iter = m_factory.m_notice_player_id.begin(); iter != m_factory.m_notice_player_id.end(); ++iter)
		{
			WriteRecordPlayer(notice_file, iter->first, iter->second, match_round);
		}
	}

	WritePlayersInfo(m_factory.m_players, "result.csv");

	std::cout << "模拟对局后玩家的水平数据为: " << result_file << std::endl;
	std::cout << "每个等级首位玩家数据输出文件为: " << record_file << std::endl;
	if (!m_factory.m_notice_player_id.empty())
	{
		std::cout << "新添加的Player数据输出record_file: " << notice_file << std::endl;
	}
	
}

void SimulationMatch::RandomMatch(int num)
{
	for (int match_round = 0; match_round < num; ++match_round)
	{
		unsigned int all_player_size = m_factory.m_players.size();
		std::random_shuffle(m_factory.m_players.begin(), m_factory.m_players.end());
		std::list<X51Player> all_players;
		for (std::vector<X51Player>::iterator iter = m_factory.m_players.begin(); iter != m_factory.m_players.end(); ++iter)
		{
			all_players.push_back(*iter);
		}

		std::list<X51Player> finish_match_list;
		while (all_players.size() >= MATCH_PLAYER_NUM)
		{
			std::list<X51Player> match_list;
			for (int i = 0; i < MATCH_PLAYER_NUM; ++i)
			{
				match_list.push_back(all_players.front());
				all_players.pop_front();
			}

			double total_mu = 0.0;
			std::list<X51Player>::iterator iter = match_list.begin();
			for (; iter != match_list.end(); ++iter)
			{
				total_mu += iter->GetMu();
			}
			double avg_mu = total_mu / match_list.size();

			iter = match_list.begin();
			for (; iter != match_list.end(); ++iter)
			{
				iter->m_avg_mu = avg_mu;
			}

			finish_match_list.splice(finish_match_list.begin(), match_list);
		}

		if (all_player_size - finish_match_list.size() < MATCH_PLAYER_NUM)
		{
			finish_match_list.splice(finish_match_list.begin(), all_players);
		}

		//将结果重新赋值给m_factory
		m_factory.m_players.clear();
		for (std::list<X51Player>::iterator iter = finish_match_list.begin(); iter != finish_match_list.end(); ++iter)
		{
			iter->m_match_times = 0;
			m_factory.m_players.push_back(*iter);
		}

		std::string record_file = "sort_LevPlayer.csv";
		for (std::map<int, int>::iterator iter = m_factory.m_record_player_id.begin(); iter != m_factory.m_record_player_id.end(); ++iter)
		{
			WriteRecordPlayer(record_file, iter->first, iter->second, match_round);
		}
	}
}

void SimulationMatch::WriteRecordPlayer(std::string file, int lv, int index, int match_round)
{
	std::ofstream _csvOut;
	_csvOut.open(file, std::ios::out | std::ios::app);

	for (std::vector<X51Player>::iterator iter = m_factory.m_players.begin(); iter != m_factory.m_players.end(); ++iter)
	{
		if (iter->m_index == index)
		{
			_csvOut << lv << "," << match_round << "," << iter->GetMu() << "," << iter->GetSigma() << "," << iter->m_avg_mu<< std::endl;
			break;
		}
	}
	_csvOut.close();
}

void SimulationMatch::AddPlayerMatch(double mu, double sigma, int lev)
{
	if (lev == 1)
	{
		m_factory.CreateLev1Player(1);
	}
	else if (lev == 2)
	{
		m_factory.CreateLev2Player(1);
	}
	else if (lev == 3)
	{
		m_factory.CreateLev3Player(1);
	}
	else if (lev == 4)
	{
		m_factory.CreateLev4Player(1);
	}
	else if (lev == 5)
	{
		m_factory.CreateLev5Player(1);
	}
	else
	{
		return;
	}

	m_factory.NoticeLastPlayer(mu, sigma, lev);
}

void SimulationMatch::TrueSkillOneRoundMatch()
{
	unsigned int all_player_size = m_factory.m_players.size();
 	std::random_shuffle(m_factory.m_players.begin(), m_factory.m_players.end());
	std::list<X51Player> all_players;
	for (std::vector<X51Player>::iterator iter = m_factory.m_players.begin(); iter != m_factory.m_players.end(); ++iter)
	{
		all_players.push_back(*iter);
	}

	std::list<X51Player> high_quality_list;
	std::list<X51Player> low_quality_list;
	std::list<X51Player> no_quality_list;
	std::list<X51Player> finish_match_list;

	while (all_player_size != finish_match_list.size())
	{
		//将未比赛的玩家放入高质量队列中
		int player_size = all_players.size();
		for (int i = 0; i < MATCH_PLAYER_NUM && i < player_size; i++)
		{
			X51Player& front_player = all_players.front();
			high_quality_list.push_back(front_player);
			all_players.pop_front();
		}
		//在高质量队伍里面匹配玩家
		TrueSkillMatchInList(high_quality_list, finish_match_list, 0.7);
		//高质量队列里面玩家匹配次数超过10次，则下放到低质量匹配队列
		TrueSkillChangeQulityList(high_quality_list, low_quality_list, 10);

		//在低质量队伍里面匹配玩家
		TrueSkillMatchInList(low_quality_list, finish_match_list, 0.35);
		//低质量队列里面玩家匹配次数超过20次，则下放到无质量匹配队列
		TrueSkillChangeQulityList(low_quality_list, no_quality_list, 20);

		//无质量队列匹配玩家
		TrueSkillMatchInList(no_quality_list, finish_match_list, -1);

		if (all_player_size - finish_match_list.size() < MATCH_PLAYER_NUM)
		{
			finish_match_list.splice(finish_match_list.begin(), no_quality_list);
		}
	}

	//将结果重新赋值给m_factory
	m_factory.m_players.clear();
	for (std::list<X51Player>::iterator iter = finish_match_list.begin(); iter != finish_match_list.end(); ++iter)
	{
		iter->m_match_times = 0;
		m_factory.m_players.push_back(*iter);
	}
}

void SimulationMatch::TrueSkillMatchInList(std::list<X51Player>& list, std::list<X51Player>& finish_match_list, double draw_rate)
{
	if (draw_rate < 0.0)
	{
		while (list.size() >= MATCH_PLAYER_NUM)
		{
			std::list<X51Player> match_list;
			for (int i = 0; i < MATCH_PLAYER_NUM; ++i)
			{
				
				match_list.push_back(list.front());
				list.pop_front();

				MatchPK(match_list, finish_match_list);
			}
		}
	}
	else
	{
		std::list<X51Player> no_match_list;

		while (!list.empty())
		{
			std::list<X51Player> match_list;

			X51Player& front_player = list.front();
			match_list.push_back(front_player);
			std::list<X51Player>::iterator iter = list.begin();
			iter++;
			for (; iter != list.end(); ++iter)
			{
				CanTrueSkillMatch(match_list, *iter, draw_rate);

				if (match_list.size() >= MATCH_PLAYER_NUM)
				{
					break;
				}
			}

			if (match_list.size() < MATCH_PLAYER_NUM)
			{
				X51Player& no_match_player = list.front();
				no_match_player.m_match_times++;
				no_match_list.push_back(no_match_player);
				list.pop_front();

				if (list.size() < MATCH_PLAYER_NUM)
				{
					std::list<X51Player>::iterator iter_cannt_match = list.begin();
					for (; iter_cannt_match != list.end(); ++iter_cannt_match)
					{
						iter_cannt_match->m_match_times++;
					}

					no_match_list.splice(no_match_list.begin(), list);
					list.clear();

					break;
				}
			}
			else
			{
				std::list<X51Player>::iterator iter_match = match_list.begin();
				for (; iter_match != match_list.end(); ++iter_match)
				{
					EarsePlayerInList(list, iter_match->m_index);
				}

				MatchPK(match_list, finish_match_list);
			}
		}

		list = no_match_list;
	}
}

void SimulationMatch::CanTrueSkillMatch(std::list<X51Player>& list, X51Player player, double draw_rate)
{
	for (std::list<X51Player>::iterator iter = list.begin(); iter != list.end(); ++iter)
	{
		if (Trueskill::CheckQuality(*iter, player) < draw_rate)
			return;
	}

	list.push_back(player);
}

void SimulationMatch::EarsePlayerInList(std::list<X51Player>& list, int index)
{
	for (std::list<X51Player>::iterator iter = list.begin(); iter != list.end(); ++iter)
	{
		if (iter->m_index == index)
		{
			list.erase(iter);
			break;
		}
	}
}

void SimulationMatch::TrueSkillChangeQulityList(std::list<X51Player>& out_list, std::list<X51Player>& in_list, int match_num)
{
	for (std::list<X51Player>::iterator iter = out_list.begin(); iter != out_list.end(); )
	{
		if (iter->m_match_times >= match_num)
		{
			in_list.push_back(*iter);
			iter = out_list.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void SimulationMatch::MatchPK(std::list<X51Player>& list, std::list<X51Player>& finish_match_list)
{
	if (list.size() != MATCH_PLAYER_NUM)
	{
		return;
	}

	std::vector<int> match_level;
	SelectMatchLevel(list.front().GetMu(), match_level);


	std::vector<MatchResult> match_results;

	double total_mu = 0.0;
	std::list<X51Player>::iterator iter = list.begin();
	for (; iter != list.end(); ++iter)
	{
		MatchResult result;
		result.score = CalcPlayerScore(*iter, match_level);
		result.player = *iter;
		total_mu += iter->GetMu();
		match_results.push_back(result);
	}

	std::sort(match_results.begin(), match_results.end());
	
	int last_score = 0;
	int rank = 0;

	std::vector<TrueSkillTeam> teams_rank;
	std::vector<MatchResult>::iterator iter_res = match_results.begin();
	for (; iter_res != match_results.end(); ++iter_res)
	{
		if (last_score > iter_res->score)
		{
			rank++;
		}

		last_score = iter_res->score;
		iter_res->player.m_avg_mu = total_mu / list.size();
		TrueSkillTeam team(rank);
		team.AddPlayer(&(iter_res->player));

		teams_rank.push_back(team);
	}

	Trueskill::CalcNewRate(teams_rank);

	std::vector<TrueSkillTeam>::iterator iter_team = teams_rank.begin();
	for (; iter_team != teams_rank.end(); ++iter_team)
	{
		X51Player* player = static_cast<X51Player*>(iter_team->m_PlayersInTeam[0]);
		finish_match_list.push_back(*player);
	}
}

void SimulationMatch::SelectMatchLevel(double GetSkillValue, std::vector<int>& match_level)
{
	int level = 0;
	if (GetSkillValue < 20.0)
	{
		level = rand() % 7;
	}
	else if (GetSkillValue < 25.1)
	{
		level = 2 + rand() % 8;
	}
	else if (GetSkillValue < 30.0)
	{
		level = 4 + rand() % 6;
	}
	else
	{
		level = 5 + rand() % 5;
	}

	match_level = m_match_levels[level];
}

int SimulationMatch::CalcPlayerScore(X51Player& player, const std::vector<int>& match_level )
{
	int perfect = 0;
	int combo = 0;
	int score = 0;
	std::vector<int>::const_iterator iter = match_level.begin();
	for (; iter != match_level.end(); ++iter)
	{
		FinishLevel res = player.RandomResults(*iter);
		score += CalcScore(res, perfect, combo);
	}

	return score;
}

int SimulationMatch::CalcScore(const FinishLevel& result, int& perfect, int& combo)
{
	int score = 0;
	if (result == FL_Bad)
	{
		score = 300;
		combo++;
		perfect = 0;
	}
	else if (result == FL_Good)
	{
		score = 500;
		combo++;
		perfect = 0;
	}
	else if (result == FL_Great)
	{
		score = 700;
		combo++;
		perfect = 0;
	}
	else if (result == FL_PERFECT)
	{
		score = 1000;
		if (perfect == 2)
		{
			score = 1400;
		}
		else if (perfect == 3)
		{
			score = 1900;
		}
		else if (perfect == 4)
		{
			score = 2500;
		}
		else if (perfect >= 5)
		{
			score = 3200;
		}
		combo++;
		perfect++;
	}
	else
	{
		score = 0;
		combo = 0;
		perfect = 0;
	}

	if (combo)
	{
		if (combo <= 100)
		{
			double r = 1.0 + 0.005 * combo;
			score = (int)(score * r);
		}
		else
		{
			score = (int)(score * 1.5);
		}
	}

	return score;
}

std::vector< std::string > SimulationMatch::SplitString(const std::string& str, const std::string& pattern)
{
	std::vector<std::string> subStrings;
	if (str.empty())
		return subStrings;
	std::string strs = str + pattern;

	size_t pos = strs.find(pattern);
	size_t size = strs.size();

	while (pos != std::string::npos) {
		std::string sub = strs.substr(0, pos);
		subStrings.push_back(sub);
		strs = strs.substr(pos + 1, size);
		pos = strs.find(pattern);
	}

	return subStrings;

}

void SimulationMatch::ReadMatchLevel(std::map<int, std::vector<int>>& match_levels, std::string file)
{
	if (file.empty())
	{
		return;
	}
	std::ifstream _csvInput(file, std::ios::in);
	std::string _Oneline;

	while (getline(_csvInput, _Oneline))
	{
		std::vector<std::string> data = SplitString(_Oneline, " ");
		if (data.size() < 2)
			return;

		int level = atoi(data[0].c_str());
		std::vector<std::string> ml = SplitString(data[1], ",");
		std::vector<int> match_level;
		for (std::vector<std::string>::iterator iter = ml.begin(); iter != ml.end(); ++iter)
		{
			match_level.push_back(atoi(iter->c_str()));
		}

		match_levels[level] = match_level;
	}
}

void SimulationMatch::WritePlayersInfo(std::vector<X51Player>& players, std::string file)
{
	std::sort(players.begin(), players.end());

	if (file.empty())
	{
		return;
	}
	std::ofstream _csvOut;
	_csvOut.open(file, std::ios::out);

	_csvOut << "index" << "," << "mu" << "," << "sigma" << std::endl;
	for (std::vector<X51Player>::iterator iter = players.begin(); iter != players.end(); ++iter)
	{
		_csvOut << iter->m_index << "," << iter->GetMu() << "," << iter->GetSigma() << std::endl;
	}
	_csvOut.close();
}