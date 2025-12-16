---
sidebar_position: 4
---

# High-concurrency Crawler Implementation

Build a high-performance web crawler that can efficiently crawl and index web pages from multiple domains. This project combines network programming, concurrency, and data processing techniques.

## Project Structure

The crawler is organized as follows:

```
phase4/crawler/
├── CMakeLists.txt
├── include/
│   └── crawler.h  # Crawler interface and classes
└── src/
    └── crawler.cpp  # Implementation of crawling functionality
```

## Learning Objectives

During this project, you will:

- Design and implement a concurrent web crawling system
- Handle HTTP requests and responses efficiently
- Manage large-scale URL discovery and scheduling
- Implement politeness policies (robots.txt, crawl delays)
- Build URL filtering and duplicate detection systems
- Create efficient HTML parsing and link extraction
- Apply rate limiting and resource management techniques
- Design persistent storage for crawled data

## Core Concepts

### Crawler Architecture

A typical web crawler consists of several components:

1. **URL Frontier**: Queue of URLs to be crawled
2. **Scheduler**: Determines which URL to crawl next
3. **Fetcher**: Downloads web pages via HTTP
4. **Extractor**: Parses HTML to find new links
5. **Storage**: Saves crawled content and metadata
6. **Politeness Policy**: Respects website crawl rules

### Concurrency Model

The crawler should use a producer-consumer pattern:

- Multiple fetcher threads download pages concurrently
- Extractor threads process downloaded pages
- Scheduler manages the URL queue
- Storage component handles data persistence

## Implementation Architecture

### Crawler Interface

```cpp
class Crawler {
public:
    struct CrawlConfig {
        std::vector<std::string> seed_urls;
        int max_depth = 3;
        int max_pages = 1000;
        std::chrono::milliseconds crawl_delay = std::chrono::milliseconds(1000);
        int concurrent_threads = 4;
        std::string robots_policy = "obey";  // obey, ignore, custom
        std::string output_dir = "./crawl_data";
    };
    
    explicit Crawler(const CrawlConfig& config);
    void start();
    void stop();
    void pause();
    void resume();
    
    // Statistics
    CrawlStats get_stats() const;
    
private:
    CrawlConfig config_;
    std::atomic<bool> running_;
    std::atomic<bool> paused_;
    
    // Core components
    URLFrontier url_frontier_;
    std::vector<std::thread> fetcher_threads_;
    ContentProcessor content_processor_;
    std::mutex stats_mutex_;
    CrawlStats stats_;
    
    void fetcher_worker(int thread_id);
    void process_page(const CrawledPage& page);
    bool should_crawl(const std::string& url) const;
};
```

### Supporting Structures

```cpp
struct CrawledPage {
    std::string url;
    std::string content;
    std::vector<std::string> links;
    std::string content_type;
    int status_code;
    std::chrono::system_clock::time_point timestamp;
};

struct CrawlStats {
    size_t pages_crawled;
    size_t pages_failed;
    size_t urls_discovered;
    size_t urls_skipped;
    std::chrono::system_clock::time_point start_time;
};
```

### URL Frontier

```cpp
class URLFrontier {
public:
    void add_url(const std::string& url, int depth = 0);
    std::optional<std::pair<std::string, int>> get_next_url();
    bool is_duplicate(const std::string& url) const;
    size_t size() const;
    
private:
    std::queue<std::pair<std::string, int>> url_queue_;  // URL + depth
    std::unordered_set<std::string> seen_urls_;
    mutable std::mutex mutex_;
};
```

## Implementation Requirements

### Core Features

1. **Concurrent Fetching**: Download multiple pages simultaneously
2. **URL Filtering**: Skip certain URLs based on patterns or rules
3. **Duplicate Detection**: Prevent crawling the same URL multiple times
4. **Politeness**: Respect robots.txt and implement crawl delays
5. **Error Handling**: Deal with network errors, timeouts, and invalid content
6. **Storage**: Save crawled content to persistent storage
7. **Statistics**: Track crawl progress and performance metrics

### HTTP Client Implementation

Your crawler needs to handle:

- HTTP/1.1 requests with appropriate headers
- Redirect following (with cycle detection)
- Content type detection and filtering
- Connection pooling for efficiency
- Timeouts and retry logic

### Sample Implementation Approach

```cpp
class Crawler {
private:
    struct Fetcher {
        static std::optional<CrawledPage> fetch(const std::string& url, 
                                              const CrawlConfig& config);
    };
    
    struct LinkExtractor {
        static std::vector<std::string> extract_links(const std::string& html, 
                                                    const std::string& base_url);
    };
    
    CrawlConfig config_;
    URLFrontier frontier_;
    std::vector<std::thread> workers_;
    std::unordered_set<std::string> processed_urls_;
    
    void worker_thread(int id);
    void save_page(const CrawledPage& page);
    
public:
    void start();
    void stop();
};
```

## Politeness Implementation

### robots.txt Handling

- Download and parse robots.txt for each domain
- Respect crawl delays specified in robots.txt
- Obey allow/disallow rules
- Implement user-agent specific rules

### Rate Limiting

- Implement per-domain request throttling
- Use exponential backoff for failed requests
- Respect HTTP response headers (Retry-After, etc.)

## Performance Considerations

- **Connection Pooling**: Reuse HTTP connections to reduce overhead
- **Bandwidth Throttling**: Limit total bandwidth usage
- **Memory Management**: Efficiently manage large numbers of URLs in memory
- **Storage Efficiency**: Use appropriate formats for storing crawled data
- **Duplicate Detection**: Efficiently detect and prevent duplicate crawling
- **Thread Safety**: Proper synchronization for shared data structures

## Storage Strategies

Consider implementing different storage options:

1. **File System**: Store each page in a separate file
2. **Database**: Use SQLite or similar for structured storage
3. **Key-Value Store**: Use a format suitable for large-scale crawling
4. **Compressed Storage**: Compress content to save space

## Testing Strategy

Test your crawler with:

- Small, controlled website crawls
- Robots.txt compliance verification
- Concurrency and performance under load
- Error handling for various HTTP status codes
- Duplicate detection accuracy
- Memory usage with large seed lists
- Politeness behavior verification

## Usage Example

```cpp
#include "crawler.h"
#include <iostream>

int main() {
    Crawler::CrawlConfig config;
    config.seed_urls = {"https://example.com"};
    config.max_depth = 2;
    config.max_pages = 100;
    config.concurrent_threads = 4;
    config.crawl_delay = std::chrono::milliseconds(1000);
    
    Crawler crawler(config);
    
    std::cout << "Starting crawl..." << std::endl;
    crawler.start();
    
    // Wait for completion or user interruption
    // In a real application, you might want to implement proper signal handling
    
    auto stats = crawler.get_stats();
    std::cout << "Crawl completed:" << std::endl;
    std::cout << "Pages crawled: " << stats.pages_crawled << std::endl;
    std::cout << "Pages failed: " << stats.pages_failed << std::endl;
    std::cout << "URLs discovered: " << stats.urls_discovered << std::endl;
    
    return 0;
}
```

## Advanced Features (Optional)

Consider implementing additional capabilities:

1. **Sitemap Support**: Parse and follow XML sitemaps
2. **Content Analysis**: Extract and index text content for search
3. **Media Crawling**: Download images, documents, and other media
4. **Real-time Processing**: Process pages as they're crawled
5. **Distributed Crawling**: Split crawling across multiple machines
6. **JavaScript Rendering**: Use headless browser for JavaScript-heavy sites

## Building and Testing

```bash
# Build the project
cd build
cmake ..
make -j4

# Test crawling a local test site or controlled external site
./phase4/crawler/web_crawler
```

## Next Steps

After completing the crawler project, review the [Development Standards](../standards/code-style.md) documentation to ensure your code follows the project's guidelines. Then proceed to implement all the projects you've learned about throughout this comprehensive playground!