#ifndef IITPEXCEPTION_H
#define IITPEXCEPTION_H

#include "Poco/Net/NetException.h"

namespace Innovative {
    POCO_DECLARE_EXCEPTION(Net_API, IITPException, Poco::Net::NetException)
}

#endif // IITPEXCEPTION_H
