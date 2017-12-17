#ifndef UTILS_DEBUG_LOG_HXX
#define UTILS_DEBUG_LOG_HXX


#include "../config/logger.hxx" // ::Config::Utils::Logger::*
#include "../logging/logger.hxx" // ::Utils::Logger::{log, log_Detailed}
#include "../logging/source-location.hxx" // CURRENT_SOURCE_LOCATION
#include "../preproc/stringify.hxx" // STRINGIFY_E


#ifdef WITH_DEBUG_LOG
/**
 * @brief
 */
#define LOG_F_L(format, ...) \
  do \
  { \
    ::Utils::Logger::log_Detailed ( \
      (CURRENT_SOURCE_LOCATION ()), ::Config::Utils::Logger::Detailed_prefix, (format), __VA_ARGS__ \
    ); \
  } \
  while (false)


/**
 * @brief
 */
#define LOG_F_NL(format, ...) \
  do \
  { \
    ::Utils::Logger::log (::Config::Utils::Logger::Default_prefix, (format), __VA_ARGS__); \
  } \
  while (false)


/**
 * @brief
 */
#define LOG_NF_L(message) \
  do \
  { \
    ::Utils::Logger::log_Detailed ( \
      (CURRENT_SOURCE_LOCATION ()), ::Config::Utils::Logger::Detailed_prefix, (message) \
    ); \
  } \
  while (false)


/**
 * @brief
 */
#define LOG_NF_NL(message) \
  do \
  { \
    ::Utils::Logger::log (::Config::Utils::Logger::Default_prefix, (message)); \
  } \
  while (false)


/**
 * @brief
 */
#define LOG_VAL_L(expression) \
  do \
  { \
    ::Utils::Logger::log_Detailed ( \
      (CURRENT_SOURCE_LOCATION ()), ::Config::Utils::Logger::Value_prefix, \
      "`{0:s}' == `{1}'", (STRINGIFY_E (expression)), (expression) \
    ); \
  } \
  while (false)


/**
 * @brief
 */
#define LOG_VAL_NL(expression) \
  do \
  { \
    ::Utils::Logger::log ( \
      ::Config::Utils::Logger::Value_prefix, "`{0:s}' == `{1}'", (STRINGIFY_E (expression)), (expression) \
    ); \
  } \
  while (false)


/**
 * @brief
 */
#define LOG_FUNC() \
  do \
  { \
    ::Utils::Logger::log ( \
      ::Config::Utils::Logger::Func_prefix, "{0:s}", (CURRENT_SOURCE_LOCATION ()).function () \
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

#define LOG_FUNC() do { } while (false)
#endif // WITH_DEBUG_LOG


#endif // UTILS_DEBUG_LOG_HXX
