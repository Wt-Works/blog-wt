// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2009 Emweb bvba, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#ifndef MA_WSERVER_H_
#define MA_WSERVER_H_

#include <Wt/WServer>

namespace Ma {

class WServer : public Wt::WServer
{

public:
    WServer(int argc, char *argv[], const std::string& serverConfigurationFile = std::string())
        : Wt::WServer()
    {
        setServerConfiguration(argc, argv, serverConfigurationFile);
    }

    WServer(const std::string &wtApplicationPath=std::string(), const std::string &wtConfigurationFile=std::string())
        : Wt::WServer(wtApplicationPath, wtConfigurationFile)
    {

    }

};

}


#endif // MA_WSERVER_H_
