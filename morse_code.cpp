#include <iostream>
#include <string>
#include <vector>
#include <map>    
#include <cctype> 
#include <sstream> 
#include <stdexcept> // std::stoi를 위한 예외 처리

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
    charToMorse[':'] = "---..."; // 시간 포맷을 위해 추가

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
            morse += "   "; // 단어 구분: 세 칸 공백
        } else {
            auto it = charToMorse.find(lower_c);
            if (it != charToMorse.end()) {
                morse += it->second + ' '; // 문자 구분: 한 칸 공백
            }
            // else: 알 수 없는 문자는 무시 
        }
    }
    // 마지막에 붙은 불필요한 공백 제거
    if (!morse.empty() && morse.back() == ' ') {
        morse.pop_back();
    }
}

// 모스부호를 입력받으면 텍스트를 반환하는 멤버함수
void Morse::morse2Text(const std::string& morse, std::string& text) {
    text.clear(); // 결과 문자열 초기화
    std::string processedMorse = morse;

    // "   " (세 칸 공백)을 "@@@"와 같은 고유한 구분자로 대체하여 단어 경계를 표시
    size_t pos = processedMorse.find("   ");
    while (pos != std::string::npos) {
        processedMorse.replace(pos, 3, " @@@ ");
        pos = processedMorse.find("   ", pos + 5); // 대체된 부분 뒤에서 다시 검색
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

// 24시간 형식 (HH:MM)을 12시간 형식 (H:MM AM/PM)으로 변환하는 유틸리티 함수
void convert24to12(const std::string& input_24h, std::string& output_12h) {
    if (input_24h.length() != 5 || input_24h[2] != ':') {
        output_12h = input_24h; 
        return;
    }

    try {
        int hour = std::stoi(input_24h.substr(0, 2));
        std::string minute = input_24h.substr(3, 2);
        std::string ampm;
        int h12;

        if (hour == 0) {
            h12 = 12;
            ampm = "AM";
        } else if (hour < 12) {
            h12 = hour;
            ampm = "AM";
        } else if (hour == 12) {
            h12 = 12;
            ampm = "PM";
        } else { // hour > 12
            h12 = hour - 12;
            ampm = "PM";
        }

        // 최종 12시간 형식 문자열 생성 (예: "2:36 PM")
        output_12h = std::to_string(h12) + ":" + minute + " " + ampm;
    } catch (const std::exception& e) {
        output_12h = input_24h;
    }
}


int main() {
    Morse m;
    std::string input_24h_time;    // 예: "19:07" (워크플로우 입력)
    std::string formatted_12h_time; // 예: "7:07 PM" (변환 후)
    std::string morseCodeResult;

    // 표준 입력(std::cin)에서 24시간 형식의 시간 받기
    if (!std::getline(std::cin, input_24h_time) || input_24h_time.empty()) {
        // 입력이 없거나 비어 있으면 아무것도 출력하지 않고 종료
        return 0; 
    }

    // 2. 시간을 12시간 형식 (예: 7:07 PM)으로 변환
    convert24to12(input_24h_time, formatted_12h_time);

    // 3. 12시간 형식 문자열을 모스 부호로 변환
    m.text2Morse(formatted_12h_time, morseCodeResult);

    // 4. 변환된 모스 부호만 표준 출력으로 내보냄
    std::cout << morseCodeResult << std::endl;

    return 0;
}