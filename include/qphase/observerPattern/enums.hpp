#ifndef QPHASE_OBSERVERPATTERN_ENUMS_HPP
#define QPHASE_OBSERVERPATTERN_ENUMS_HPP
namespace QPhase::ObserverPattern
{
/// @class Message "message.hpp" "qphase/observerPattern/message.hpp"
/// @brief Defines the message type in the observer pattern.
/// @copyright Ben Baker (University of Utah) distributed under the MIT license.
/// @date January 14 2021
enum class Message
{
    UPDATE,   /*!< Indicates that the subject as been updated. */
    NO_CHANGE /*!< Indicates that the subject was intended to be updated
                   however there was ultimately no change. */
};
}
#endif
