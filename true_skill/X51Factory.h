#pragma once
#include "X51Player.h"
#include <list>
#include <map>
class X51Factory
{
public:
	X51Factory();
	~X51Factory();

	bool CreatePlayer(int player_num);
	void CreateLev1Player(int player_num);
	void CreateLev2Player(int player_num);
	void CreateLev3Player(int player_num);
	void CreateLev4Player(int player_num);
	void CreateLev5Player(int player_num);

	void NoticeLastPlayer(double mu, double sigma, int lev);

	std::vector<X51Player> m_players;
	std::map<int, int> m_notice_player_id;
	std::map<int, int> m_record_player_id;
};

