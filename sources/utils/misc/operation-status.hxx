#ifndef UTILS_MISC_OPERATIONSTATUS_HXX
#define UTILS_MISC_OPERATIONSTATUS_HXX


namespace Utils
{
  /**
   * @brief
   */
  enum struct [[nodiscard]] OperationStatus : unsigned
  {
    Indeterminate = 0,
    Success = 1 << 0,
    Failure = 1 << 1,
    Nothing = 1 << 2,
  };
}


#endif  // UTILS_MISC_OPERATIONSTATUS_HXX
