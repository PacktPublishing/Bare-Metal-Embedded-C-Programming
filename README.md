# Bare Metal Embedded C Programming

<a href="https://www.packtpub.com/en-in/product/bare-metal-embedded-c-programming-9781835460818"> <img src="https://content.packt.com/_/image/xxlarge/B21914/cover_image_large.jpg" alt="Bare Metal Embedded C Programming" itemprop="url" height="256px" align="right">

This is the code repository for [Bare-Metal-Embedded-C-Programming](https://www.packtpub.com/en-in/product/bare-metal-embedded-c-programming-9781835460818), published by Packt.

**Develop high-performance embedded systems with C for Arm microcontrollers**

## What is this book about?
This book helps you uncover the world of Arm-based microcontroller firmware development, equipping you to decode datasheets, expertly manipulate registers, and autonomously craft optimized code.

This book covers the following exciting features:
* Decode microcontroller datasheets, enabling precise firmware development
* Master register manipulations for optimized Arm-based microcontroller firmware creation
* Discover how to navigate hardware intricacies confidently
* Find out how to write optimized firmware without any assistance
* Work on exercises to create bare-metal drivers for GPIO, timers, ADC, UART, SPI, I2C, DMA, and more
* Design energy-efficient embedded systems with power management techniques

If you feel this book is for you, get your [copy](https://a.co/d/334nmgc) today!

## Instructions and Navigations

All of the code is organized into folders. For example, Chapter1.

The code will look like the following:

```
// 22: Set PA5(LED_PIN) high 
GPIOA_OD_R |= LED_PIN; 
```

**Following is what you need for this book:**
Whether you're an experienced engineer seeking in-depth expertise in decoding datasheets, precise register manipulations, and creating firmware from scratch, or a software developer transitioning to the embedded systems domain, this book is your comprehensive guide. It equips you with the practical skills needed for confident, independent firmware development, making it an essential resource for professionals and enthusiasts in the field.

With the following software and hardware list you can run all code files present in the book (Chapter 1-18).

## Software and Hardware List
| Software/ Hardware required | OS required/ Other requirements |
| ------------------------------------ | ----------------------------------- |
| STM32CubeIDE | Windows |
| GNU Arm Embedded Toolchain | Windows |
| NUCLEO-411 Development Board | Windows |
| 10k Potentiometer| Windows |
| OpenOCD | Windows |
| Notepad++ | Windows |
| RealTerm | Windows |

## Errata
* Page 47 (line 4): **0 and bit 11 to 1** _should be_ **1 and bit 11 to 0**
* Page 78 (Paragraph 1, line 1): **This command erases the existing firmware on the microcontroller and writes the new firmware (in this case, 2_RegisterManipulation.elf) Onto it.** _should be ignored_
* Page 81 (Paragraph 1, line 5): **than we can** _should be ignored_
* Page 213 (line 2): **an 8-bit ADC has 256 levels (28), while a 12-bit ADC has 4096 levels (212)** _should be_ **an 8-bit ADC has 256 levels (2^8), while a 12-bit ADC has 4096 levels (2^12)**
* Page 214 (Paragraph 1, line 1): **an 8-bit ADC has 256(28)** _should be_ **an 8-bit ADC has 256(2^8)**
  
## Related products
* Learn C Programming [[Packt]](https://www.packtpub.com/en-in/product/learn-c-programming-9781801078450) [[Amazon]](https://a.co/d/41k1gwQ)
* Embedded Systems Architecture [[Packt]](https://www.packtpub.com/en-in/product/embedded-systems-architecture-9781803239545) [[Amazon]](https://a.co/d/eVvwdUt)

## Get to Know the Author
**Israel Gbati**
is a distinguished firmware engineer boasting over a decade of hands-on experience in the field. Over the years, he has imparted his profound knowledge to more than 100,000 professionals, fortifying the next generation of experts. In addition to his engineering skills, Israel is an entrepreneur and an award-winning inventor, recognized for his outstanding inventions. He possesses a Bachelor's degree in Mechanical Engineering and Automation, complemented by a double Masters degree in Global Innovation Design from Imperial College London and the Royal College of Arts.







