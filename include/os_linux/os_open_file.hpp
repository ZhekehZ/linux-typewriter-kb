#pragma once

#include <optional>

namespace os {

std::optional<int> open_file(char const *path, int mode, int opts);

}// namespace os