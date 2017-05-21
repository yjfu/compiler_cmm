//
// Created by 傅永健 on 16/10/31.
// Copyright (c) 2016 ___FULLUSERNAME___. All rights reserved.
//

#ifndef COMPILER_SYNTACTICANALYSER_H
#define COMPILER_SYNTACTICANALYSER_H
class WrongProcesser;
#include "LexicalAnalyser.h"
#include "WrongProcesser.h"
#include <map>
#include <vector>
#include <set>
#include <stack>
struct production {
	int belong;
	vector<int> follow_list;
	set<int>select;
	production(int belong_to) {
		belong = belong_to;
	}
	production() {}
	bool operator <(const production& p)const {
		if (belong != p.belong)
			return belong<p.belong;
		else {
			if (follow_list.size() != p.follow_list.size())
				return follow_list.size()<p.follow_list.size();
			for (int i = 0; i<follow_list.size(); i++) {
				if (follow_list[i]<p.follow_list[i])
					return 1;
				else if (follow_list[i]>p.follow_list[i])
					return 0;
			}
			return 0;
		}
	}
};
struct node {
	Symbol symbol;
	int node_num;
	node* father = NULL;
	vector<node*>sub_node;
	node() {}
};
class SyntacticAnalyser {
public:
	//所有的产生式
	map<int, set<production>>production_cmm;
	vector<production>all_production;
	map<int, set<int>>first;
	map<int, set<int>>follow;
	//预测分析表,1~30行(非终结符)1~30列(终结符)
	int predict_table[32][32];
	//语法树根节点
	node *root;
	WrongProcesser *wp;
	string name;
public:
	//使用语法产生式文件初始化
	SyntacticAnalyser(string file, Ui::compiler_finalClass*aui);
	//分析输入
	bool analyse(LexicalAnalyser &la);
	//打印语法树
	void print_tree();
	FILE*file;
private:
	//每个非终结符的first集
	void make_first();
	//每个符号的follow集
	void make_follow();
	//返回一个非终结符的first集
	set<int> get_first(int non_term_num);
	//非终结符是否可以推出为空字
	bool can_be_null(int non_term_num);
	//select集
	void make_select();
	//预测分析表
	void make_predict_table();
	int print_node(node *n, int layer);
	Ui::compiler_finalClass*ui;
};


#endif //COMPILER_SYNTACTICANALYSER_H

