#include "accent.h"

unsigned char get_accented_char(unsigned char normal_char, unsigned char accent)
{
    /*Create letters with the following accents: | ^ | ` | ~ | ¨ | ´ | */
    unsigned char letter;
    if (normal_char == 'A') {
        if (accent == 94) letter = 194;
        else if (accent == 96) letter = 192;
        else if (accent == 126) letter = 195;
        else if (accent == 168) letter = 196;
        else if (accent == 180) letter = 193;
        else letter = 'A';
    }
    else if (normal_char == 'a') {
        if (accent == 94) letter = 226;
        else if (accent == 96) letter = 224;
        else if (accent == 126) letter = 227;
        else if (accent == 168) letter = 228;
        else if (accent == 180) letter = 225;
        else letter = 'a';
    }
    else if (normal_char == 'E') {
        if (accent == 94) letter = 202;
        else if (accent == 96) letter = 200;
        else if (accent == 168) letter = 203;
        else if (accent == 180) letter = 201;
        else letter = 'E';
    }
    else if (normal_char == 'e') {
        if (accent == 94) letter = 234;
        else if (accent == 96) letter = 232;
        else if (accent == 168) letter = 235;
        else if (accent == 180) letter = 233;
        else letter = 'e';
    }
    else if (normal_char == 'I') {
        if (accent == 94) letter = 206;
        else if (accent == 96) letter = 204;
        else if (accent == 168) letter = 207;
        else if (accent == 180) letter = 205;
        else letter = 'I';
    }
    else if (normal_char == 'i') {
        if (accent == 94) letter = 238;
        else if (accent == 96) letter = 236;
        else if (accent == 168) letter = 239;
        else if (accent == 180) letter = 237;
        else letter = 'i';
    }
    else if (normal_char == 'O') {
        if (accent == 94) letter = 212;
        else if (accent == 96) letter = 210;
        else if (accent == 126) letter = 213;
        else if (accent == 168) letter = 214;
        else if (accent == 180) letter = 211;
        else letter = 'O';
    }
    else if (normal_char == 'o') {
        if (accent == 94) letter = 244;
        else if (accent == 96) letter = 242;
        else if (accent == 126) letter = 245;
        else if (accent == 168) letter = 246;
        else if (accent == 180) letter = 243;
        else letter = 'o';
    }
    else if (normal_char == 'U') {
        if (accent == 94) letter = 219;
        else if (accent == 96) letter = 217;
        else if (accent == 168) letter = 220;
        else if (accent == 180) letter = 218;
        else letter = 'U';
    }
    else if (normal_char == 'u') {
        if (accent == 94) letter = 251;
        else if (accent == 96) letter = 249;
        else if (accent == 168) letter = 252;
        else if (accent == 180) letter = 250;
        else letter = 'u';
    }
    else if (normal_char == 'Y') {
        if (accent == 180) letter = 221;
        else letter = 'Y';
    }
    else if (normal_char == 'y') {
        if (accent == 168) letter = 255;
        else if (accent == 180) letter = 253;
        else letter = 'y';
    }
    else if (normal_char == 'N') {
        if (accent == 126) letter = 209;
        else letter = 'N';
    }
    else if (normal_char == 'n') {
        if (accent == 126) letter = 241;
        else letter = 'n';
    }
    return letter;
}
