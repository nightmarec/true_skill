// true_skill.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include "trueskill.h"
#include "SimulationMatch.h"
#include "TrueSkillTeam.h"
#include <string>

static std::vector< std::vector< double > > players;

void ShowHelp()
{
	std::cout << "******************************************************************************** " << std::endl;
	std::cout << "SimulationMatch : 模拟TrueSkill对战命令" << std::endl;
	std::cout << "ContinueMatch : 添加对战次数命令，需执行完SimulationMatch后才可以执行" << std::endl;
	std::cout << "AddPlayer : 添加玩家命令，需执行完SimulationMatch后才可以执行,执行完，通过ContineMatch匹配" << std::endl;
	std::cout << "CheckQuality : 比较玩家对战质量命令" << std::endl;
	std::cout << "Help : 帮助命令" << std::endl;
	std::cout << "Exit : 退出命令" << std::endl;
	std::cout << "******************************************************************************** " << std::endl;
}

SimulationMatch global_sim_match;

int main()
{
	std::cout << "欢迎使用TrueSkill Demo！以下为命令介绍： " << std::endl;
	ShowHelp();

	while (true)
	{
		std::cout << "请输入命令: ";
		char c_command[100];
		std::cin >> c_command;
		std::string command = c_command;
		if (!command.compare("Exit"))
		{
			break;
		}
		else if (!command.compare("Help"))
		{
			ShowHelp();
		}
		else if (!command.compare("SimulationMatch"))
		{
			int player_num = 0;
			std::cout << "请输入对局玩家数量（至少6人）: ";
			std::cin >> player_num;

			if (!global_sim_match.CreatePlayers(player_num))
			{
				std::cout << "人数不满足要求！ " << std::endl;
				continue;
			}

			int match_time = 0;
			std::cout << "请输入对局次数: ";
			std::cin >> match_time;
			
			global_sim_match.SimulationTrueSkillMatch(match_time);
		}
		else if (!command.compare("ContinueMatch"))
		{
			int match_time = 0;
			std::cout << "请输入对局次数: ";
			std::cin >> match_time;

			global_sim_match.SimulationTrueSkillMatch(match_time);
		}
		else if (!command.compare("AddPlayer"))
		{
			double mu = 25.0;
			std::cout << "请输入玩家的mu值: ";
			std::cin >> mu;

			double sigma = 25.0;
			std::cout << "请输入玩家的sigma值: ";
			std::cin >> sigma;

			int level = 0;
			std::cout << "请输入玩家的等级（1-5，越来越强）: ";
			std::cin >> level;

			global_sim_match.AddPlayerMatch(mu, sigma, level);
		}
		else if (!command.compare("CheckQuality"))
		{
			double playerA_mu = 0.0;
			double playerA_sigma = 0.0;
			std::cout << "请玩家A的mu: ";
			std::cin >> playerA_mu;
			std::cout << "请玩家A的sigma: ";
			std::cin >> playerA_sigma;

			double playerB_mu = 0.0;
			double playerB_sigma = 0.0;
			std::cout << "请玩家B的mu: ";
			std::cin >> playerB_mu;
			std::cout << "请玩家B的sigma: ";
			std::cin >> playerB_sigma;

			TrueSkillPlayer playerA(playerA_mu, playerA_sigma, 1);
			TrueSkillPlayer playerB(playerB_mu, playerB_sigma, 1);

			double qt = Trueskill::CheckQuality(playerA, playerB);

			std::cout << "玩家的平局系数为: " << qt << std::endl;
		}
		else if (!command.compare("RandomMatch"))
		{
			int match_time = 0;
			std::cout << "请输入对局次数: ";
			std::cin >> match_time;

			global_sim_match.RandomMatch(match_time);
		}
	}
}
