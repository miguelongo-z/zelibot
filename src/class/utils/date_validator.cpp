#include "utils/date_validator.hpp"
#include <chrono>
#include <regex>
#include <sstream>

bool DateValidator::check_hour(const std::string &hour) {

  int h, m;
  char sep;

  std::stringstream ss(hour);

  if (!(ss >> h >> sep >> m))
    return false;

  if (sep != ':')
    return false;

  return h >= 0 && h <= 23 && m >= 0 && m <= 59;
}

bool DateValidator::check_date(const std::string &date) {
  int year, month, day;
  char sep1, sep2;

  std::stringstream ss(date);

  if (!(ss >> year >> sep1 >> month >> sep2 >> day))
    return false;

  if (sep1 != '-' || sep2 != '-')
    return false;

  std::chrono::year_month_day ymd{
      std::chrono::year{year}, std::chrono::month{static_cast<unsigned>(month)},
      std::chrono::day{static_cast<unsigned>(day)}};

  return ymd.ok();
}

bool DateValidator::is_valid_format_date(const std::string &date,
                                         const std::string &hour) {

  std::regex date_regex(R"(^\d{4}-\d{2}-\d{2}$)");
  std::regex hour_regex(R"(^\d{2}:\d{2}$)");

  return std::regex_match(date, date_regex) &&
         std::regex_match(hour, hour_regex) && check_date(date) &&
         check_hour(hour);
}

bool DateValidator::is_valid(const std::string &date, const std::string &hour) {
  return is_valid_format_date(date, hour);
}
