#pragma once

#include <string>

namespace dtl {
class Target {
   private:
    std::string target_;

   public:
    Target() = default;
    Target(const std::string& target) { target_ = target; }
    operator std::string() { return target_; }
    operator const char*() { return target_.c_str(); }
    auto operator<=>(const Target& other) const {
        return target_.compare(other.target_);
    }
};
}  // namespace dtl
