///************************************************************
/// @Copyright (C), 2015-2030, jsbn  Information Technologies Co., Ltd.
/// @URL
/// @file           proc_ProcRelay.cpp
/// @brief          处理转发
/// @attention
/// @Author         chenjianjun
/// @Version        0.1
/// @Date           2015年10月27日
/// @Description
/// @History
///************************************************************
#include "proc_relay.h"
#include "../../../communicate/service/bss/bss_client_manager.h"
#include "../../../communicate/service/cms/cms_client_manager.h"

using namespace jsbn;
using namespace jsbn::protoc;

ProcRelay::ProcRelay()
{}

ProcRelay::~ProcRelay()
{}

void ProcRelay::Process(const sProtocolData_ptr& pMsg)
{
    TDataRelay* pData = (TDataRelay*)pMsg.get;
    sSendDataPage_ptr pSend = MallocStructFactory::Instance().get_send_page();
    pSend->sock_handle = pMsg->sock_handle;
    pSend->Copy(pData->msg.c_str(), pData->msg.length());

    switch (pData->dst_srv_type)
    {
        case jsbn::protoc::ServiceTpye::ST_BSS:
        {
            BssClientManager::Instance().SendData(pData->city_id, pSend);
            break;
        }
        case jsbn::protoc::ServiceTpye::ST_CMS:
        {
            CmsClientManager::Instance().SendData(pSend);
            break;
        }
        default:
        {
            LOG(ERROR)<<"无效的转发服务器类型";
            break;
        }
    }
}