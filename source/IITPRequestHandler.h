#ifndef IITPREQUESTHANDLER_H
#define IITPREQUESTHANDLER_H

namespace Innovative {

class IITPRequestHandler
{
public:
    IITPRequestHandler();

    virtual ~IITPRequestHandler();

    virtual void handleRequest() = 0;

private:
    IITPRequestHandler(const IITPRequestHandler&);
    IITPRequestHandler& operator = (const IITPRequestHandler&);
};



}

#endif // IITPREQUESTHANDLER_H
