
#ifndef IITPClientSession_INCLUDED
#define IITPClientSession_INCLUDED

#include <Buffer_Mb.h>

#include "Poco/Net/Net.h"
#include "Poco/Net/DialogSocket.h"
#include "Poco/Net/SocketStream.h"
#include "Poco/Timespan.h"
#include <istream>
#include <ostream>


namespace Innovative {


using Poco::Net::StreamSocket;
using Poco::Net::SocketAddress;
using Poco::Net::DialogSocket;
using Poco::Net::SocketStream;

class IITPClientSession
{
public:
	enum
	{
        IITP_PORT = 9980
	};
	
    explicit IITPClientSession(const StreamSocket& socket);

    IITPClientSession(const std::string& host, Poco::UInt16 port = IITP_PORT);
		
    virtual ~IITPClientSession();
	
	void setTimeout(const Poco::Timespan& timeout);
		
	Poco::Timespan getTimeout() const;

	void login(const std::string& username, const std::string& password);

	void close();

	void setWorkingDirectory(const std::string& path);
	std::string getWorkingDirectory();
    void setFile(const std::string& name);

	void cdup();
	void rename(const std::string& oldName, const std::string& newName);
	void remove(const std::string& path);
    void mkdir(const std::string& path);
    void rmdir(const std::string& path);

    std::istream& beginPlay();
    void endPlay();
		
    std::ostream& beginStore();
    void endStore();

	void abort();
    int  sendCommand(const std::string& command, std::string& response);
    int  sendCommand(const std::string& command, const std::string& arg, std::string& response);
    int  sendCommand(const std::string &command, const std::string &arg1, const std::string &arg2, std::string &response);
    int  sendCommand(const std::string &command, const vector<pair<std::string, std::string> > &args, std::string &response);

    void sendBuffer(const Innovative::Buffer &buf);

protected:
	enum StatusClass
	{
		FTP_POSITIVE_PRELIMINARY  = 1,
		FTP_POSITIVE_COMPLETION   = 2,
		FTP_POSITIVE_INTERMEDIATE = 3,
		FTP_TRANSIENT_NEGATIVE    = 4,
		FTP_PERMANENT_NEGATIVE    = 5
	};
	enum
	{
		DEFAULT_TIMEOUT = 30000000 // 30 seconds default timeout for socket operations	
	};

    static bool isPositiveCompletion(int status);
    std::string extractPath(const std::string& response);
    StreamSocket establishDataConnection(const std::string& command);
	StreamSocket activeDataConnection(const std::string& command, const std::string& arg);
    StreamSocket passiveDataConnection(const std::string& command);

    void sendPortCommand(const SocketAddress& addr);
	SocketAddress sendPassiveCommand();
	void sendPORT(const SocketAddress& addr);
	void sendPASV(SocketAddress& addr);
	void parseAddress(const std::string& str, SocketAddress& addr);
	void endTransfer();
	
private:
    IITPClientSession();
    IITPClientSession(const IITPClientSession&);
    IITPClientSession& operator = (const IITPClientSession&);
		
	DialogSocket   _controlSocket;
	SocketStream*  _pDataStream;
	bool           _isOpen;
    Poco::Timespan _timeout;
    std::string paramPack(const pair<std::string, std::string> &arg);
};


//
// inlines
//
//inline bool IITPClientSession::isPositivePreliminary(int status)
//{
//	return status/100 == FTP_POSITIVE_PRELIMINARY;
//}


inline bool IITPClientSession::isPositiveCompletion(int status)
{
    return status/100 == FTP_POSITIVE_COMPLETION;
}


//inline bool IITPClientSession::isPositiveIntermediate(int status)
//{
//	return status/100 == FTP_POSITIVE_INTERMEDIATE;
//}


//inline bool IITPClientSession::isTransientNegative(int status)
//{
//	return status/100 == FTP_TRANSIENT_NEGATIVE;
//}


//inline bool IITPClientSession::isPermanentNegative(int status)
//{
//	return status/100 == FTP_PERMANENT_NEGATIVE;
//}


}


#endif
