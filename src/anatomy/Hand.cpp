//
// Created by Nagareddy on 30/06/26.
//

#include <thread>
#include <vector>
#include <iostream>
#include <chrono>

#include "anatomy/Hand.h"

using namespace articulation;
using namespace articulation::movement;

namespace anatomy::hand {

     void Hand::execute_flex(Finger &finger, FingerMovement movement) {
          std::visit( [&finger, movement](auto&& arg) {
               using T = std::decay_t<decltype(arg)>;
               if constexpr (std::is_same_v<T, Flexion>) {
                    const Status status = finger.flex(arg);
                    std::cout<< "finger movement status:" << status << std::endl;
               } else if constexpr (std::is_same_v<T, Extension>) {
                    const Status status = finger.extend(arg);
                    std::cout<< "finger " << movement.finger << " movement status:" << status << std::endl;
               }
          }, movement.flex_value);
     }
     void Hand::execute_spread(Thumb &thumb, ThumbMovement movement) {
          std::visit( [&thumb, movement](auto&& arg) {
               using T = std::decay_t<decltype(arg)>;
               if constexpr (std::is_same_v<T, Adduction>) {
                    const Status status = thumb.adduct(arg);
                    std::cout<< "finger movement status:" << status << std::endl;
               } else if constexpr (std::is_same_v<T, Abduction>) {
                    const Status status = thumb.abduct(arg);
                    std::cout<< "finger " << movement.finger << " movement status:" << status << std::endl;
               }
          }, movement.spread_value);
     }
     void Hand::execute_oppose(Thumb &thumb, ThumbMovement movement) {
          std::visit( [&thumb, movement](auto&& arg) {
               using T = std::decay_t<decltype(arg)>;
               if constexpr (std::is_same_v<T, Opposition>) {
                    const Status status = thumb.oppose(arg);
                    std::cout<< "finger movement status:" << status << std::endl;
               } else if constexpr (std::is_same_v<T, Reposition>) {
                    const Status status = thumb.repose(arg);
                    std::cout<< "finger " << movement.finger << " movement status:" << status << std::endl;
               }
          }, movement.oppose_value);
     }


     void Hand::apply_finger_movement_(FingerMovement &movement) {
          if (movement.finger == Fingers::THUMB) {
               std::cerr<< "Unsupported finger type " <<movement.finger << " in executing fingermovement" <<std::endl;
               return;
          }
          Finger& finger = get_finger_(movement.finger);
          execute_flex(finger, movement);
     }

     void Hand::apply_thumb_movement_(ThumbMovement &movement) {
          if (movement.finger != Fingers::THUMB) {
               std::cerr<< "Unsupported finger type " <<movement.finger << " in executing fingermovement" <<std::endl;
               return;
          }
          Thumb& thumb = get_thumb_();
          execute_flex(thumb, movement);
          execute_spread(thumb, movement);
          execute_oppose(thumb, movement);
     }

     void Hand::apply_single_movement_(std::variant<FingerMovement,ThumbMovement> &movement) {
          std::visit([this](auto&& arg) {
               using T = std::decay_t<decltype(arg)>;
               if constexpr (std::is_same_v<T, FingerMovement>) {
                    apply_finger_movement_(arg);
               } else if constexpr (std::is_same_v<T, ThumbMovement>) {
                    apply_thumb_movement_(arg);
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
