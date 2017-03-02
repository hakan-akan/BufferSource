#include "RemoteLogger_Mb.h"

namespace Innovative {

RemoteLogger::RemoteLogger()
{
}

RemoteLogger::RemoteLogger(const std::string &uri) : _uri(uri), _sess(new Innovative::IITPClientSession(_uri.getHost(), _uri.getPort()))
{

}

RemoteLogger::~RemoteLogger()
{

}

void RemoteLogger::setTarget(const std::string &uri)
{
    _uri = uri;
    _sess.reset(new Innovative::IITPClientSession(_uri.getHost(), _uri.getPort()));
}

void RemoteLogger::setFilename(const std::string &file)
{
    _sess->setFile(file);
}

void RemoteLogger::start()
{
   _sess->beginStore();
}

void RemoteLogger::stop()
{
    _sess->endStore();
}

void RemoteLogger::log(const Innovative::Buffer &buf)
{
    _sess->sendBuffer(buf);
}

string RemoteLogger::getWorkingDirectory()
{
    return _sess->getWorkingDirectory();
}

void RemoteLogger::setWorkingDirectory(const string &dir)
{
    _sess->setWorkingDirectory(dir);
}

}


