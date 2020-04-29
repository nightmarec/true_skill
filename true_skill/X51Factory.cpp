#include "pch.h"
#include "X51Factory.h"
#include <cstdlib> 
#include <ctime>


X51Factory::X51Factory()
{
}


X51Factory::~X51Factory()
{
}


bool X51Factory::CreatePlayer(int player_num)
{
	if (player_num < 6)
	{
		return false;
	}

	m_players.clear();
	m_notice_player_id.clear();
	m_record_player_id.clear();
	int avg_num = player_num / 5;
	int lev3_num = player_num - player_num / 5 * 4;

	m_record_player_id[1] = m_players.size();
	CreateLev1Player(avg_num);

	m_record_player_id[2] = m_players.size();
	CreateLev2Player(avg_num);

	m_record_player_id[3] = m_players.size();
	CreateLev3Player(lev3_num);

	m_record_player_id[4] = m_players.size();
	CreateLev4Player(avg_num);

	m_record_player_id[5] = m_players.size();
	CreateLev5Player(avg_num);

	X51Player player;
	for (int key = 0; key < 9; ++key)
	{
		player.m_key_finish_rating[key] = 100;
	}
	player.m_finish_level_rating[FL_PERFECT] = 100;
	player.m_index = m_players.size();
	m_players.push_back(player);

	return true;
}

void X51Factory::CreateLev1Player(int player_num)
{
	srand((unsigned)time(NULL));
	// 最大按键5， perfect概率小于20
	for (int i = 0; i < player_num; ++i)
	{
		X51Player player;
		for (int key = 0; key < 5; ++key)
		{
			if (key < 3)
			{
				player.m_key_finish_rating[key] = 100;
			}
			else
			{
				//随机递减
				player.m_key_finish_rating[key] = player.m_key_finish_rating[key -1] - 25 - rand() % 25;
				if (player.m_key_finish_rating[key] < 0)
				{
					player.m_key_finish_rating[key] = 0;
				}
			}
		}

		//除了perfect，其他都平均吧
		player.m_finish_level_rating[FL_PERFECT] = rand() % 20;
		int average = (100 - player.m_finish_level_rating[FL_PERFECT])/3;
		player.m_finish_level_rating[FL_Great] = average;
		player.m_finish_level_rating[FL_Good] = 100 - 2 * average - player.m_finish_level_rating[FL_PERFECT];
		player.m_finish_level_rating[FL_Bad] = average;
		player.m_index = m_players.size();

		m_players.push_back(player);
	}
}

void X51Factory::CreateLev2Player(int player_num)
{
	// 最大按键6， perfect概率小于25
	srand((unsigned)time(NULL));
	for (int i = 0; i < player_num; ++i)
	{
		X51Player player;
		for (int key = 0; key < 6; ++key)
		{
			if (key < 4)
			{
				player.m_key_finish_rating[key] = 100;
			}
			else
			{
				player.m_key_finish_rating[key] = player.m_key_finish_rating[key -1] - 20 - rand() % 20;
				if (player.m_key_finish_rating[key] < 0)
				{
					player.m_key_finish_rating[key] = 0;
				}
			}
			
		}

		//除了perfect，其他都平均吧
		player.m_finish_level_rating[FL_PERFECT] = rand() % 15 + 10;
		int average = (100 - player.m_finish_level_rating[FL_PERFECT]) / 3;
		player.m_finish_level_rating[FL_Great] = average;
		player.m_finish_level_rating[FL_Good] = 100 - 2 * average - player.m_finish_level_rating[FL_PERFECT];
		player.m_finish_level_rating[FL_Bad] = average;
		player.m_index = m_players.size();

		m_players.push_back(player);
	}
}

void X51Factory::CreateLev3Player(int player_num)
{
	// 最大按键7,perfect概率小于30
	srand((unsigned)time(NULL));
	for (int i = 0; i < player_num; ++i)
	{
		X51Player player;
		for (int key = 0; key < 7; ++key)
		{
			if (key < 5)
			{
				player.m_key_finish_rating[key] = 100;
			}
			else
			{
				//随机30%递减
				player.m_key_finish_rating[key] = player.m_key_finish_rating[key - 1] - 20 - rand() % 20;
				if (player.m_key_finish_rating[key] < 0)
				{
					player.m_key_finish_rating[key] = 0;
				}
			}
		}

		//除了perfect，其他都平均吧
		player.m_finish_level_rating[FL_PERFECT] = rand() % 20 + 20;
		int average = (100 - player.m_finish_level_rating[FL_PERFECT]) / 3;
		player.m_finish_level_rating[FL_Great] = average;
		player.m_finish_level_rating[FL_Good] = 100 - 2 * average - player.m_finish_level_rating[FL_PERFECT];
		player.m_finish_level_rating[FL_Bad] = average;
		player.m_index = m_players.size();

		m_players.push_back(player);
	}
}

void X51Factory::CreateLev4Player(int player_num)
{
	// 最大按键8,perfect概率小于40
	srand((unsigned)time(NULL));
	for (int i = 0; i < player_num; ++i)
	{
		X51Player player;
		for (int key = 0; key < 9; ++key)
		{
			if (key < 6)
			{
				player.m_key_finish_rating[key] = 100;
			}
			else
			{
				//随机20%递减
				player.m_key_finish_rating[key] = player.m_key_finish_rating[key - 1] - 10 - rand() % 20;
			}
		}

		//除了perfect，其他都平均吧
		player.m_finish_level_rating[FL_PERFECT] = rand() % 30 + 30;
		int average = (100 - player.m_finish_level_rating[FL_PERFECT]) / 3;
		player.m_finish_level_rating[FL_Great] = average;
		player.m_finish_level_rating[FL_Good] = 100 - 2 * average - player.m_finish_level_rating[FL_PERFECT];
		player.m_finish_level_rating[FL_Bad] = average;
		player.m_index = m_players.size();

		m_players.push_back(player);
	}
}

void X51Factory::CreateLev5Player(int player_num)
{
	// 最大按键9,perfect概率小于50
	srand((unsigned)time(NULL));
	for (int i = 0; i < player_num; ++i)
	{
		X51Player player;
		for (int key = 0; key < 9; ++key)
		{
			if (key < 7)
			{
				player.m_key_finish_rating[key] = 100;
			}
			else
			{
				//随机20%递减
				player.m_key_finish_rating[key] = player.m_key_finish_rating[key - 1] - rand() % 20;
			}
		}

		//除了perfect，其他都平均吧
		player.m_finish_level_rating[FL_PERFECT] = rand() % 50 + 50;
		int average = (100 - player.m_finish_level_rating[FL_PERFECT]) / 3;
		player.m_finish_level_rating[FL_Great] = average;
		player.m_finish_level_rating[FL_Good] = 100 - 2 * average - player.m_finish_level_rating[FL_PERFECT];
		player.m_finish_level_rating[FL_Bad] = average;
		player.m_index = m_players.size();

		m_players.push_back(player);
	}
}

void X51Factory::NoticeLastPlayer(double mu, double sigma, int lev)
{
	if (m_players.empty())
	{
		return;
	}
	std::vector<X51Player>::reverse_iterator riter = m_players.rbegin();
	riter->SetMu(mu);
	riter->SetSigma(sigma);
	m_notice_player_id[lev] = riter->m_index;
}