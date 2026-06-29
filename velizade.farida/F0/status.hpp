#ifndef STATUS_HPP
#define STATUS_HPP

#include <string>

namespace velizade
{
  enum class Status
  {
    UNREAD,
    READING,
    FINISHED
  };

  std::string statusToString(Status s);
  Status stringToStatus(const std::string& str);
}

#endif
