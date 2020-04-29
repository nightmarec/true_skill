#include "pch.h"
#include "trueskill.h"
#include "TrueSkillTeam.h"

static double MU = 25;
static double TAU = MU / 3 / 100;
static double BETA = MU / 3 / 2;
static double DRAW_PROBABILITY = 0.1;

double Trueskill::CheckQuality(TrueSkillPlayer& player1, TrueSkillPlayer& player2)
{
	double c = 2 * std::pow(BETA, 2) + std::pow(player1.GetSigma(), 2) + std::pow(player2.GetSigma(), 2);
	double d = 2 * std::pow(BETA, 2) / c;

	double e = -1 * std::pow(player1.GetMu() - player2.GetMu(), 2) / (2 * c);

	return exp(e) * sqrt(d);
}

void Trueskill::CalcNewRate(std::vector<TrueSkillTeam> &Teams, double min_delta)
{
	if (Teams.empty())
	{
		return;
	}

	//std::stable_sort(Teams.begin(), Teams.end());

	int t = Teams.size();
	//我们假设每队的人数一样
	int team_players = Teams[0].m_PlayersInTeam.size();
	int size = team_players * t;
	

	std::vector< std::vector< Gaussian > > ratings(t);
	std::vector< Variable > rating_vars(size, Variable());
	std::vector< Variable > perf_vars(size, Variable());
	std::vector< Variable > team_perf_vars(t, Variable());
	std::vector< Variable > team_diff_vars(t - 1, Variable());

	std::vector< PriorFactor > build_rating_layer;
	std::vector< LikelihoodFactor > build_perf_layer;

	int player_index = 0;
	for (int team_index = 0; team_index < t; team_index++)
	{
		std::vector<TrueSkillPlayer*>::iterator iter_player = Teams[team_index].m_PlayersInTeam.begin();
		for (; iter_player != Teams[team_index].m_PlayersInTeam.end(); ++iter_player)
		{
			Gaussian rating = Gaussian((*iter_player)->GetMu(), (*iter_player)->GetSigma(), 0, 0);
			ratings[team_index].push_back(rating);
			build_rating_layer.push_back(PriorFactor(&rating_vars[player_index], &rating, TAU));
			build_perf_layer.push_back(LikelihoodFactor(&rating_vars[player_index], &perf_vars[player_index], std::pow(BETA, 2)));
			++player_index;
		}
	}

	std::vector< SumFactor > build_team_perf_layer;
	std::vector< Variable* > child_perf_vars;
	std::vector< double > coeffs;

	player_index = 0;
	for (int team_index = 0; team_index < t; team_index++)
	{
		for (unsigned int n = 0; n < Teams[team_index].m_PlayersInTeam.size(); n++)
		{
			child_perf_vars.push_back(&perf_vars[player_index++]);
			coeffs.push_back(Teams[team_index].m_PlayersInTeam[n]->GetWeight());
		}

		build_team_perf_layer.push_back(SumFactor(&team_perf_vars[team_index], child_perf_vars, coeffs));
		child_perf_vars.clear();
		coeffs.clear();
	}

	for (unsigned int i = 0; i < build_rating_layer.size(); i++)
	{
		build_rating_layer[i].down();
	}

	for (unsigned int i = 0; i < build_perf_layer.size(); i++)
	{
		build_perf_layer[i].down();
	}

	for (unsigned int i = 0; i < build_team_perf_layer.size(); i++)
	{
		build_team_perf_layer[i].down();
	}

	std::vector< SumFactor > build_team_diff_layer;
	std::function< double(double, double) > v_func, w_func;
	int calcsize;
	double draw_margin;
	coeffs.push_back(1);
	coeffs.push_back(-1);
	for (int i = 0; i < t - 1; i++)
	{
		for (int n = i; n < i + 2; n++)
		{
			child_perf_vars.push_back(&team_perf_vars[n]);
		}
		build_team_diff_layer.push_back(SumFactor(&team_diff_vars[i], child_perf_vars, coeffs));
		child_perf_vars.clear();
	}
	coeffs.clear();

	std::vector< TruncateFactor > build_trunc_layer;
	for (int i = 0; i < t - 1; i++)
	{
		calcsize = 0;
		for (int n = i; n < i + 2; n++)
		{
			calcsize += ratings[n].size();
		}
		draw_margin = calc_draw_margin(DRAW_PROBABILITY, calcsize);
		if (Teams[i].m_rank == Teams[i+1].m_rank)
		{
			v_func = v_draw;
			w_func = w_draw;
		}
		else
		{
			v_func = v_win;
			w_func = w_win;
		}

		build_trunc_layer.push_back(TruncateFactor(&team_diff_vars[i], v_func, w_func, draw_margin));
	}

	unsigned int team_diff_len = build_team_diff_layer.size();
	double delta;
	for (int i = 0; i < 10; i++)
	{
		if (team_diff_len == 1)
		{
			build_team_diff_layer[0].down();
			delta = build_trunc_layer[0].up();
		}
		else
		{
			delta = 0;
			for (unsigned int n = 0; n < team_diff_len - 1; n++)
			{
				build_team_diff_layer[n].down();
				delta = std::max(delta, build_trunc_layer[n].up());
				build_team_diff_layer[n].up(1);
			}
			for (unsigned int n = team_diff_len - 1; n > 0; n--)
			{
				build_team_diff_layer[n].down();
				delta = std::max(delta, build_trunc_layer[n].up());
				build_team_diff_layer[n].up(0);
			}
		}
		if (delta <= min_delta)
		{
			break;
		}
	}

	build_team_diff_layer[0].up(0);
	build_team_diff_layer[team_diff_len - 1].up(1);
	for (unsigned int i = 0; i < build_team_perf_layer.size(); i++)
	{
		for (unsigned int n = 0; n < build_team_perf_layer[i].getVar().size() - 1; n++)
		{
			build_team_perf_layer[i].up(n);
		}
	}

	for (unsigned int i = 0; i < build_perf_layer.size(); i++)
	{
		build_perf_layer[i].up();
	}

	for (int i = 0; i < size; i++)
	{
		int team_index = i / team_players;
		int player_index = i - team_index * team_players;
		Teams[team_index].m_PlayersInTeam[player_index]->SetMu(rating_vars[i].getVal()._mu());
		Teams[team_index].m_PlayersInTeam[player_index]->SetSigma(rating_vars[i].getVal()._sigma());
	}
}

double Trueskill::v_win( double diff, double draw_margin )
{
	double x = diff - draw_margin;
	double denom = cdf( x );

	return denom ? ( pdf( x ) / denom ) : -x;
}

double Trueskill::v_draw( double diff, double draw_margin )
{
	double abs_diff = std::abs( diff );
	double a = draw_margin - abs_diff;
	double b = -draw_margin - abs_diff;
	double denom = cdf( a ) - cdf( b );
	double numer = pdf( b ) - pdf( a );
	return (denom ? ( numer / denom ) : a) * (diff < 0 ? -1 : 1);
}

double Trueskill::w_win( double diff, double draw_margin )
{
	double x = diff - draw_margin;
	double v = v_win( diff, draw_margin );
	double w = v * ( v + x );
	
	if ( 0 < w && w < 1 )
	{
		return w;
	}
	return -1;
}

double Trueskill::w_draw( double diff, double draw_margin )
{
	double abs_diff = std::abs( diff );
	double a = draw_margin - abs_diff;
	double b = -draw_margin - abs_diff;
	double denom = cdf( a ) - cdf( b );
	if ( !denom )
	{
		return -1;
	}

	double v = v_draw( abs_diff, draw_margin );
	return std::pow( v, 2 ) + ( a * pdf( a ) - b * pdf( b ) ) / denom;;
}

double Trueskill::calc_draw_margin( double draw_probability, int size )
{
	return ppf( ( draw_probability + 1 ) / 2 ) * std::sqrt( size ) * BETA;
}

void Trueskill::update_mu( double new_mu )
{
	MU = new_mu;
	TAU = MU / 3 / 100;
	BETA = MU / 3 / 2;
}

void Trueskill::update_draw( double new_draw )
{
	DRAW_PROBABILITY = new_draw;
}