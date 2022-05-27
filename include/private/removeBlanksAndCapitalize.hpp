#include <algorithm>
#include <string>
namespace
{
[[nodiscard]] std::string removeBlanksAndCapitalize(const std::string &s) 
{
    auto result = s;
    result.erase(std::remove_if(result.begin(), result.end(), ::isspace),
                 result.end());
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}
}
