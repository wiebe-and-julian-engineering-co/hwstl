```
 _ _ _ _     _                    _       __     _ _         
| | | |_|___| |_ ___    ___ ___ _| |   __|  |_ _| |_|___ ___ 
| | | | | -_| . | -_|  | .'|   | . |  |  |  | | | | | .'|   |
|_____|_|___|___|___|  |__,|_|_|___|  |_____|___|_|_|__,|_|_|
                                                             
                                                             
 _____         _                 _            _____          
|   __|___ ___|_|___ ___ ___ ___|_|___ ___   |     |___      
|   __|   | . | |   | -_| -_|  _| |   | . |  |   --| . |_    
|_____|_|_|_  |_|_|_|___|___|_| |_|_|_|_  |  |_____|___|_|   
          |___|                       |___|                  
```

## HWSTL

### Fundamentals
- Fully templated (when possible).
- Easy to use debugging inspectors (maybe integration with JTAG) (as all libraries claim ;0).
- Able to get started quickly, but fully configurable. The end user should have the final word in term of higher performance or less flash usage.
- Cross platform compatible (Arduino IDE style multiplatform programming as it should be done).
- When possible; implementation of safety features, in practicular during compile time as it doesn't cause much overhead.

## Legacy code
HWSTL is target to code breaking changes. As HWSTL 1.0 was deprecated, the following features may return to HWSTL 2.0.

Keeping:
- hal -> istream/ostream/iostream
- hal -> streambuf
- memory -> ring_buffer
- target -> arduino_due
- target -> uart (only keep the core io fundamentials, remove the std_interface implementation).

Considering:
- std_interface (maybe only use the pattern, throw all implementation stuff out).
- target -> type_definition

### About
HWSTL is managed by Wiebe van Breukelen and Julian van Doorn


