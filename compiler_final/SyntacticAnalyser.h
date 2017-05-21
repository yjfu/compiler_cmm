//
// Created by ������ on 16/10/31.
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
	//���еĲ���ʽ
	map<int, set<production>>production_cmm;
	vector<production>all_production;
	map<int, set<int>>first;
	map<int, set<int>>follow;
	//Ԥ�������,1~30��(���ս��)1~30��(�ս��)
	int predict_table[32][32];
	//�﷨�����ڵ�
	node *root;
	WrongProcesser *wp;
	string name;
public:
	//ʹ���﷨����ʽ�ļ���ʼ��
	SyntacticAnalyser(string file, Ui::compiler_finalClass*aui);
	//��������
	bool analyse(LexicalAnalyser &la);
	//��ӡ�﷨��
	void print_tree();
	FILE*file;
private:
	//ÿ�����ս����first��
	void make_first();
	//ÿ�����ŵ�follow��
	void make_follow();
	//����һ�����ս����first��
	set<int> get_first(int non_term_num);
	//���ս���Ƿ�����Ƴ�Ϊ����
	bool can_be_null(int non_term_num);
	//select��
	void make_select();
	//Ԥ�������
	void make_predict_table();
	int print_node(node *n, int layer);
	Ui::compiler_finalClass*ui;
};


#endif //COMPILER_SYNTACTICANALYSER_H

