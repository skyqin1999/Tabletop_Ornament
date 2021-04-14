# Tabletop_Ornament


- Project name:Tabletop ornament by esp8266
- Time: 2020-03
- Author: Kelevc(skyqin1999)
- Aim: Gifts for good friends
- To-do:
  - Weather warning information
  - Countdown function
- About project:
  - tech blog: https://www.write-down.tech/junior/tabletop-ornament.html
  - github: https://github.com/skyqin1999/Tabletop_Ornament/
- Some Tool Links:
  * Display library: https://github.com/olikraus/u8g2/wiki/u8g2reference
  * Taichi Chinese ESP8266 Doc: http://www.taichi-maker.com/homepage/iot-development/iot-dev-reference/esp8266-c-plus-plus-reference/
  * Arduinojson Library: https://arduinojson.org/v6/assistant/
  * An API for obtaining geographic information from IP: https://ipstack.com/
  * An API for weather: https://caiyunapp.com/
  * My tech site to offers updates: https://www.write-down.tech/iot/
- README:
  
  (Because Chinese may be garbled, most of the program notes are written in English and translated by Baidu)
  Due to the limitation of memory size, it is impossible to store all the astronaut animation image information in the static area of the program, so I used an extra program to save the image in flash of esp8266.
  If you need to use astronaut boot animation, you need to use additional programs to upload images. After uploading, you can set ANI to true to use boot animation, otherwise boot is a simple loading effect.

  由于内存大小的限制，无法将所有的太空人动画图片信息存储到程序的静态区中，所以我是用了额外的程序将图片存到esp8266的flash中
  如果你需要使用太空人开机动画，你需要使用额外的程序上传图像，完成上传后你可以通过设置ANI为真来使用开机动画，否则开机就是简单的加载效果

