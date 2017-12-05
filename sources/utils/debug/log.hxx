#ifndef UTILS_DEBUG_LOG_HXX
#define UTILS_DEBUG_LOG_HXX


#include "../config/logger.hxx" // ::Config::Utils::Logger::*
#include "../logging/logger.hxx" // ::Utils::Logger::printLog_{Short, Detailed}
#include "../logging/source-location.hxx" // CURRENT_SOURCE_LOCATION
#include "../preproc/stringify.hxx" // STRINGIFY_E


#ifdef WITH_DEBUG_LOG
/**
 * @brief
 */
#define LOG_F_L(format, ...) \
  do \
  { \
    ::Utils::Logger::printLog_Detailed ( \
      CURRENT_SOURCE_LOCATION (), \
      ::Config::Utils::Logger::Detailed_log_prefix, \
      (format), __VA_ARGS__ \
    ); \
  } \
  while (false)


/**
 * @brief
 */
#define LOG_F_NL(format, ...) \
  do \
  { \
    ::Utils::Logger::printLog_Short ( \
      ::Config::Utils::Logger::Short_log_prefix, \
      (format), __VA_ARGS__ \
    ); \
  } \
  while (false)


/**
 * @brief
 */
#define LOG_NF_L(message) \
  do \
  { \
    ::Utils::Logger::printLog_Detailed ( \
      CURRENT_SOURCE_LOCATION (), \
      ::Config::Utils::Logger::Detailed_log_prefix, \
      (message) \
    ); \
  } \
  while (false)


/**
 * @brief
 */
#define LOG_NF_NL(message) \
  do \
  { \
    ::Utils::Logger::printLog_Short ( \
      ::Config::Utils::Logger::Short_log_prefix, \
      (message) \
    ); \
  } \
  while (false)


/**
 * @brief
 */
#define LOG_VAL_L(expression) \
  do \
  { \
    ::Utils::Logger::printLog_Detailed ( \
      CURRENT_SOURCE_LOCATION (), \
      ::Config::Utils::Logger::Value_log_prefix, \
      "`{0}' == `{1}'", (STRINGIFY_E (expression)), (expression) \
    ); \
  } \
  while (false)


/**
 * @brief
 */
#define LOG_VAL_NL(expression) \
  do \
  { \
    ::Utils::Logger::printLog_Short ( \
      ::Config::Utils::Logger::Value_log_prefix, \
      "`{0}' == `{1}'", (STRINGIFY_E (expression)), (expression) \
    ); \
  } \
  while (false)
#else // WITH_DEBUG_LOG
#define LOG_F_L(format, ...) do { } while (false)

#define LOG_F_NL(format, ...) do { } while (false)

#define LOG_NF_L(message) do { } while (false)

#define LOG_NF_NL(message) do { } while (false)

#define LOG_VAL_L(expression) do { } while (false)

#define LOG_VAL_NL(expression) do { } while (false)
#endif // WITH_DEBUG_LOG


#endif // UTILS_DEBUG_LOG_HXX
