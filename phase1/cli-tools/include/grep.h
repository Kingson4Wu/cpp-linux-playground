#pragma once

#include <filesystem>
#include <iosfwd>
#include <string>
#include <vector>

/**
 * @brief 在文件中搜索匹配的行
 * 
 * @param pattern 搜索模式
 * @param filepath 文件路径
 * @param out 输出流
 * @param show_line_numbers 是否显示行号
 * @return true 搜索成功
 * @return false 搜索失败（如文件不存在）
 */
bool grep_file(const std::string& pattern, const std::filesystem::path& filepath, 
               std::ostream& out, bool show_line_numbers = false);

/**
 * @brief 在文本中搜索匹配的行
 * 
 * @param pattern 搜索模式
 * @param text 输入文本
 * @param out 输出流
 * @param show_line_numbers 是否显示行号
 */
void grep_text(const std::string& pattern, const std::string& text, 
               std::ostream& out, bool show_line_numbers = false);
