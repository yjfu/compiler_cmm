//
// Created by 傅永健 on 16/12/1.
// Copyright (c) 2016 ___FULLUSERNAME___. All rights reserved.
//

#ifndef COMPILER_WRONGPROCESSER_H
#define COMPILER_WRONGPROCESSER_H

#include "SyntacticAnalyser.h"

enum Errorno{
    decleared_function=1,
    have_same_para=2,
    decleared_var=3,
    wrong_return_value=4,
    not_decleared_func_or_var=5,
    list_dice_should_not_be_double=6,
    type_is_not_correct=7,
    use_a_list_as_value=8,
    use_a_value_as_list=9,
    use_function_like_a_value_or_list=10,
    wrong_var_type=11,
    cannot_read_or_write_a_list_or_function=12
};
class WrongProcesser {
public:
    void process(int errorno,node*n);
};


#endif //COMPILER_WRONGPROCESSER_H
