#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cmath>

using namespace std;

long byte_multiply(long x, long g){
    return (x ^ g);
}

void encrypt(string name){
    vector <char> ans;
    string s;
    long password;
    ofstream fout;
    fout.open(name, ios_base::binary);
    cout << endl << "Type your string" << endl;
    cin >> s;
    cout << endl << "Type your password" << endl;
    cin >> password;
    srand(password);
    if (s.length() % 4 != 0){
        for(int i = 0; i < s.length() % 4; i++){
            s+=char(0b00000000);
        }
    }
    for (int i = 0; i < s.length(); i += 4){
        long fix_coding = 255;
        long g = rand() * pow(2, 16) + rand();
        long temp = ((long(s[i]) & fix_coding) << 24) |
                ((long(s[i + 1]) & fix_coding) << 16) |
                ((long(s[i + 2]) & fix_coding) << 8) |
                ((long(s[i + 3]) & fix_coding));
        long s1 = byte_multiply(temp, g);
        long s2 = (s1 << 4) & 0b11111111111111111111111111110000;
        long s3 = (s1 >> 28) & 0b00000000000000000000000000001111;
        s1 = s2 | s3;
        string str = "";
        ans.push_back(char(s1 >> 24));
        ans.push_back(char(s1 >> 16));
        ans.push_back(char(s1 >> 8));
        ans.push_back(char(s1));

    }
    for (int i = 0; i < ans.size(); i++) {
        fout << ans[i];
    }
    fout.close();
}

void decrypt(string name){
    vector <char> s;
    long password;
    ifstream fin;
    fin.open(name, ios_base::binary);
    char chch;
    while (!fin.eof()){
        fin >> chch;
        s.push_back(chch);
    }
    s.pop_back();
    cout << endl << "Type your password" << endl;
    cin >> password;
    srand(password);
    string ans = "";
    for (int i = 0; i < s.size(); i += 4){
        long fix_coding = 255;
        long g = rand() * pow(2, 16) + rand();
        long temp = ((long(s[i]) & fix_coding) << 24) |
                    ((long(s[i + 1]) & fix_coding) << 16) |
                    ((long(s[i + 2]) & fix_coding) << 8) |
                    ((long(s[i + 3]) & fix_coding));
        long s2 = (temp >> 4) & 0b00001111111111111111111111111111;
        long s3 = (temp << 28) & 0b11110000000000000000000000000000;
        temp = s2 | s3;
        long s1 = byte_multiply(temp, g);
        ans += char(s1 >> 24);
        ans += char(s1 >> 16);
        ans += char(s1 >> 8);
        ans += char(s1);
    }
    cout << ans;
    fin.close();
}

int main(int argc, const char* argv[]) {
    if (argc != 3) {
        cerr << "Error: Use three parameters\n";
        return 5;
    }
    const string mode(argv[1]); // Режим работы
    const string file_name(argv[2]); // Имя файла
    if (mode == "encryption") {
        encrypt(file_name);
    }
    else if (mode == "decryption") {
        decrypt(file_name);
    }
    return 0;
}

