#pragma once

#include "pch.h"
#include <cmath>
#include <vector>
#include <functional>
#include "matrix.h"
#include "gaussian.h"
#include "factorgraph.h"
#include "TrueSkillTeam.h"

class Trueskill
{
public:
	Trueskill() {};
	~Trueskill() {};

	static double CheckQuality(TrueSkillPlayer& player1, TrueSkillPlayer& player2);
	static void CalcNewRate(std::vector<TrueSkillTeam> &Teams, double min_delta = 0.0001);

private:
	static double v_win(double diff, double draw_margin);
	static double v_draw(double diff, double draw_margin);
	static double w_win(double diff, double draw_margin);
	static double w_draw(double diff, double draw_margin);
	static double calc_draw_margin(double draw_probability, int size);
	static void update_mu(double new_mu);
	static void update_draw(double new_draw);
};


// BaseMath
double pdf( double x, double mu = 0, double sigma = 1 );
double cdf( double x, double mu = 0, double sigma = 1 );
double ppf( double x, double mu = 0, double sigma = 1 );