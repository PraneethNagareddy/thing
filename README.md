# Thing - Robotic Hand Control Library

This repository contains the software abstraction and control library for "Thing," a 7 Degrees of Freedom (DoF) robotic hand.

## TODO

*   Include Bill of Materials
*   Include the STL file of the updated servo horn to fit Feetech SCS0009 servos.

## Hardware Design Origin

The mechanical design of the "Thing" robotic hand is adapted from the **Aero Project**. All original STL files for the hand's components can be found at the following repository:

*   **Aero Hand Open Project:** [https://github.com/TetherIA/aero-hand-open](https://github.com/TetherIA/aero-hand-open)

Please note that the original Aero project design is optimized for the HLS3606M servo motors. For "Thing," modifications have been made specifically to the servo horn mounts to ensure compatibility with the Feetech SCS0009 servo motors. The updated STL file for the SCS0009-compatible servo horn will be provided in the TODO section above.

## Robotic Hand "Thing" Overview

"Thing" is designed to mimic the basic movements of a human hand, focusing on grasping and manipulation. It features a total of 7 Degrees of Freedom distributed as follows:

*   **Thumb:** 3 DoF
    *   **Flexion/Extension:** Allows the thumb to curl inwards towards the palm (flexion) or straighten outwards (extension).
    *   **Abduction/Adduction:** Moves the thumb away from the palm (abduction) or closer to the palm (adduction) in the plane of the palm.
    *   **Opposition/Reposition:** Enables the thumb to rotate and move across the palm to touch the fingertips (opposition) or return to its resting position (reposition). This is crucial for grasping.

*   **Fingers (Index, Middle, Ring, Pinky):** 1 DoF each (total 4 DoF)
    *   **Flexion/Extension:** Each finger can curl inwards towards the palm (flexion) or straighten outwards (extension). This single DoF typically controls the movement across all joints of the finger simultaneously (e.g., all knuckles bend together).

This configuration allows "Thing" to perform various grasping patterns, from power grasps to precision pinches.
