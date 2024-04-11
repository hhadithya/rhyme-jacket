# Fatigue level detection by using DS18B20

![ds18b20 picture](https://techshop.lk/cdn/shop/products/DS18B20.jpg?v=1676439313)

## Features

- Unique 1-Wire interface requires only one port pin for communication
- Power supply range is 3.0V to 5.5V
- Measures temperatures from -55°C to +125°C. Fahrenheit equivalent is -67°F to +257°F
- ±0.5°C accuracy from -10°C to +85°C

## Wires

![GND, VDD, DQ](https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2019/07/DS18B20-tempeature-sensor-pinout.jpg?w=750&quality=100&strip=all&ssl=1)

## Challenges

Environment temperature can be differed according to the place. And those differed temperature can affect the body temperature , resulting in false assumptions of getting tired.

## Coding

- There are **three fatigue levels** like Level 0, Level 1, Level 2. Level 2 means too tired and level 0 means not tired. A blinking Red LED is used to indicate level 2 fatigueness. A blinking blue LED is used to indicate level 1 fatigueness. There is no indicator for the level 0 fatigueness.
- The code is supported for cold and hot temperatures as well.

## Testing

![setting up using arduino nano board](https://github.com/hhadithya/rhyme-jacket/blob/main/Progress/Hardware/DS18B20_test.jpg?raw=true)
