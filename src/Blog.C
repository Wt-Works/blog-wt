/*
 * Copyright (C) 2009 Emweb bvba, Heverlee, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include <Wt/WApplication>
#include <Wt/WContainerWidget>
//#include <Wt/WServer>
#include <Wt/Dbo/SqlConnectionPool>
#include <Wt/Dbo/FixedSqlConnectionPool>

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
namespace dbo = Wt::Dbo;

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

  static dbo::SqlConnectionPool *createConnectionPool(const std::string& sqliteDb)
  {
    dbo::backend::Sqlite3 *connection = new dbo::backend::Sqlite3(sqliteDb);

    connection->setProperty("show-queries", "true");
    connection->setDateTimeStorage(Wt::Dbo::SqlDateTime,
                                   Wt::Dbo::backend::Sqlite3::PseudoISO8601AsText);

    return new dbo::FixedSqlConnectionPool(connection, 10);
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

    Database *db = BlogApplication::createConnectionPool(server.appRoot() + "blog.db");
    server.addApplication(bind(&create, _1, db), BLOGURL);

    BlogRSSFeed rssFeed(*db, "Wt blog example", "", "It's just an example.");
    server.addResource(&rssFeed, FEEDURL);

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

