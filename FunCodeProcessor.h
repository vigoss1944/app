#pragma once
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

class AsioClientSession;
class MysqlConnectorWapper;
struct BussPacket;
class FunCodeProcessor : public boost::enable_shared_from_this<FunCodeProcessor>
{
public:
	typedef boost::shared_ptr<FunCodeProcessor> pointer;
	FunCodeProcessor(boost::shared_ptr<AsioClientSession> sessionPtr, const BussPacket& packet, int responseFunCode=0);
	virtual ~FunCodeProcessor();

	void Execute();
protected:
	virtual bool IsBadInParam() { return false; }
	virtual void DoBusiness(MysqlConnectorWapper& wapper) = 0;

	int GetResponseFunCode() { return _responseFunCode; }
	bool Send(BussPacket& packet);
	void SendError(int errorCode);
protected:
	boost::shared_ptr<AsioClientSession> _sessionPtr;
	const BussPacket& _packet;
	int _responseFunCode;
};