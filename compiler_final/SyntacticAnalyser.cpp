//
// Created by ������ on 16/10/31.
// Copyright (c) 2016 ___FULLUSERNAME___. All rights reserved.
//

#include "SyntacticAnalyser.h"
#include "LexicalAnalyser.h"
#include <fstream>
#include <algorithm>  
#include <set>
#include <iterator>
#include <sstream>
using namespace std;
int match_signal_and_id(string s) {
	for (int i = 0; i<type_num; i++) {
		if (s == type_name[i])
			return i;
	}
	return -1;
}
int SyntacticAnalyser::print_node(node *n, int layer) {
	fprintf(this->file, "|--%15s", type_name[n->symbol.type].c_str());
	if (n->symbol.type >= SMALLEST_NONTERNMENAL)
		fprintf(this->file, "--");
	vector<node*> vn = n->sub_node;
	int line_num = 1;//����ڵ�չ���ж೤
	for (int i = 0; i<vn.size(); i++) {
		line_num += print_node(vn[i], layer + 1);
		if (i + 1<vn.size()) {
			fprintf(this->file, "\n");
			//��ӡ��ǰ��Ŀո��ʽ
			fprintf(this->file, "         ");//Ϊ��ǰ��'program--'����9���ո�
			for (int j = 0; j <= layer; j++) {
				fprintf(this->file, "|");
				for (int k = 0; k < 19; k++)printf(" ");
			}
		}
	}
	return line_num;
}
bool SyntacticAnalyser::can_be_null(int non_term_num) {
	set<production>& sp = this->production_cmm[non_term_num];
	for (set<production>::iterator it = sp.begin(); it != sp.end(); it++) {
		if (it->follow_list[0] == 0)
			return 1;
	}
	return 0;
}
SyntacticAnalyser::SyntacticAnalyser(string file, Ui::compiler_finalClass*aui) {
	this->wp = new WrongProcesser(aui);
	this->ui = aui;
	fstream fin(file, ios::in);
	string line;
	while (getline(fin, line)) {
		if (line.length() == 0)
			continue;
		stringstream ss(line);
		string left_s, right_s;
		int left_i, right_i;
		ss >> left_s;
		left_i = match_signal_and_id(left_s);
		production p(left_i);
		while (ss >> right_s) {
			right_i = match_signal_and_id(right_s);
			p.follow_list.push_back(right_i);
		}
		if (this->production_cmm.count(left_i))
			this->production_cmm[left_i].insert(p);
		else {
			set<production> sp;
			sp.insert(p);
			this->production_cmm[left_i] = sp;
		}
		this->all_production.push_back(p);
	}
	make_first();
	make_follow();
	make_select();
	make_predict_table();
	fin.close();
	//print test
	//    for(map<int,set<production>>::iterator i = this->production_cmm.begin();i!=this->production_cmm.end();i++){
	//        for(set<production>::iterator j = i->second.begin();j!=i->second.end();j++){
	//            printf("%d\t",j->belong);
	//            for(int k = 0;k<j->follow_list.size();k++)
	//                printf("%d\t",j->follow_list[k]);
	//            printf("\n");
	//        }
	//    }
}
set<int> SyntacticAnalyser::get_first(int non_term_num) {
	set<int> si;
	set<production>&sp = this->production_cmm[non_term_num];
	for (set<production>::iterator it = sp.begin(); it != sp.end(); it++) {
		const vector<int>&vi = it->follow_list;
		if (vi[0]<SMALLEST_NONTERNMENAL)
			si.insert(vi[0]);
		else {
			//����ʽ��ͷ�����ս��,�ݹ����
			//ʵ����Ӧ�ÿ����Ҳ���һ�����ս����Ϊ�յ����
			//�����ķ��в�û���������,����û��д
			set<int>sub_si = get_first(vi[0]);
			set<int>result;
			set_union(si.begin(), si.end(), sub_si.begin(),
				sub_si.end(), inserter(result, result.begin()));
			si = result;
		}
	}
	return si;
}
void SyntacticAnalyser::make_first() {
	for (int i = SMALLEST_NONTERNMENAL; i<LARGEST_NONTERNMENAL; i++)
		this->first[i] = get_first(i);
	//test
	//    for(int i = SMALLEST_NONTERNMENAL;i<LARGEST_NONTERNMENAL;i++) {
	//        printf("%d\t",i);
	//        for(set<int>::iterator it=first[i].begin();it!=first[i].end();it++)
	//            printf("%d,",*it);
	//        printf("\n");
	//    }
}
void SyntacticAnalyser::make_follow() {
	vector<production>&vp = this->all_production;
	//��һ�ֺ�����ż��ϵĲ������,a->bC,��C�Ŀ�ʼ���ż��Ͼ���b�ĺ�����ż���
	for (int i = 0; i<vp.size(); i++) {
		vector<int>&vi = vp[i].follow_list;
		for (int j = 0; j<vi.size() - 1; j++) {
			if (vi[j] == 0)continue;
			if (vi[j + 1] >= SMALLEST_NONTERNMENAL) {
				//���������һ�����ս��
				set<int> si, temp;
				if (this->follow.count(vi[j]))
					temp = follow[vi[j]];
				set_union(temp.begin(), temp.end(),
					this->first[vi[j + 1]].begin(),
					this->first[vi[j + 1]].end(),
					inserter(si, si.begin()));
				this->follow[vi[j]] = si;
			}
			else {
				//���������һ���ս��
				if (this->follow.count(vi[j]))
					this->follow[vi[j]].insert(vi[j + 1]);
				else {
					set<int>si;
					si.insert(vi[j + 1]);
					this->follow[vi[j]] = si;
				}
			}
		}
	}
	//�ڶ���,a->cb,��a�ĺ�����ű�b����;��b���Ƴ�null,��b�ĺ�����ű�c����
	bool have_changed = 1;
	while (have_changed) {
		have_changed = 0;
		for (int i = 0; i < vp.size(); i++) {
			vector<int> &vi = vp[i].follow_list;
			for (int j = 0; j < vi.size(); j++) {
				if (vi[j] == 0)continue;
				set<int>si, temp;
				//ǰһ�����
				if (j == vi.size() - 1) {
					if (this->follow.count(vp[i].belong))
						temp = this->follow[vp[i].belong];
				}//��һ�����
				else if (can_be_null(vi[j + 1])) {
					if (this->follow.count(vi[j + 1]))
						temp = this->follow[vi[j + 1]];
				}
				set_union(temp.begin(), temp.end(),
					this->follow[vi[j]].begin(),
					this->follow[vi[j]].end(),
					inserter(si, si.begin()));
				if (si.size()>this->follow[vi[j]].size()) {
					have_changed = 1;
					this->follow[vi[j]] = si;
				}

			}
		}
	}
	//test
	//    for(map<int,set<int>>::iterator itm = this->follow.begin();itm!=this->follow.end();itm++){
	//        printf("%d\t",itm->first);
	//        for(set<int>::iterator its = itm->second.begin();its!=itm->second.end();its++){
	//            printf("%d ",*its);
	//        }
	//        printf("\n");
	//    }
}
void SyntacticAnalyser::make_select() {
	for (vector<production>::iterator it = this->all_production.begin(); it != this->all_production.end(); it++) {
		//����select������Ƚ�����
		//����Ӧ���ж�A->alpha��alpha�Ƿ����Ϊ����
		//�����ķ��з��ֳ���alpha�����ǿ���,����alpha�������Ƴ�����
		//(���˵�һ������ʽ,���������ֱ�ӵ����������,��Ϊ��ʱ�൱��û������)
		//ͬʱ,���ֲ���ʽ�Ҳ���ͷ�����ܵ�������
		//��������������Ҳ����Ϊ��������Եõ��˼�
		if (it->follow_list[0] == 0)
			it->select = this->follow[it->belong];
		else if (it->follow_list[0]<SMALLEST_NONTERNMENAL)
			it->select.insert(it->follow_list[0]);
		else
			it->select = this->first[it->follow_list[0]];
	}
}
void SyntacticAnalyser::make_predict_table() {
	//��Ԥ��������ÿһ��(����һ�����ս��,32��)
	int base = SMALLEST_NONTERNMENAL;
	memset(this->predict_table, -1, sizeof(int) * 32 * 32);
	for (int i = 0; i<LARGEST_NONTERNMENAL - SMALLEST_NONTERNMENAL; i++) {
		vector<production> &vp = this->all_production;
		//��ÿһ������ʽvp[j]
		for (int j = 0; j<vp.size(); j++) {
			if (i + base == vp[j].belong) {
				for (set<int>::iterator sit = vp[j].select.begin(); sit != vp[j].select.end(); sit++) {
					if (this->predict_table[i][*sit] == -1)
						this->predict_table[i][*sit] = j;
					else
						printf("\nWRONG!at row %d col %d first is %d next is %d\n",
							i, *sit, this->predict_table[i][*sit], j);
				}
			}
		}
	}
	//test
	/*FILE* f = freopen("table", "w", stdout);
	    fprintf(f,"type\t");
	    for(int j = 0;j<32;j++)
	        fprintf(f, "%s\t",type_name[j].c_str());
	    fprintf(f, "\n");
	    for(int i = 0;i<32;i++){
	        fprintf(f, "%s\t",type_name[i+SMALLEST_NONTERNMENAL].c_str());
	        for(int j = 0;j<32;j++)
	            fprintf(f, "%d\t",this->predict_table[i][j]);
	        fprintf(f, "\n");
	    }*/
}
bool SyntacticAnalyser::analyse(LexicalAnalyser &la) {
	if (la.program.size() == 1) {
		wp->write("û�г�������\n");
		return 0;
	}
	node*n = new node;
	n->symbol.type = SMALLEST_NONTERNMENAL;
	bool success = 1;
	stack<int>symbol_stack;//�ܿس���ķ���ջ
	int input_pointer = 0;//ָ�������Ѿ���������λ��
	vector<Symbol>& vs = la.program;
	this->name = la.name;
	symbol_stack.push(SMALLEST_NONTERNMENAL);
	while (!symbol_stack.empty()) {
		int symbol_num = symbol_stack.top();
		symbol_stack.pop();
		//������α�������
		if (input_pointer == vs.size() && symbol_stack.size() != 0) {
			wp->write("����δ�սᣡ\n");
			success = 0;
			break;
		}
		//���ջ����-1,��ô˵��һ�����ս����Ӧ�Ĳ���ʽ�Ѿ�����������
		if (symbol_num == -1) {
			n = n->father;
			if (n->father&&n->father->sub_node.size()>n->node_num + 1) {
				n = n->father->sub_node[n->node_num + 1];
			}
			continue;
		}
		//������ս��
		if (symbol_num<SMALLEST_NONTERNMENAL) {
			if (symbol_num == 0) {
				if (n->father->sub_node.size()>n->node_num + 1)
					n = n->father->sub_node[n->node_num + 1];
				continue;
			}
			if (vs[input_pointer].type == symbol_num) {
				n->symbol = vs[input_pointer];
				if (n->father->sub_node.size()>n->node_num + 1)
					n = n->father->sub_node[n->node_num + 1];
				input_pointer++;
			}
			else {
				wp->process_syn1(vs[input_pointer].lineNum,
					symbol_chinese[symbol_num],
					symbol_chinese[vs[input_pointer].type]);
				//                printf("Syntactic Error in line %d :need a \"%s\" but get a \"%s\"\n",
				//                        vs[input_pointer].lineNum,
				//                        type_name[symbol_num].c_str(),
				//                        type_name[vs[input_pointer].type].c_str());
				success = 0;
				break;
			}
		}//����Ƿ��ս��
		else {
			int row = symbol_num - SMALLEST_NONTERNMENAL;
			int col = vs[input_pointer].type;
			int predict_num = this->predict_table[row][col];
			if (predict_num == -1) {

				//                printf("Syntactic Error in line %d :symbol after %s should not be a \"%s\"\n",
				//                        vs[input_pointer].lineNum,
				//                        type_name[symbol_num].c_str(),
				//                        type_name[vs[input_pointer].type].c_str());
				//                printf("It can be a");
				vector<string> can_be;
				for (int i = 0; i<32; i++) {
					if (predict_table[row][i] != -1)can_be.push_back(symbol_chinese[i]);
					//printf(" %s",type_name[i].c_str());
				}
				//printf("\n");
				wp->process_syn2(vs[input_pointer].lineNum,
					symbol_chinese[symbol_num],
					symbol_chinese[vs[input_pointer].type],
					can_be);
				success = 0;
				break;
			}
			vector<int> vi = this->all_production[predict_num].follow_list;
			//�����ӽڵ�
			for (int i = 0; i<(int)vi.size(); i++) {
				node *sub_n = new node;
				sub_n->father = n;
				sub_n->symbol.type = vi[i];
				sub_n->node_num = i;
				n->sub_node.push_back(sub_n);
			}
			n = n->sub_node[0];
			//ѹ�����ջ
			//����-1,��ʾ��������һ�����ս�����ѳ���
			symbol_stack.push(-1);
			for (int i = (int)vi.size() - 1; i>-1; i--) {
				symbol_stack.push(vi[i]);
			}
		}
	}
	if (symbol_stack.empty() && success) {
		wp->write("�﷨���������ȷ\n");
		this->root = n;
	}
	else
		success = 0;
	return success;
}
void SyntacticAnalyser::print_tree() {
	this->file = freopen((this->name + ".tree").c_str(), "w", stdout);
	fprintf(this->file,"program--");
	for (int i = 0; i<root->sub_node.size(); i++) {
		//node*n=root->sub_node[i];
		print_node(root->sub_node[i], 0);
		fprintf(this->file,"\n");
		//��ӡ��ǰ��Ŀո��ʽ
		fprintf(this->file, "         ");//Ϊ��ǰ��'program--'����9���ո�
	}
	fclose(this->file);
	freopen("CON", "w", stdout);
	this->wp->write("�﷨��������\n�뵽.tree�ļ��в����﷨��\n");
}

