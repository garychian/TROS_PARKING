#pragma once

#include <string>

#include "ad_msg_idl/avp_state/avp_main_state.capnp.h"

namespace senseAD {
namespace avp_utils {

using StateMachine = senseAD::msg::avp_state::AvpMainState::State;
const std::string& GetStateName(StateMachine state);

}  // namespace avp_utils
}  // namespace senseAD