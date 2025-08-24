#ifndef CRAWLER_H
#define CRAWLER_H

#include "url_parser.h"
#include "http_client.h"
#include "thread_pool.h"
#include <string>
#include <set>
#include <queue>
#include <mutex>
#include <atomic>
#include <functional>
#include <memory>
#include <condition_variable>

namespace crawler {

/**
 * @brief A high-concurrency web crawler
 * 
 * This class implements a web crawler that can fetch web pages concurrently
 * using a thread pool.
 */
class Crawler {
public:
    /**
     * @brief Callback function type for handling crawled pages
     * 
     * @param url The URL of the crawled page
     * @param content The content of the crawled page
     */
    typedef std::function<void(const std::string&, const std::string&)> PageCallback;

    /**
     * @brief Construct a new Crawler object
     * 
     * @param num_threads Number of threads in the thread pool
     * @param max_pages Maximum number of pages to crawl
     */
    Crawler(size_t num_threads = 4, size_t max_pages = 100);

    /**
     * @brief Destroy the Crawler object
     */
    ~Crawler();

    // Delete copy constructor and assignment operator
    Crawler(const Crawler&) = delete;
    Crawler& operator=(const Crawler&) = delete;

    /**
     * @brief Add a URL to the crawl queue
     * 
     * @param url The URL to crawl
     */
    void AddUrl(const std::string& url);

    /**
     * @brief Start crawling
     * 
     * @param callback Function to call for each crawled page
     */
    void Start(const PageCallback& callback);

    /**
     * @brief Stop crawling
     */
    void Stop();

    /**
     * @brief Wait for crawling to complete
     */
    void Wait();

    /**
     * @brief Get the number of crawled pages
     * 
     * @return size_t The number of crawled pages
     */
    size_t GetCrawledCount() const;

private:
    ThreadPool thread_pool_;
    std::queue<std::string> url_queue_;
    std::set<std::string> visited_urls_;
    mutable std::mutex queue_mutex_;
    mutable std::mutex visited_mutex_;
    std::atomic<bool> stop_;
    std::atomic<size_t> crawled_count_;
    size_t max_pages_;
    std::condition_variable crawl_complete_cv_;
    std::mutex crawl_complete_mutex_;
    std::atomic<size_t> active_crawls_;

    /**
     * @brief Process the crawl queue
     * 
     * @param callback Function to call for each crawled page
     */
    void ProcessQueue(const PageCallback& callback);

    /**
     * @brief Crawl a single URL
     * 
     * @param url The URL to crawl
     * @param callback Function to call for the crawled page
     */
    void CrawlUrl(const std::string& url, const PageCallback& callback);
};

} // namespace crawler

#endif // CRAWLER_H