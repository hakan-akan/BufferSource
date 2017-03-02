
#include "IITPRequest.h"
#include "Poco/Net/NetException.h"
#include "Poco/Net/NameValueCollection.h"
#include "Poco/Ascii.h"
#include "Poco/String.h"
#include "Poco/NumberFormatter.h"

namespace Innovative {


const std::string IITPRequest::HTTP_GET      = "GET";
const std::string IITPRequest::HTTP_HEAD     = "HEAD";
const std::string IITPRequest::HTTP_PUT      = "PUT";
const std::string IITPRequest::HTTP_POST     = "POST";
const std::string IITPRequest::HTTP_OPTIONS  = "OPTIONS";
const std::string IITPRequest::HTTP_DELETE   = "DELETE";
const std::string IITPRequest::HTTP_TRACE    = "TRACE";
const std::string IITPRequest::HTTP_CONNECT  = "CONNECT";
const std::string IITPRequest::HOST          = "Host";
const std::string IITPRequest::COOKIE        = "Cookie";
const std::string IITPRequest::AUTHORIZATION = "Authorization";


IITPRequest::IITPRequest():
	_method(HTTP_GET),
	_uri("/")
{
}

	
IITPRequest::IITPRequest(const std::string& version):
	HTTPMessage(version),
	_method(HTTP_GET),
	_uri("/")
{
}

	
IITPRequest::IITPRequest(const std::string& method, const std::string& uri):
	_method(method),
	_uri(uri)
{
}


IITPRequest::IITPRequest(const std::string& method, const std::string& uri, const std::string& version):
	HTTPMessage(version),
	_method(method),
	_uri(uri)
{
}


IITPRequest::~IITPRequest()
{
}


void IITPRequest::setMethod(const std::string& method)
{
	_method = method;
}


void IITPRequest::setURI(const std::string& uri)
{
	_uri = uri;
}


void IITPRequest::setHost(const std::string& host)
{
	set(HOST, host);
}

	
void IITPRequest::setHost(const std::string& host, Poco::UInt16 port)
{
	std::string value(host);
	if (port != 80 && port != 443)
	{
		value.append(":");
        Poco::NumberFormatter::append(value, port);
	}
	setHost(value);
}

	
const std::string& IITPRequest::getHost() const
{
	return get(HOST);
}


bool IITPRequest::hasCredentials() const
{
	return has(AUTHORIZATION);
}

	
void IITPRequest::getCredentials(std::string& scheme, std::string& authInfo) const
{
	scheme.clear();
	authInfo.clear();
	if (has(AUTHORIZATION))
	{
		const std::string& auth = get(AUTHORIZATION);
		std::string::const_iterator it  = auth.begin();
		std::string::const_iterator end = auth.end();
		while (it != end && Poco::Ascii::isSpace(*it)) ++it;
		while (it != end && !Poco::Ascii::isSpace(*it)) scheme += *it++;
		while (it != end && Poco::Ascii::isSpace(*it)) ++it;
		while (it != end) authInfo += *it++;
	}
    else throw Poco::Net::NotAuthenticatedException();
}

	
void IITPRequest::setCredentials(const std::string& scheme, const std::string& authInfo)
{
	std::string auth(scheme);
	auth.append(" ");
	auth.append(authInfo);
	set(AUTHORIZATION, auth);
}


void IITPRequest::write(std::ostream& ostr) const
{
	ostr << _method << " " << _uri << " " << getVersion() << "\r\n";
	HTTPMessage::write(ostr);
	ostr << "\r\n";
}


void IITPRequest::read(std::istream& istr)
{
	static const int eof = std::char_traits<char>::eof();

	std::string method;
	std::string uri;
	std::string version;
	method.reserve(16);
	uri.reserve(64);
	version.reserve(16);
	int ch = istr.get();
    if (ch == eof) throw Poco::Net::NoMessageException();
	while (Poco::Ascii::isSpace(ch)) ch = istr.get();
    if (ch == eof) throw Poco::Net::MessageException("No HTTP request header");
	while (!Poco::Ascii::isSpace(ch) && ch != eof && method.length() < MAX_METHOD_LENGTH) { method += (char) ch; ch = istr.get(); }
    if (!Poco::Ascii::isSpace(ch)) throw Poco::Net::MessageException("HTTP request method invalid or too long");
	while (Poco::Ascii::isSpace(ch)) ch = istr.get();
	while (!Poco::Ascii::isSpace(ch) && ch != eof && uri.length() < MAX_URI_LENGTH) { uri += (char) ch; ch = istr.get(); }
    if (!Poco::Ascii::isSpace(ch)) throw Poco::Net::MessageException("HTTP request URI invalid or too long");
	while (Poco::Ascii::isSpace(ch)) ch = istr.get();
	while (!Poco::Ascii::isSpace(ch) && ch != eof && version.length() < MAX_VERSION_LENGTH) { version += (char) ch; ch = istr.get(); }
    if (!Poco::Ascii::isSpace(ch)) throw Poco::Net::MessageException("Invalid HTTP version string");
	while (ch != '\n' && ch != eof) { ch = istr.get(); }
	HTTPMessage::read(istr);
	ch = istr.get();
	while (ch != '\n' && ch != eof) { ch = istr.get(); }
	setMethod(method);
	setURI(uri);
	setVersion(version);
}


}
