/*
 * Copyright (C) 2009 Emweb bvba, Heverlee, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include <Wt/WApplication>
#include <Wt/WContainerWidget>
//#include <Wt/WServer>
#include <Wt/Dbo/SqlConnectionPool>

#include "model/BlogSession.h"
#include "auth/Token.h"
#include "model/User.h"
#include "view/BlogView.h"
#include "BlogRSSFeed.h"

#include "0/WServer.h"

using std::cerr;

using Wt::WApplication;
using Wt::WEnvironment;
using Wt::Application;
//using Wt::WServer;
using Ma::WServer;

using  Wt::Dbo::SqlConnectionPool;

static const char *FeedUrl = "/blog/feed/";
static const char *BlogUrl = "/blog";

class BlogApplication : public WApplication
{
public:
  BlogApplication(const WEnvironment& env, SqlConnectionPool& blogDb) : WApplication(env)
  {
    root()->addWidget(new BlogView("/", blogDb, FeedUrl));
    useStyleSheet("css/blogexample.css");
  }
};

WApplication *createApplication(const WEnvironment& env, SqlConnectionPool *blogDb)
{
  return new BlogApplication(env, *blogDb);
}

int main(int argc, char **argv)
{
  try {
    WServer server(argc, argv, "./wthttpd");

    BlogSession::configureAuth();

    SqlConnectionPool *blogDb = BlogSession::createConnectionPool(server.appRoot() + "blog.db");

    BlogRSSFeed rssFeed(*blogDb, "Wt blog example", "", "It's just an example.");
    server.addResource(&rssFeed, FeedUrl);

    server.addEntryPoint(Application, boost::bind(&createApplication, _1, blogDb), BlogUrl);

    cerr << "\n\n -- Warning: Example is deployed at '" << BlogUrl << "'\n\n";

    server.run();

    delete blogDb;
  }
  catch (WServer::Exception& e) {
    cerr << e.what() << std::endl;
  }
  catch (std::exception &e) {
    cerr << "exception: " << e.what() << std::endl;
  }
}

