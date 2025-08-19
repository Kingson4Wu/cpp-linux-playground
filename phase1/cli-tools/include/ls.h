#pragma once

#include <filesystem>
#include <iosfwd>

void list_directory(const std::filesystem::path& path, std::ostream& out);
