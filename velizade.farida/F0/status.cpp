#include "status.hpp"

std::string velizade::statusToString(velizade::Status s)
{
  if (s == velizade::Status::UNREAD)
  {
    return "unread";
  }
  else if (s == velizade::Status::READING)
  {
    return "reading";
  }
  else if (s == velizade::Status::FINISHED)
  {
    return "finished";
  }
  return "";
}

velizade::Status velizade::stringToStatus(const std::string& str)
{
  if (str == "unread")
  {
    return velizade::Status::UNREAD;
  }
  if (str == "reading")
  {
    return velizade::Status::READING;
  }
  if (str == "finished")
  {
    return velizade::Status::FINISHED;
  }
  return velizade::Status::UNREAD;
}
