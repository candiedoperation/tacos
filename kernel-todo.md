### tacos::Drivers::HAL::Ps2Kbd
1. Define statics for currently pressed keys and stuff
2. Send sys interrupts in future for vterm, maybe?
3. Control Caps Lock and stuff using the static states.
4. Define ASCII keycodes?
6. Have an update fn or directly get interrupts from isr?

### tacos::Drivers::HAL::VirtualKbd
1. Define a Kernel Keyboard Input Packets:
    - Unicode code point (if applicable)
    - Key code
    - Pressed/released flag
    - Various other key states (shift, alt, control, etc)
    - Various "toggle" states (CapsLock, ScrollLock, NumberLock, etc)

2. Support Internationalization. Translate KeyCodes to UTF based
    on the current language set for the OS.

3. Provide a uniform Kbd Input all to components on top of Kernel
    irrespective of whether input's from PS/2 Keyboards, USB kbds,
    or some software based virtual keyboard.

4. Event loops to notify userspace? VFS Device Files? Implement something similar to https://learn.microsoft.com/en-us/windows/win32/inputdev/about-keyboard-input 