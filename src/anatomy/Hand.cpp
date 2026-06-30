//
// Created by Nagareddy on 30/06/26.
//

#include <thread>
#include <vector>
#include <iostream>
#include <chrono>

#include "anatomy/Hand.h"
#include "articulation/Joint.h"

using namespace articulation;
using namespace articulation::movement;

namespace anatomy::hand {

     void Hand::execute_finger_movement_(FingerMovement &movement) {
          Finger& finger = get_finger_(movement.finger);
          std::visit( [&finger](auto&& arg) {
               using T = std::decay_t<decltype(arg)>;
               if constexpr (std::is_same_v<T, Flexion>) {
                    finger.flex(arg);
               } else if constexpr (std::is_same_v<T, Extension>) {
                    finger.extend(arg);
               }
          }, movement.flex_value);
     }

     void Hand::execute_thumb_movement_(ThumbMovement &movement) {

     }

     void Hand::apply_single_movement_(std::variant<FingerMovement,ThumbMovement> &movement) {
          std::visit([this](auto&& arg) {
               using T = std::decay_t<decltype(arg)>;
               if constexpr (std::is_same_v<T, FingerMovement>) {
                    execute_finger_movement_(arg);
               } else if constexpr (std::is_same_v<T, ThumbMovement>) {
                    execute_thumb_movement_(arg);
               }
          }, movement);
     }


     void Hand::apply(std::span<std::variant<FingerMovement,ThumbMovement>> movements) {
          if (movements.empty()) {
               return;
          }
          std::vector<std::thread> active_movement_threads;

          for (const auto& movement : movements) {
               active_movement_threads.emplace_back(&Hand::apply_single_movement_, this, movement);
          }

          for (auto& t : active_movement_threads) {
               if (t.joinable()) {
                    t.join();
               }
          }
          std::cout << "All individual movement threads have completed their tasks." << std::endl;
     }





}
