///************************************************************
/// @Copyright (C), 1993-2022, hzcw  Information Technologies Co., Ltd.
/// @URL
/// @file           module_data_center.h
/// @brief          数据存取中心
/// @attention
/// @Author         chenjianjun
/// @Version        0.1
/// @Date           2015年09月16日
/// @Description
/// @History
///************************************************************
#include "module_data_center.h"

ModuleDataCenter* ModuleDataCenter::Instance() {
    static ModuleDataCenter mInstance;
    return &mInstance;
}

ModuleDataCenter::ModuleDataCenter()
    :_recv_bc_cond_variable(jsbn::ConditionVariable::Create())/*,
      _recv_sc_cond_variable(jsbn::ConditionVariable::Create())*/
{}

ModuleDataCenter::~ModuleDataCenter()
{
    _recv_bc_data_lists.clear();
    delete _recv_bc_cond_variable;

//    _recv_sc_data_lists.clear();
//    delete _recv_sc_cond_variable;
}

int ModuleDataCenter::PutBCProtocolData(sBCProtocolData_ptr& pData)
{
    jsbn::CriticalSectionScoped css(&_recv_bc_critical_section);

    // 判断数据积压是否达到上限
    if (_recv_bc_data_lists.size() > 10000)
    {
        LOG(INFO)<<"接收数据队列发生积压，做丢弃处理.";
        return FUNC_FAILED;
    }

    _recv_bc_data_lists.push_back(pData);

    // 通知数据请求线程有数据到达
    _recv_bc_cond_variable->Wake();

    return FUNC_SUCCESS;
}

sBCProtocolData_ptr ModuleDataCenter::GetBCProtocolData(unsigned long max_time_inMS)
{
    jsbn::CriticalSectionScoped css(&_recv_bc_critical_section);

    // 如果没有数据就等待数据到达
    if (_recv_bc_data_lists.size() == 0)
    {
        _recv_bc_cond_variable->SleepCS(_recv_bc_critical_section, max_time_inMS);
    }

    // 延迟一秒是否有数据到达，有的话取出返回，没有的话返回一个空智能指针
    if (_recv_bc_data_lists.size() == 0)
    {
        return nullptr;
    }

    sBCProtocolData_ptr pData = _recv_bc_data_lists.front();
    _recv_bc_data_lists.pop_front();

    return pData;
}

//int ModuleDataCenter::PutSCProtocolData(sSCProtocolData_ptr& pData)
//{
//    jsbn::CriticalSectionScoped css(&_recv_sc_critical_section);

//    // 判断数据积压是否达到上限
//    if (_recv_sc_data_lists.size() > 10000)
//    {
//        LOG(INFO)<<"接收数据队列发生积压，做丢弃处理.";
//        return FUNC_FAILED;
//    }

//    _recv_sc_data_lists.push_back(pData);

//    // 通知数据请求线程有数据到达
//    _recv_sc_cond_variable->Wake();

//    return FUNC_SUCCESS;
//}

//sSCProtocolData_ptr ModuleDataCenter::GetSCProtocolData(unsigned long max_time_inMS)
//{
//    jsbn::CriticalSectionScoped css(&_recv_sc_critical_section);

//    // 如果没有数据就等待数据到达
//    if (_recv_sc_data_lists.size() == 0)
//    {
//        _recv_sc_cond_variable->SleepCS(_recv_sc_critical_section, max_time_inMS);
//    }

//    // 延迟一秒是否有数据到达，有的话取出返回，没有的话返回一个空智能指针
//    if (_recv_sc_data_lists.size() == 0)
//    {
//        return nullptr;
//    }

//    sSCProtocolData_ptr pData = _recv_sc_data_lists.front();
//    _recv_sc_data_lists.pop_front();

//    return pData;
//}