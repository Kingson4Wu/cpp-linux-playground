/**
 * @file crawler_test.cpp
 * @brief Unit tests for the crawler components.
 */

#include "crawler.h"
#include "url_parser.h"
#include "http_client.h"
#include "thread_pool.h"
#include <gtest/gtest.h>
#include <thread>
#include <chrono>

namespace crawler {

TEST(CrawlerTest, CreateAndDestroy) {
    // Test creating and destroying a crawler
    Crawler crawler(4, 100);
    
    // If we reach this point, the crawler was created and destroyed successfully
    SUCCEED();
}

TEST(CrawlerTest, AddUrl) {
    // Test adding a URL to the crawler
    Crawler crawler(4, 100);
    crawler.AddUrl("http://www.example.com");
    
    // If we reach this point, the URL was added successfully
    SUCCEED();
}

TEST(CrawlerTest, GetCrawledCount) {
    // Test getting the crawled count
    Crawler crawler(4, 100);
    EXPECT_EQ(crawler.GetCrawledCount(), 0);
}

} // namespace crawler