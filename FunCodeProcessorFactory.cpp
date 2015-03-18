#include "FunCodeProcessorFactory.h"
#include "FunCodeProcessor.h"
#include "AsioClientSession.h"
#include "PubStruct.h"
#include "FunCode.h"
#include "ResponseCode.h"
#include "MysqlConnectorWapper.h"

#include "AppInfoQueryProcessor.h"
#include "AppListQueryProcessor.h"
#include "AppUploadProcessor.h"
#include "AppListUploadProcessor.h"
#include "UserStatisticsProcessor.h"
#include "AppUserChPwdDBSaver.h"
#include "AppVersionUpdateProcessor.h"
#include "AppAdvertProcessor.h"
#include "ApkCommentProcessor.h"
#include "ApkClassProcessor.h"
#include "AppListUpdateProcessor.h"
#include "AppDownloadGatherProcessor.h"
#include "AppListtypeProcessor.h"
#include <glog/logging.h>

class NullProcessor : public FunCodeProcessor
{
public:
	NullProcessor(boost::shared_ptr<AsioClientSession> sessionPtr, const BussPacket& packet)
		: FunCodeProcessor(sessionPtr, packet)
	{
	}

	void DoBusiness(MysqlConnectorWapper& wapper)
	{
		LOG(ERROR) << "into DoBusiness FunCode error";
		SendError(ResponseErrorFunCode);
	}
};


boost::shared_ptr<FunCodeProcessorFactory> FunCodeProcessorFactory::Instance()
{
	static boost::shared_ptr<FunCodeProcessorFactory> _instance(new FunCodeProcessorFactory);
	return _instance;
}

FunCodeProcessor::pointer FunCodeProcessorFactory::Create(boost::shared_ptr<AsioClientSession> sessionPtr, const BussPacket& papcket)
{
	switch (packet.header.FunCode)
	{
		case FUN_APP_QUERY: return FunCodeProcessor::pointer(new AppInfoQueryProcessor);
		case FUN_APP_LIST_QUERY: return FunCodeProcessor::pointer(new AppListQueryProcessor(sessionPtr, packet) );
        case FUN_APP_UPLOAD: return FunCodeProcessor::pointer(new AppUploadProcessor(sessionPtr, packet) );
        case FUN_APP_LIST_UPLOAD: return FunCodeProcessor::pointer(new AppListUploadProcessor(sessionPtr, packet) );
        case FUN_USER_INFO_STATISTICS: return FunCodeProcessor::pointer(new UserStatisticsProcessor(sessionPtr, packet) );
        case FUN_APP_VERSION_UPDATE_REQUEST: return FunCodeProcessor::pointer(new AppVersionUpdateProcessor(sessionPtr, packet) );
        case FUN_USER_PASSWORD_CHANGE: return FunCodeProcessor::pointer(new AppUserChPwdDBSaver(sessionPtr, packet) );
        case FUN_APP_ADVERT: return FunCodeProcessor::pointer(new AppAdvertProcessor(sessionPtr, packet) );
        case FUN_APP_COMMENT_QUERY: return FunCodeProcessor::pointer(new ApkCommentProcessor(sessionPtr, packet) );
        case FUN_APP_CLASS_QUERY: return FunCodeProcessor::pointer(new ApkClassProcessor(sessionPtr, packet) );
        case FUN_APP_LIST_UPDATE_QUERY: return FunCodeProcessor::pointer(new AppListUpdateProcessor(sessionPtr, packet) );
        case FUN_APP_DOWNLOAD_COUNT_GATHER: return FunCodeProcessor::pointer(new AppDownloadGatherProcessor(sessionPtr, packet) );
        case FUN_APP_LISTTYPE_QUERY: return FunCodeProcessor::pointer(new AppListtypeProcessor(sessionPtr, packet) );
	}
	LOG(ERROR) << "Create funCode error:" << packet.header.funCode;
	return FunCodeProcessor::pointer(new NullProcessor(sessionPtr, packet) );
}









