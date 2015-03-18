#include "FunCodeProcessor.h"
#include "AsioClientSession.h"
#include "PubStruct.h"
#include "JsonParserInstance.h"
#include "FunCode.h"
#include "ResponseCode.h"
#include "MysqlConnectorWapper.h"

#include <glog/logging.h>


FunCodeProcessor::FunCodeProcessor(boost::shared_ptr<AsioClientSession> sessionPtr, const BussPacket& packet, int responseFunCode)
	: _sessionPtr(sessionPtr), _packet(packet), _responseFunCode(responseFunCode)
{
}

FunCodeProcessor::~FunCodeProcessor()
{
}

void FunCodeProcessor::Execute()
{
	if (IsBadInParam() )
	{
		LOG(ERROR) << "Execute IsBadInParam error";
		return;
	}

	//数据库检查用户
	MysqlConnectorWapper wapper;
	if (!wapper.Get() )
	{
		LOG(ERROR) << "MysqlConnectorWapper wapper.Get error";
		//SendError(ResponseDBConnectError);
		return;
	}
	DoBusiness(wapper);
}

bool FunCodeProcessor::Send(BussPacket& packet)
{
	if (!_sessionPtr.get() )
	{
		LOG(ERROR) << "Send _sessionPtr.get() is null";
		return false;
	}
	Packet buffer;
	if (JsonParserInstance::Get().Serialize(packet, &buffer) )
	{
		std::string sendStr((char*)buffer.data, buffer.len);
		_sessionPtr->write(sendStr);
		free(buffer.data);
		return true;		
	}
	LOG(ERROR) << "Send JsonParser Serialize is error";
	return false;	
}

void FunCodeProcessor::SendError(int errorCode)
{
	BussPacket response;
	response.header = _packet.header;
	int responseFunCode = GetResponseFunCode();
	if (responseFunCode > 0)
		response.header.funCode = responseFunCode;
	response.header.errorCode = errorCode;
	response.message.message = NULL;
	response.message.len = 0;

	Send(response);
}

