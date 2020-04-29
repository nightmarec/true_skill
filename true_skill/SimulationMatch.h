#pragma once
#include "X51Factory.h"
#include <map>
class SimulationMatch
{
public:
	SimulationMatch();
	~SimulationMatch();

	//������Һ���
	bool CreatePlayers(int num);
	//ģ���ս������numΪ�Ծ�ѭ������
	void SimulationTrueSkillMatch(int num);
	//������
	void AddPlayerMatch(double mu, double sigma, int lev);
	//ģ��˳��ƥ��
	void RandomMatch(int num);
private:
	//ʹ��TrueSkillƥ��ͱ���һ��
	void TrueSkillOneRoundMatch();
	//�ڶ����е���ҽ���ƥ��
	void TrueSkillMatchInList(std::list<X51Player>& list, std::list<X51Player>& finish_match_list, double draw_rate);
	//����ƥ�����
	void TrueSkillChangeQulityList(std::list<X51Player>& out_list, std::list<X51Player>& in_list, int match_num);
	//�Ƿ���������Ƿ����ƥ��
	void CanTrueSkillMatch(std::list<X51Player>& list, X51Player player, double draw_rate);
	//�ܾ�indexɾ���ڶ����е����
	void EarsePlayerInList(std::list<X51Player>& list, int index);

	//������һ�õķ���
	int CalcPlayerScore(X51Player& player, const std::vector<int>& match_level);
	//����һ������������ҵķ���
	int CalcScore(const FinishLevel& result, int& perfect, int& combo);
	//����muֵ����Ծ�
	void SelectMatchLevel(double GetSkillValue, std::vector<int>& match_level);

	//��ҶԾֱ���������Trueskill����
	void MatchPK(std::list<X51Player>& list, std::list<X51Player>& finish_match_list);
	//�ָ��ַ���
	std::vector< std::string > SplitString(const std::string& str, const std::string& pattern);
	//��ȡ�Ծֹؿ�
	void ReadMatchLevel(std::map<int, std::vector<int>>& match_levels, std::string file);
	//��¼��������������
	void WriteRecordPlayer(std::string file, int lv, int index, int match_round);
	//д���������
	void WritePlayersInfo(std::vector<X51Player>& players, std::string file);

private:
	//��ҹ���
	X51Factory m_factory;
	//�ؿ��洢����
	std::map<int, std::vector<int>> m_match_levels;
};

