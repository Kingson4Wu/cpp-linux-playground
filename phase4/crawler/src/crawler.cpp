#include "crawler.h"
#include <iostream>
#include <algorithm>
#include <cctype>

namespace crawler {

Crawler::Crawler(size_t num_threads, size_t max_pages) 
    : thread_pool_(num_threads), stop_(false), crawled_count_(0), max_pages_(max_pages), active_crawls_(0) {}

Crawler::~Crawler() {
    Stop();
}

void Crawler::AddUrl(const std::string& url) {
    std::lock_guard<std::mutex> lock(queue_mutex_);
    url_queue_.push(url);
}

void Crawler::Start(const PageCallback& callback) {
    stop_ = false;
    ProcessQueue(callback);
}

void Crawler::Stop() {
    stop_ = true;
    // Wake up any waiting threads
    crawl_complete_cv_.notify_all();
}

void Crawler::Wait() {
    std::unique_lock<std::mutex> lock(crawl_complete_mutex_);
    crawl_complete_cv_.wait(lock, [this] { 
        std::lock_guard<std::mutex> queue_lock(queue_mutex_);
        return stop_ || (url_queue_.empty() && active_crawls_.load() == 0); 
    });
}

size_t Crawler::GetCrawledCount() const {
    return crawled_count_.load();
}

void Crawler::ProcessQueue(const PageCallback& callback) {
    // Process the crawl queue
    while (!stop_ && crawled_count_.load() < max_pages_) {
        // Get URL from queue
        std::string url;
        {
            std::lock_guard<std::mutex> lock(queue_mutex_);
            if (url_queue_.empty()) {
                break;
            }
            url = url_queue_.front();
            url_queue_.pop();
        }
        
        // Check if URL has been visited
        {
            std::lock_guard<std::mutex> lock(visited_mutex_);
            if (visited_urls_.find(url) != visited_urls_.end()) {
                continue;
            }
            visited_urls_.insert(url);
        }
        
        // Crawl URL
        active_crawls_.fetch_add(1);
        CrawlUrl(url, callback);
    }
    
    // Wait for all active crawls to complete
    std::unique_lock<std::mutex> lock(crawl_complete_mutex_);
    crawl_complete_cv_.wait(lock, [this] { 
        return stop_ || active_crawls_.load() == 0; 
    });
}

void Crawler::CrawlUrl(const std::string& url, const PageCallback& callback) {
    // Enqueue the crawling task to the thread pool
    thread_pool_.Enqueue([this, url, callback]() {
        if (this->stop_ || this->crawled_count_.load() >= this->max_pages_) {
            this->active_crawls_.fetch_sub(1);
            this->crawl_complete_cv_.notify_all();
            return;
        }
        
        // Create an HTTP client for this request
        HttpClient http_client(30);
        auto response = http_client.Get(url);
        if (response && response->status_code == 200) {
            // Call the callback with the crawled page
            callback(url, response->body);
            
            // Increment crawled count
            this->crawled_count_.fetch_add(1);
        } else {
            std::cerr << "Failed to crawl URL: " << url << std::endl;
            if (response) {
                std::cerr << "Status code: " << response->status_code << std::endl;
            }
        }
        
        // Decrement active crawls count and notify if needed
        if (this->active_crawls_.fetch_sub(1) == 1) {
            this->crawl_complete_cv_.notify_all();
        }
    });
}

} // namespace crawler