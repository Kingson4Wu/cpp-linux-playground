/**
 * @file http_client_test.cpp
 * @brief Unit tests for the HTTP client.
 */

#include "http_client.h"
#include <gtest/gtest.h>

namespace crawler {

TEST(HttpClientTest, CreateAndDestroy) {
    HttpClient client(30);
    // If we reach this point, the client was created and destroyed successfully
    SUCCEED();
}

} // namespace crawler