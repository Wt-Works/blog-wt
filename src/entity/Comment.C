/*
 * Copyright (C) 2009 Emweb bvba, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include "Comment.h"
#include "Post.h"
#include "User.h"
#include "Tag.h"
#include "../auth/Token.h"

#include <Wt/Dbo/Impl>
#include <Wt/WWebWidget>

DBO_INSTANTIATE_TEMPLATES(Comment);

using std::string;
using std::cout;
using std::cerr;

namespace {
  string& replace(string& text, const string& key, const string& replacement)
  {
    string::size_type pos = 0;

    while ((pos = text.find(key, pos)) != string::npos) {
      text.replace(pos, key.length(), replacement);
      pos += replacement.length();
    }

    return text;
  }
}

void Comment::setText(const Wt::WString& src)
{
  textSrc_ = src;

  string html = Wt::WWebWidget::escapeText(src, true).toUTF8();

  string::size_type posOpen = 0;

  // Replace &lt;code&gt;...&lt/code&gt; with <pre>...</pre>
  // This is kind of very ad-hoc!

  // <br />&lt;code&gt;<br />   <br />&lt;/code&gt;<br />
  // 12345678901234567890123456789012345678901234567890123456789


  while ((posOpen = html.find("&lt;code&gt;", posOpen)) != string::npos) {
    string::size_type posClose = html.find("&lt;/code&gt;", posOpen);
    if (posClose == string::npos)
      break;
    else {
      // erase the preceeding br tag, correct positions
      if (posOpen > 6 && html.substr(posOpen - 6, 6) == "<br />") {
        html.erase(posOpen - 6, 6);
        posOpen -= 6;
        posClose -= 6;
        cerr << "br posOpen\n";
      }

      // replace
      html.replace(posOpen, 12, "<pre>");
      posClose -= 7;
      cerr << "posOpen\n";

      // erase the following br tag, correct position
      if (html.substr(posOpen + 5, 6) == "<br />") {
        html.erase(posOpen + 5, 6);
        posClose -= 6;
        cerr << "posOpen br\n";
      }

      // ease the preceeding br tag
      if (html.substr(posClose - 6, 6) == "<br />") {
        html.erase(posClose - 6, 6);
        posClose -= 6;
        cerr << "br posClose\n";
      }

      // replace
      html.replace(posClose, 13, "</pre>");
      posClose += 6;
      cerr << "posClose\n";

      // erase the following bt tag
      if (posClose + 6 <= html.length() && html.substr(posClose, 6) == "<br />") {
        html.erase(posClose, 6);
        posClose -= 6;
        cerr << "posClose br\n";
      }

      posOpen = posClose;
    }
  }

  //replace(html, "&lt;code&gt;", "<pre>");
  //replace(html, "&lt;/code&gt;", "</pre >");

  // We would also want to replace <br /><br /> (empty line) with
  // <div class="vspace"></div>
  replace(html, "<br /><br />", "<div class=\"vspace\"></div>");

  textHtml_ = Wt::WString::fromUTF8(html);
}

void Comment::setDeleted()
{
  textHtml_ = Wt::WString::tr("comment-deleted");
}
