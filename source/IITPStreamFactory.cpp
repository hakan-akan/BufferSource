
#include "IITPStreamFactory.h"
#include "IITPClientSession.h"
#include "Poco/Net/HTTPIOStream.h"
#include "IITPRequest.h"
#include "IITPResponse.h"
#include "Poco/Net/NetException.h"
#include "Poco/URI.h"
#include "Poco/URIStreamOpener.h"
#include "Poco/UnbufferedStreamBuf.h"
#include "Poco/NullStream.h"
#include "Poco/StreamCopier.h"
#include "Poco/Path.h"

using Poco::URIStreamFactory;
using Poco::URIStreamOpener;
using Poco::UnbufferedStreamBuf;


namespace Innovative {


class IIBufferStreamBuf: public UnbufferedStreamBuf
{
public:
    IIBufferStreamBuf(std::istream& istr):
        _istr(istr)
    {
    }

    ~IIBufferStreamBuf()
    {
    }

private:
    int readFromDevice()
    {
        return _istr.get();
    }

    std::istream& _istr;
};


class IITPIOS: public virtual std::ios
{
public:
    IITPIOS(std::istream& istr):
        _buf(istr)
    {
        poco_ios_init(&_buf);
    }

    ~IITPIOS()
    {
    }

    IIBufferStreamBuf* rdbuf()
    {
        return &_buf;
    }

protected:
    IIBufferStreamBuf _buf;
};


class IIBufferStream: public IITPIOS, public std::istream
{
public:
    IIBufferStream(std::istream& istr, IITPClientSession* pSession):
        IITPIOS(istr),
        std::istream(&_buf),
        _pSession(pSession)
    {
    }

    ~IIBufferStream()
    {
        delete _pSession;
    }

private:
    IITPClientSession* _pSession;
};

IITPStreamFactory::IITPStreamFactory():
    _proxyPort(Poco::Net::HTTPSession::HTTP_PORT)
{
}


IITPStreamFactory::IITPStreamFactory(const std::string& proxyHost, Poco::UInt16 proxyPort):
	_proxyHost(proxyHost),
	_proxyPort(proxyPort)
{
}


IITPStreamFactory::IITPStreamFactory(const std::string& proxyHost, Poco::UInt16 proxyPort, const std::string& proxyUsername, const std::string& proxyPassword):
	_proxyHost(proxyHost),
	_proxyPort(proxyPort),
	_proxyUsername(proxyUsername),
	_proxyPassword(proxyPassword)
{
}


IITPStreamFactory::~IITPStreamFactory()
{
}

void IITPStreamFactory::getPathAndType(const Poco::URI& uri, std::string& path, char& type)
{
    path = uri.getPath();
    type = 'i';
    std::string::size_type pos = path.rfind(';');
    if (pos != std::string::npos)
    {
        if (path.length() == pos + 7 && path.compare(pos + 1, 5, "type=") == 0)
        {
            type = path[pos + 6];
            path.resize(pos);
        }
    }
}

std::istream* IITPStreamFactory::open(const Poco::URI& uri)
{
    poco_assert (uri.getScheme() == "iitp");

    IITPClientSession* pSession = new IITPClientSession(uri.getHost(), uri.getPort());
    try
    {
        //std::string username;
        //::string password;
        // getUserInfo(uri, username, password);

        std::string path;
        char        type;
        getPathAndType(uri, path, type);

      //  pSession->login(username, password);
       // if (type == 'a')
        //    pSession->setFileType(FTPClientSession::TYPE_TEXT);

        Poco::Path p(path, Poco::Path::PATH_UNIX);
        p.makeFile();
        for (int i = 0; i < p.depth(); ++i)
            pSession->setWorkingDirectory(p[i]);
        std::string file(p.getFileName());
        std::istream& istr = pSession->beginPlay();

        return new IIBufferStream(istr, pSession);
    }
    catch (...)
    {
        delete pSession;
        throw;
    }
}


void IITPStreamFactory::registerFactory()
{
    URIStreamOpener::defaultOpener().registerStreamFactory("iitp", new IITPStreamFactory);
}


}
