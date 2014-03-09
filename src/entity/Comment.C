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

  string& replaceWithBr(string& text, const string& key, const string& replacement)
  {
    replace(text, "<br />" + key + "<br />", replacement);
    replace(text, "<br />" + key,            replacement);
    replace(text,            key + "<br />", replacement);
    replace(text,            key,            replacement);

    return text;
  }
}

void Comment::setText(const Wt::WString& src)
{
  textSrc_ = src;

  string html = Wt::WWebWidget::escapeText(src, true).toUTF8();

  // Replace &lt;code&gt;...&lt/code&gt; with <pre>...</pre>
  // including leading/trailing <br />

  replaceWithBr(html, "&lt;code&gt;", "<pre>");
  replaceWithBr(html, "&lt;/code&gt;", "</pre>");

  // Replace empty line with <div class="vspace"></div>
  replace(html, "<br /><br />", "<div class=\"vspace\"></div>");

  textHtml_ = Wt::WString::fromUTF8(html);
}

void Comment::setDeleted()
{
  textHtml_ = Wt::WString::tr("comment-deleted");
}
