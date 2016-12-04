//
//  main.cpp
//  compiler
//
//  Created by ∏µ”¿Ω° on 16/10/9.
//  Copyright (c) 2016 ∏µ”¿Ω°. All rights reserved.
//

#include <iostream>
#include "LexicalAnalyser.h"
#include "SyntacticAnalyser.h"
#include "SemanticAnalyser.h"
int main(int argc, const char * argv[]) {
	// insert code here...
	SyntacticAnalyser sa("cmm.protocol");
    LexicalAnalyser la("writepro.cmm");
    if(la.success&&sa.analyse(la)) {
        sa.print_tree();
        //SemanticAnalyser analyser(sa);
        //getchar();
    }


	//“‘œ¬Œ™¥ ∑®∑÷Œˆ∆˜≤‚ ‘
//	string file_name;
//	do {
//		cout << "input the file name here or input 'quit' to leave:\n";
//		cin >> file_name;
//		LexicalAnalyser la(file_name);
//		la.print();
//	} while (file_name != "quit");
//	return 0;
}