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
    cannot_be_reform=12,
    function_may_cannot_return=13,
    too_many_or_to_less_function_para=14
};
enum Errorno_run{
    no_main_function=1,
    wrong_addr_access_or_the_value_is_not_assign=2,
    devide_zero=3,
};
class WrongProcesser {
public:
    void process(Errorno errorno,node*n);
    void process_run(Errorno_run errorno_run);
};


#endif //COMPILER_WRONGPROCESSER_H
