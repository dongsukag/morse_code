#include <iostream>
#include <string>
#include <vector>
#include <map>    // std::map을 사용하기 위해 포함
#include <cctype> // tolower를 위해 포함
#include <sstream> // stringstream을 사용하기 위해 포함

// 모스부호 <==> 영문자특수문자 변환을 해주는 클래스
class Morse {
private:
    // 문자를 모스 부호로 매핑
    std::map<char, std::string> charToMorse;
    // 모스 부호를 문자로 매핑
    std::map<std::string, char> morseToChar;

public:
    Morse();
    void text2Morse(const std::string& text, std::string& morse);
    void morse2Text(const std::string& morse, std::string& text);
};

// 각각의 모스부호를 입력한다 (생성자에서 맵 초기화)
Morse::Morse() {
    // 알파벳 매핑
    charToMorse['a'] = ".-";
    charToMorse['b'] = "-...";
    charToMorse['c'] = "-.-.";
    charToMorse['d'] = "-..";
    charToMorse['e'] = ".";
    charToMorse['f'] = "..-.";
    charToMorse['g'] = "--.";
    charToMorse['h'] = "....";
    charToMorse['i'] = "..";
    charToMorse['j'] = ".---";
    charToMorse['k'] = "-.-";
    charToMorse['l'] = ".-..";
    charToMorse['m'] = "--";
    charToMorse['n'] = "-.";
    charToMorse['o'] = "---";
    charToMorse['p'] = ".--.";
    charToMorse['q'] = "--.-";
    charToMorse['r'] = ".-.";
    charToMorse['s'] = "...";
    charToMorse['t'] = "-";
    charToMorse['u'] = "..-";
    charToMorse['v'] = "...-";
    charToMorse['w'] = ".--";
    charToMorse['x'] = "-..-";
    charToMorse['y'] = "-.--";
    charToMorse['z'] = "--..";

    // 숫자 매핑
    charToMorse['0'] = "-----";
    charToMorse['1'] = ".----";
    charToMorse['2'] = "..---";
    charToMorse['3'] = "...--";
    charToMorse['4'] = "....-";
    charToMorse['5'] = ".....";
    charToMorse['6'] = "-....";
    charToMorse['7'] = "--...";
    charToMorse['8'] = "---..";
    charToMorse['9'] = "----.";

    // 특수 문자 매핑
    charToMorse['/'] = "-..-.";
    charToMorse['?'] = "..--..";
    charToMorse[','] = "--..--";
    charToMorse['.'] = ".-.-.-";
    charToMorse['+'] = ".-.-.";
    charToMorse['='] = "-...-";

    // 역방향 매핑 (모스 부호 -> 문자) 초기화
    for (const auto& pair : charToMorse) {
        morseToChar[pair.second] = pair.first;
    }
}

// 텍스트를 입력받으면 모스부호를 반환하는 멤버함수
void Morse::text2Morse(const std::string& text, std::string& morse) {
    morse.clear(); // 결과 문자열 초기화
    for (char c : text) {
        char lower_c = std::tolower(static_cast<unsigned char>(c)); // 소문자로 변환

        if (lower_c == ' ') {
            morse += "   "; // 단어 구분: 세 칸 공백
        } else {
            auto it = charToMorse.find(lower_c);
            if (it != charToMorse.end()) {
                morse += it->second + ' '; // 문자 구분: 한 칸 공백
            }
            // else: 알 수 없는 문자는 무시 (또는 다른 방식으로 처리 가능)
        }
    }
    // 마지막에 붙은 불필요한 공백 제거 (단어가 공백으로 끝나지 않도록)
    if (!morse.empty() && morse.back() == ' ') {
        morse.pop_back();
    }
}

// 모스부호를 입력받으면 텍스트를 반환하는 멤버함수
void Morse::morse2Text(const std::string& morse, std::string& text) {
    text.clear(); // 결과 문자열 초기화
    std::string processedMorse = morse;

    // "   " (세 칸 공백)을 "@@@"와 같은 고유한 구분자로 대체하여 단어 경계를 표시
    size_t pos = processedMorse.find("   ");
    while (pos != std::string::npos) {
        processedMorse.replace(pos, 3, " @@@ ");
        pos = processedMorse.find("   ", pos + 5); // 대체된 부분 뒤에서 다시 검색
    }

    std::stringstream ss(processedMorse);
    std::string token; // 각 모스 부호 토큰 또는 단어 구분자

    while (ss >> token) {
        if (token == "@@@") { // 단어 구분자 발견
            text += " ";
        } else {
            auto it = morseToChar.find(token);
            if (it != morseToChar.end()) {
                text += it->second;
            } else {
                // 알 수 없는 모스 부호는 '?'로 처리
                text += '?';
            }
        }
    }
}

int main() {
    Morse m;
    std::string sentence;
    std::string morseCodeResult; // 변수명 변경
    std::string decodedSentence; // 변수명 변경

    std::cout << "현재 시간을 입력하세요. 모스 부호로 바꿉니다" << std::endl;
    std::getline(std::cin, sentence); // '\n' 구분자 불필요

    m.text2Morse(sentence, morseCodeResult);
    std::cout << morseCodeResult << std::endl;

    std::cout << "모스 부호를 다시 현재 시간으로 바꿉니다" << std::endl;
    m.morse2Text(morseCodeResult, decodedSentence);
    std::cout << decodedSentence << std::endl;

    return 0;
}