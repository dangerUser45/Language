#pragma once

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-overflow"

#ifndef COLOR_H
#define COLOR_H

#define RESET           "\x1B[0m"
#define RED             "\x1B[31;1m"
#define GREEN           "\x1B[32;1m"
#define YELLOW          "\x1B[33;1m"
#define BLUE            "\x1B[34;1m"
#define MAGENTA         "\x1B[35;1m"
#define CYAN            "\x1B[36;1m"
#define WHITE           "\x1B[37;1m"
#define INDIGO          "\x1B[38;2;75;0;130m"
#define ORANGE          "\x1B[38;2;255;165;0m"
#define SKY_BLUE        "\x1B[38;2;82;169;254m"

#define GREEN_BGD       "\x1B[48;2;138;225;52m\x1B[1;38;2;46;52;54m"
#define ORANGE_BGD      "\x1B[48;2;255;165;0m\x1B[1;38;2;46;52;54m"

#define COLOR_TEXT(COLOR, TEXT) COLOR TEXT RESET

#define RESET__         "</span>"
#define RED__           "<span class=\"red\">"
#define BLUE__          "<span class=\"blue\">"
#define MAGENTA__       "<span class=\"magenta\">"
#define GREEN__         "<span class=\"green\">"
#define YELLOW__        "<span class=\"yellow\">"
#define WHITE__         "<span class=\"white\">"
#define INDIGO__        "<span class=\"indigo\">"
#define ORANGE__        "<span class=\"orange\">"
#define PEARL__         "<span class=\"pearl\">"
#define SKY_BLUE__      "<span class=\"sky_blue\">"

#define SPACE__ "&emsp;"

#define COLOR_TEXT__(COLOR, TEXT) COLOR TEXT RESET__
#define ORANGE_BGD__    "<span class=\"TextOrangeBackground\">"

#pragma GCC diagnostic pop

#endif
