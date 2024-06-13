# Pitch and Roll Angles

## Pitch Angle
The pitch angle measures the rotation around the lateral axis, which is the axis running from side to side. In other words, it describes how much the object is tilted forward or backward.

- **Positive Pitch**: Tilting the front of the object upwards.
- **Negative Pitch**: Tilting the front of the object downwards.

## Roll Angle
The roll angle measures the rotation around the longitudinal axis, which is the axis running from the front to the back of the object. This angle describes how much the object is tilted to the left or right.

- **Positive Roll**: Tilting the right side of the object downwards (left side up).
- **Negative Roll**: Tilting the left side of the object downwards (right side up).

## Visual Representation
Imagine an airplane:
- **Pitch**: If the nose of the airplane points up or down, that's changing the pitch.
- **Roll**: If one wing dips down and the other rises, that's changing the roll.

## Calculating Pitch and Roll from Accelerometer Data
Using the accelerometer data from the MPU6050, you can calculate these angles as follows:

### Pitch
It is calculated using the Y and Z components of the accelerometer.

\[ \text{Pitch} = \arctan \left( \frac{a_y}{\sqrt{a_x^2 + a_z^2}} \right) \]

where \( a_x \), \( a_y \), and \( a_z \) are the acceleration readings along the X, Y, and Z axes, respectively.

### Roll
It is calculated using the X and Z components of the accelerometer.

\[ \text{Roll} = \arctan \left( \frac{-a_x}{a_z} \right) \]

### Notes
- \( \arctan \) is the arctangent function (often represented as `atan` in programming languages).
- The result is usually converted from radians to degrees by multiplying by \( \frac{180}{\pi} \).

These calculations assume that the sensor is stationary and the accelerations due to movement are negligible compared to gravity.

### Code Snippet
Here’s a brief snippet from the previous code showing these calculations:

```cpp
float pitch = atan2(a.acceleration.y, sqrt(a.acceleration.x * a.acceleration.x + a.acceleration.z * a.acceleration.z)) * 180 / PI;
float roll = atan2(-a.acceleration.x, a.acceleration.z) * 180 / PI;
