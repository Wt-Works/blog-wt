/*
 * Copyright (C) 2009 Emweb bvba, Heverlee, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include <Wt/WApplication>
#include <Wt/WContainerWidget>
//#include <Wt/WServer>
#include <Wt/Dbo/SqlConnectionPool>

#include "BlogSession.h"
#include "auth/Token.h"
#include "entity/User.h"
#include "BlogView.h"
#include "BlogRSSFeed.h"

#include "0/WServer.h"

using std::cerr;
using std::endl;
using boost::bind;

using Wt::WApplication;
using Wt::WEnvironment;
using Wt::Application;
//using Wt::WServer;
using Ma::WServer;

typedef Wt::Dbo::SqlConnectionPool Database;

static const char *FEEDURL = "/blog/feed/";
static const char *BLOGURL = "/blog";

class BlogApplication : public WApplication
{
public:
  BlogApplication(const WEnvironment& env, Database& db) : WApplication(env)
  {
    root()->addWidget(new BlogView("/", db, FEEDURL));
    useStyleSheet("css/blogexample.css");
  }
};

WApplication *create(const WEnvironment& env, Database *db)
{
  return new BlogApplication(env, *db);
}

int main(int argc, char **argv)
{
  try {
    WServer server(argc, argv, "./wthttpd");

    BlogSession::configureAuth();

    Database *db = BlogSession::createConnectionPool(server.appRoot() + "blog.db");

    BlogRSSFeed rssFeed(*db, "Wt blog example", "", "It's just an example.");
    server.addResource(&rssFeed, FEEDURL);

    server.addApplication(bind(&create, _1, db), BLOGURL);

    cerr << "\n\n -- Warning: Example is deployed at '" << BLOGURL << "'\n\n";

    server.run();

    delete db;
  }
  catch (WServer::Exception& e) {
    cerr << e.what() << endl;
  }
  catch (std::exception &e) {
    cerr << "exception: " << e.what() << endl;
  }
}

