/**
 * @file url_parser_test.cpp
 * @brief Unit tests for the URL parser.
 */

#include "url_parser.h"
#include <gtest/gtest.h>

namespace crawler {

TEST(UrlParserTest, ParseHttpUrl) {
    UrlParser parser("http://www.example.com");
    
    EXPECT_TRUE(parser.IsValid());
    EXPECT_EQ(parser.GetProtocol(), "http");
    EXPECT_EQ(parser.GetHost(), "www.example.com");
    EXPECT_EQ(parser.GetPort(), 80);
    EXPECT_EQ(parser.GetPath(), "/");
    EXPECT_TRUE(parser.GetQuery().empty());
    EXPECT_TRUE(parser.GetFragment().empty());
}

TEST(UrlParserTest, ParseHttpsUrl) {
    UrlParser parser("https://www.example.com:443/path/to/resource");
    
    EXPECT_TRUE(parser.IsValid());
    EXPECT_EQ(parser.GetProtocol(), "https");
    EXPECT_EQ(parser.GetHost(), "www.example.com");
    EXPECT_EQ(parser.GetPort(), 443);
    EXPECT_EQ(parser.GetPath(), "/path/to/resource");
    EXPECT_TRUE(parser.GetQuery().empty());
    EXPECT_TRUE(parser.GetFragment().empty());
}

TEST(UrlParserTest, ParseUrlWithQueryAndFragment) {
    UrlParser parser("http://www.example.com:8080/path?param1=value1&param2=value2#section1");
    
    EXPECT_TRUE(parser.IsValid());
    EXPECT_EQ(parser.GetProtocol(), "http");
    EXPECT_EQ(parser.GetHost(), "www.example.com");
    EXPECT_EQ(parser.GetPort(), 8080);
    EXPECT_EQ(parser.GetPath(), "/path");
    EXPECT_EQ(parser.GetQuery(), "param1=value1&param2=value2");
    EXPECT_EQ(parser.GetFragment(), "section1");
}

TEST(UrlParserTest, ParseInvalidUrl) {
    UrlParser parser("invalid url");
    
    // Even invalid URLs should be parsed to some extent
    EXPECT_TRUE(parser.IsValid());
    EXPECT_EQ(parser.GetProtocol(), "http"); // Default protocol
    EXPECT_EQ(parser.GetHost(), "invalid url"); // Everything becomes the host
}

} // namespace crawler