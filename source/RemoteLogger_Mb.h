#ifndef REMOTELOGGER_MB_H
#define REMOTELOGGER_MB_H

#include <ostream>
#include <string>
#include <memory>

#include "Poco/URI.h"
#include "Poco/AutoPtr.h"

#include "Buffer_Mb.h"
#include "IITPClientSession.h"

namespace Innovative {


class RemoteLogger
{
public:
    RemoteLogger();
    RemoteLogger(const std::string &uri);

    ~RemoteLogger();

    void setTarget(const std::string &uri);
    void setFilename(const std::string &file);

    void start();
    void stop();

    void log(const Innovative::Buffer &buf);

    std::string getWorkingDirectory();
    void setWorkingDirectory(const std::string &dir);


private:
    RemoteLogger(const RemoteLogger &);
    RemoteLogger &operator=(const RemoteLogger &);

    using psess_t = std::unique_ptr<Innovative::IITPClientSession>;

    Poco::URI _uri;
    psess_t   _sess;
};

}



#endif // REMOTELOGGER_MB_H
