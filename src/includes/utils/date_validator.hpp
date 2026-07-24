

#ifndef SRC_INCLUDES_UTILS_DATE_VALIDATOR_HPP_
#define SRC_INCLUDES_UTILS_DATE_VALIDATOR_HPP_

#include <string>
class DateValidator {

private:
  static bool check_hour(const std::string &hour);
  static bool check_date(const std::string &date);
  static bool is_valid_format_date(const std::string &date,
                                   const std::string &hour);

public:
  static bool is_valid(const std::string &date, const std::string &hour);
};
#endif // SRC_INCLUDES_UTILS_DATE_VALIDATOR_HPP_
