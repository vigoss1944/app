#pragma once
#include <boost/shared_ptr.hpp>

class AsioClientSession;
struct BussPacket;
class FunCodeProcessor;

class FunCodeProcessorFactory
{
public:
	static boost::shared_ptr<FunCodeProcessorFactory> Instance();
	virtual boost::shared_ptr<FunCodeProcessor> Create(boost::shared_ptr<AsioClientSession> sessionPtr, const BussPacket& packet);	
};