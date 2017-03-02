#ifndef IITPStreamFactory_INCLUDED
#define IITPStreamFactory_INCLUDED


#include "Poco/Net/Net.h"
#include "Poco/Net/HTTPSession.h"
#include "Poco/URIStreamFactory.h"


namespace Innovative {


class IITPStreamFactory: public Poco::URIStreamFactory
{
public:
    IITPStreamFactory();

    IITPStreamFactory(const std::string& proxyHost, Poco::UInt16 proxyPort = Poco::Net::HTTPSession::HTTP_PORT);

    IITPStreamFactory(const std::string& proxyHost, Poco::UInt16 proxyPort, const std::string& proxyUsername, const std::string& proxyPassword);

    virtual ~IITPStreamFactory();
		
	virtual std::istream* open(const Poco::URI& uri);

	static void registerFactory();

	static void unregisterFactory();

protected:
    void getPathAndType(const Poco::URI &uri, std::string &path, char &type);

private:
	enum
	{
		MAX_REDIRECTS = 10
	};
	
	std::string  _proxyHost;
	Poco::UInt16 _proxyPort;
	std::string  _proxyUsername;
	std::string  _proxyPassword;
};


}


#endif
