#pragma once
#include "X51Factory.h"
#include <map>
class SimulationMatch
{
public:
	SimulationMatch();
	~SimulationMatch();

	//创建玩家函数
	bool CreatePlayers(int num);
	//模拟对战函数，num为对局循环次数
	void SimulationTrueSkillMatch(int num);
	//添加玩家
	void AddPlayerMatch(double mu, double sigma, int lev);
	//模拟顺序匹配
	void RandomMatch(int num);
private:
	//使用TrueSkill匹配和比赛一局
	void TrueSkillOneRoundMatch();
	//在队列中的玩家进行匹配
	void TrueSkillMatchInList(std::list<X51Player>& list, std::list<X51Player>& finish_match_list, double draw_rate);
	//更换匹配队列
	void TrueSkillChangeQulityList(std::list<X51Player>& out_list, std::list<X51Player>& in_list, int match_num);
	//是否两个玩家是否可以匹配
	void CanTrueSkillMatch(std::list<X51Player>& list, X51Player player, double draw_rate);
	//很久index删除在队列中的玩家
	void EarsePlayerInList(std::list<X51Player>& list, int index);

	//计算玩家获得的分数
	int CalcPlayerScore(X51Player& player, const std::vector<int>& match_level);
	//计算一个按键评分玩家的分数
	int CalcScore(const FinishLevel& result, int& perfect, int& combo);
	//根据mu值分配对局
	void SelectMatchLevel(double GetSkillValue, std::vector<int>& match_level);

	//玩家对局比赛并更新Trueskill分数
	void MatchPK(std::list<X51Player>& list, std::list<X51Player>& finish_match_list);
	//分割字符串
	std::vector< std::string > SplitString(const std::string& str, const std::string& pattern);
	//读取对局关卡
	void ReadMatchLevel(std::map<int, std::vector<int>>& match_levels, std::string file);
	//记录想输出的玩家数据
	void WriteRecordPlayer(std::string file, int lv, int index, int match_round);
	//写入玩家数据
	void WritePlayersInfo(std::vector<X51Player>& players, std::string file);

private:
	//玩家工厂
	X51Factory m_factory;
	//关卡存储数据
	std::map<int, std::vector<int>> m_match_levels;
};

