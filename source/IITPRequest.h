#ifndef IITPRequest_INCLUDED
#define IITPRequest_INCLUDED


#include "Poco/Net/Net.h"
#include "Poco/Net/HTTPMessage.h"


namespace Innovative {



class IITPRequest: public Poco::Net::HTTPMessage
{
public:
	IITPRequest();
		
	IITPRequest(const std::string& version);
		
	IITPRequest(const std::string& method, const std::string& uri);

	IITPRequest(const std::string& method, const std::string& uri, const std::string& version);

	virtual ~IITPRequest();

	void setMethod(const std::string& method);

	const std::string& getMethod() const;

	void setURI(const std::string& uri);
		
	const std::string& getURI() const;
		
	void setHost(const std::string& host);
		
	void setHost(const std::string& host, Poco::UInt16 port);
		
	const std::string& getHost() const;

	bool hasCredentials() const;
		
	void getCredentials(std::string& scheme, std::string& authInfo) const;
		
	void setCredentials(const std::string& scheme, const std::string& authInfo);

	void write(std::ostream& ostr) const;

	void read(std::istream& istr);
		
	static const std::string HTTP_GET;
	static const std::string HTTP_HEAD;
	static const std::string HTTP_PUT;
	static const std::string HTTP_POST;
	static const std::string HTTP_OPTIONS;
	static const std::string HTTP_DELETE;
	static const std::string HTTP_TRACE;
	static const std::string HTTP_CONNECT;
	
	static const std::string HOST;
	static const std::string COOKIE;
	static const std::string AUTHORIZATION;

private:
	enum Limits
	{
		MAX_METHOD_LENGTH  = 32,
		MAX_URI_LENGTH     = 4096,
		MAX_VERSION_LENGTH = 8
	};
	
	std::string _method;
	std::string _uri;
	
	IITPRequest(const IITPRequest&);
	IITPRequest& operator = (const IITPRequest&);
};


//
// inlines
//
inline const std::string& IITPRequest::getMethod() const
{
	return _method;
}


inline const std::string& IITPRequest::getURI() const
{
	return _uri;
}


}


#endif
