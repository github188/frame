#include "RpcServiceHandler.h"
#include "module_const_def.h"

RpcServiceHandler::RpcServiceHandler()
{}

bool RpcServiceHandler::postUserInfo(const UserInfo& userInfo)
{
    LOG(INFO)<<"postUserInfo被调用,["
            <<userInfo.userId<<","
              <<userInfo.userName<<","
                <<userInfo.text<<","
                  <<userInfo.cityID<<","
                    <<userInfo.language<<"]";

    return true;
}

void RpcServiceHandler::searchUserInfos(UserInfoSearchResult& _return, const std::string& query)
{
    UserInfoList userInfos;
    UserInfo userInfo;
    userInfo.userId = 123456;
    userInfo.userName = "我是服务器";
    userInfo.language = "chian";
    userInfo.cityID = CityID::type::CID_CQ;
    userInfo.text = "阿海珐哈佛卡拉回访老客户分类客户发货";
    userInfos.push_back(userInfo);
    _return.__set_userinfos(userInfos);

    LOG(INFO)<<"searchUserInfos被调用"<<query;

    try
    {
        mysqlpp::ScopedConnection con = jsbn::DBOpInstance::Instance()->GetConnect();
        if(nullptr == *con)
        {
            LOG(ERROR)<<"获取数据库连接失败";
            return;
        }

        mysqlpp::Query query = con->query("select VIDEO_ID,NAME,REMARK,URL,IS_USED from jsbn_video");
        if (mysqlpp::StoreQueryResult res = query.store())
        {
            for (size_t i = 0; i < res.num_rows(); ++i)
            {
                LOG(INFO)<<res[i];
            }
        }
    }
    catch(const mysqlpp::BadQuery& e)
    {
        LOG(ERROR)<<"检索失败:"<<e.what();
    }
    catch (const mysqlpp::Exception& er)
    {
        LOG(ERROR)<<"失败:"<<er.what();
    }

}

void RpcServiceHandler::hello()
{
    LOG(INFO)<<"hello被调用";
}
