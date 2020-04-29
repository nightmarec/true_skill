#pragma once
#include <vector>

class TrueSkillPlayer
{
public:
	TrueSkillPlayer(double mu, double sigma, double weight = 1.0)
		: m_mu(mu), m_sigma(sigma), m_weight(weight)
	{

	}

	~TrueSkillPlayer() {}

	double GetMu() { return m_mu; }
	double GetSigma() { return m_sigma; }
	double GetWeight() { return m_weight; }

	void SetMu(double mu) { m_mu = mu; }
	void SetSigma(double sigma) { m_sigma = sigma; }
	void SetWeight(double weight) { m_weight = weight; }

	double GetSkillValue() { return m_mu - m_sigma; }

private:
	double m_mu;
	double m_sigma;
	double m_weight;
};


class TrueSkillTeam
{
public:
	TrueSkillTeam();
	TrueSkillTeam(int rank);
	~TrueSkillTeam();

	void AddPlayer(TrueSkillPlayer* player);
	void SetRank(int rank) { m_rank = rank; }
	bool isValid();
	bool operator< (const TrueSkillTeam& rsh);

	bool GetMaxValuePlayer(TrueSkillPlayer* max_skill_player);

	int m_rank;
	std::vector<TrueSkillPlayer*> m_PlayersInTeam;
};

