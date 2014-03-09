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

#include "0/WServer.h" // a slightly customized version of WServer

using std::cerr;
using std::endl;
using boost::bind;

using Wt::WApplication;
using Wt::WEnvironment;
using Wt::Application;
//using Wt::WServer;
using Ma::WServer;

typedef Wt::Dbo::SqlConnectionPool Database; // just for simplicity
namespace dbo = Wt::Dbo;

static const char *FEEDURL = "/blog/feed/";
static const char *BLOGURL = "/blog";

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

class BlogApplication : public WApplication
{
public:
  BlogApplication(const WEnvironment& env) : WApplication(env)
  {
    root()->addWidget(new BlogView("/", *db_, FEEDURL));
    useStyleSheet("css/blogexample.css");
  }

  // use a static method for any general/global resource allocation/initialization
  static void init(const std::string& sqliteDb)
  {
    BlogSession::configureAuth();

    dbo::backend::Sqlite3 *connection = new dbo::backend::Sqlite3(sqliteDb);

    connection->setProperty("show-queries", "true");
    connection->setDateTimeStorage(dbo::SqlDateTime,
                                   dbo::backend::Sqlite3::PseudoISO8601AsText);

    // keep the db available to the class
    db_ = new dbo::FixedSqlConnectionPool(connection, 10);
  }

  // used by the server to create instances for each user
  static WApplication *create(const WEnvironment& env)
  {
    return new BlogApplication(env);
  }

  static dbo::FixedSqlConnectionPool* db() {return db_;}
  static void free() {delete db_;}

private:
  static dbo::FixedSqlConnectionPool* db_;

};

dbo::FixedSqlConnectionPool* BlogApplication::db_; // storage for private class variable

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

int main(int argc, char **argv)
{
  try {
    WServer server(argc, argv, "./wthttpd");

    BlogApplication::init(server.appRoot() + "blog.db");
    server.addApplication(&BlogApplication::create, BLOGURL);

    BlogRSSFeed rssFeed(*BlogApplication::db(), "Wt blog example", "", "It's just an example.");
    server.addResource(&rssFeed, FEEDURL);

    server.run();

    BlogApplication::free();
  }
  catch (WServer::Exception& e) {
    cerr << e.what() << endl;
  }
  catch (std::exception &e) {
    cerr << "exception: " << e.what() << endl;
  }
}

