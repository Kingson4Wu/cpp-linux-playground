#pragma once

#include <filesystem>
#include <iosfwd>

bool list_directory(const std::filesystem::path& path, std::ostream& out);
