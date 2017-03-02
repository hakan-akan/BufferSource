#ifndef IITPREQUESTHANDLERFACTORY_H
#define IITPREQUESTHANDLERFACTORY_H

#include "Poco/SharedPtr.h"

namespace Innovative {



class IITPRequestHandler;


class IITPRequestHandlerFactory
{
public:
    typedef Poco::SharedPtr<IITPRequestHandlerFactory> Ptr;

    IITPRequestHandlerFactory();

    virtual ~IITPRequestHandlerFactory();

    virtual IITPRequestHandler* createRequestHandler() = 0;

private:
    IITPRequestHandlerFactory(const IITPRequestHandlerFactory&);
    IITPRequestHandlerFactory& operator = (const IITPRequestHandlerFactory&);
};


}


#endif // IITPREQUESTHANDLERFACTORY_H
