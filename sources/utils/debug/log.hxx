#ifndef UTILS_DEBUG_LOG_HXX
#define UTILS_DEBUG_LOG_HXX


#include "../logging/logger.hxx" // ::Utils::Logger::printDetailedLog, ::Config::Utils::Logger::*
#include "../logging/source-location.hxx" // ::Utils::SourceLocation
#include "../preproc/stringify.hxx" // STRINGIFY_E


#ifdef WITH_DEBUG_LOG
  /**
   * @brief
   */
  #define LOG_F_V(format, ...) \
    do \
    { \
      ::Utils::Logger::printDetailedLog ( \
        ::Utils::SourceLocation (__PRETTY_FUNCTION__, __FILE__, __LINE__), \
        ::Config::Utils::Logger::detailed_log_prefix, \
        (format), __VA_ARGS__ \
      ); \
    } \
    while (false)


  /**
   * @brief
   */
  #define LOG_F_NV(format, ...) \
    do \
    { \
      ::Utils::Logger::printShortLog ( \
        ::Config::Utils::Logger::short_log_prefix, \
        (format), __VA_ARGS__ \
      ); \
    } \
    while (false)


  /**
   * @brief
   */
  #define LOG_NF_V(message) \
    do \
    { \
      ::Utils::Logger::printDetailedLog ( \
        ::Utils::SourceLocation (__PRETTY_FUNCTION__, __FILE__, __LINE__), \
        ::Config::Utils::Logger::detailed_log_prefix, \
        (message) \
      ); \
    } \
    while (false)


  /**
   * @brief
   */
  #define LOG_NF_NV(message) \
    do \
    { \
      ::Utils::Logger::printShortLog ( \
        ::Config::Utils::Logger::short_log_prefix, \
        (message) \
      ); \
    } \
    while (false)


  /**
   * @brief
   */
  #define LOG_EXPR(expr) \
    do \
    { \
      ::Utils::Logger::printShortLog ( \
        ::Config::Utils::Logger::expression_log_prefix, \
        "`{0}' == `{1}'", (STRINGIFY_E (expr)), (expr) \
      ); \
    } \
    while (false)
#else // WITH_DEBUG_LOG
  #define LOG_F_V(format, ...) do { } while (false)

  #define LOG_F_NV(format, ...) do { } while (false)

  #define LOG_NF_V(message) do { } while (false)

  #define LOG_NF_NV(message) do { } while (false)

  #define LOG_EXPR(var) do { } while (false)
#endif // WITH_DEBUG_LOG


#endif // UTILS_DEBUG_LOG_HXX
