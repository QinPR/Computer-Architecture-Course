#include <stdio.h>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <cstring>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "assembler.h"

using namespace std;

map<string, int*> find_registry;
int pc = 0x400000;
int text_pc = 0x400000;

char *real_men = NULL;
uint *LO = new uint;
int *HI = new int;

string binary_adder(string a, string b, int bit = 32){                //require a, and b 32 or 64 bits
    int carry = 0;
    string result = "";
    for (int i = bit-1; i >= 0; i--){
        if (a[i] == '0' && b[i] == '0'){
            if (carry == 1){
                result = "1" + result;
                carry = 0;
            }
            else{
                result = "0" + result;
            }
        }
        else if (a[i] == '1' && b[i] == '1'){
            if (carry == 1){
                result = "1" + result;
                carry = 1;
            }
            else{
                result = "0" + result;
                carry = 1;
            }
        }
        else{
            if (carry == 1){
                result = "0" + result;
                carry = 1;
            }
            else{
                result = "1" + result;
                carry = 0;
            }
        }
    }
    if (result.length() != bit){
        return "overflow";
    }
    return result;
}

string binary_mul(string a, string b){                  //require a, and b 32 bits
    string result = "0000000000000000000000000000000000000000000000000000000000000000";
    int n = 0;
    for (int i = 0; i < b.length(); i++){
        if (b[b.length() - 1 - i] == '1'){
            string partial = a;
            for (int k = 0; k < n; k++){
                partial += "0";
            }
            while (partial.length() != 64){
                partial = "0" + partial;
            }
            result = binary_adder(result, partial, 64);
        }
        n += 1;
    }
    return result;
}

void so_registry(){
    *HI = 0;
    *LO = 0;
    find_registry["00000"] = new int;
    find_registry["00001"] = new int;
    find_registry["00010"] = new int;
    find_registry["00011"] = new int;
    find_registry["00100"] = new int;
    find_registry["00101"] = new int;
    find_registry["00110"] = new int;
    find_registry["00111"] = new int;
    find_registry["01000"] = new int;
    find_registry["01001"] = new int;
    find_registry["01010"] = new int;
    find_registry["01011"] = new int;
    find_registry["01100"] = new int;
    find_registry["01101"] = new int;
    find_registry["01110"] = new int;
    find_registry["01111"] = new int;
    find_registry["10000"] = new int;
    find_registry["10001"] = new int;
    find_registry["10010"] = new int;
    find_registry["10011"] = new int;
    find_registry["10100"] = new int;
    find_registry["10101"] = new int;
    find_registry["10110"] = new int;
    find_registry["10111"] = new int;
    find_registry["11000"] = new int;
    find_registry["11001"] = new int;
    find_registry["11010"] = new int;
    find_registry["11011"] = new int;
    find_registry["11100"] = new int;
    find_registry["11101"] = new int;
    find_registry["11110"] = new int;
    find_registry["11111"] = new int;

    *find_registry["00001"] = 0;
    *find_registry["00010"] = 0;
    *find_registry["00011"] = 0;
    *find_registry["00100"] = 0;
    *find_registry["00101"] = 0;
    *find_registry["00110"] = 0;
    *find_registry["00111"] = 0;
    *find_registry["01000"] = 0;
    *find_registry["01001"] = 0;
    *find_registry["01010"] = 0;
    *find_registry["01011"] = 0;
    *find_registry["01100"] = 0;
    *find_registry["01101"] = 0;
    *find_registry["01110"] = 0;
    *find_registry["01111"] = 0;
    *find_registry["10000"] = 0;
    *find_registry["10001"] = 0;
    *find_registry["10010"] = 0;
    *find_registry["10011"] = 0;
    *find_registry["10100"] = 0;
    *find_registry["10101"] = 0;
    *find_registry["10110"] = 0;
    *find_registry["10111"] = 0;
    *find_registry["11000"] = 0;
    *find_registry["11001"] = 0;
    *find_registry["11010"] = 0;
    *find_registry["11011"] = 0;
    *find_registry["11100"] = 0;
    *find_registry["11111"] = 0;
    *find_registry["11101"] = 0xa00000;
    *find_registry["11110"] = 0x500000;
    *find_registry["00000"] = 0;
}

string tentotwo_var(string s, int n){
    int64_t ten_number = (int64_t)stoi(s);
    int remainder;
    string result = "";
    if (ten_number == 0){
        while (result.length() != n){
            result += "0";
        }
        return result;
    }
    
    while (ten_number != 1){
        remainder = ten_number % 2;
        result = to_string(remainder) + result;
        ten_number = ten_number/2; 
    }
    if (ten_number == 1){
        result = "1" + result;
    }
    else{
        result = "0" + result;
    }
    while (result.length() != n-1){
        result = "0" + result;
    }
    return result;
}

string tentotwo_sixtythree(int64_t ten_number){
    int64_t remainder;
    string result = "";
    if (ten_number == 0){
        return "000000000000000000000000000000000000000000000000000000000000000";
    }
    while (ten_number != 1){
        remainder = ten_number % 2;
        result = to_string(remainder) + result;
        ten_number = ten_number/2; 
    }
    result = "1" + result;
    while (result.length() != 63){
        result = "0" + result;
    }
    return result;
}

string twoscomplement(string neg_value, int bit){
    string onesform = "";
    string posi_neg_value = "";
    string twosform = "";
    if (neg_value == "-1"){
        if (bit == 32){
            return "11111111111111111111111111111111";
        }
        else{
            return "1111111111111111111111111111111111111111111111111111111111111111";
        }
    }

    for (int n = 1; n < neg_value.length(); n++){
        posi_neg_value.push_back(neg_value[n]);
    }
    string first;
    if (bit == 32){
        first = tentotwo_var(posi_neg_value, 32);
    }
    else if (bit == 64){
        first = tentotwo_var(posi_neg_value, 64);
    }
    for (int i = 0; i < first.length(); i++){
        if (first[i] == '1'){
            onesform.push_back('0');
        }
        else if (first[i] == '0'){
            onesform.push_back('1');
        }
    }
    onesform = "1" + onesform;
    int carry = 1;
    int x;
    if (bit == 32){
        x = 31;
    }
    else{
        x = 63;
    }
    for (int i = x; i >= 0; i--){
        if (onesform[i] == '1' && carry == 1){
            onesform[i] = '0';
        }
        else if (onesform[i] == '0' && carry == 1){
            onesform[i] = '1';
            carry = 0;
        }
    }
    twosform = onesform;
    return twosform;
}

string tentotwo_thirtytwo(int ten_number){
    int remainder;
    string result = "";
    if (ten_number == 0){
        return "00000000000000000000000000000000";
    }
    else if (ten_number < 0){
        return twoscomplement(to_string(ten_number), 32);
    }
    while (ten_number != 1){
        remainder = ten_number % 2;
        result = to_string(remainder) + result;
        ten_number = ten_number/2; 
    }
    result = "1" + result;
    while (result.length() != 32){
        result = "0" + result;
    }
    return result;
}

string tentotwo_sixtyfour(int64_t ten_number){
    int64_t remainder;
    string result = "";
    if (ten_number == 0){
        return "0000000000000000000000000000000000000000000000000000000000000000";
    }
    else if (ten_number < 0){
        return twoscomplement(to_string(ten_number), 64);
    }
    while (ten_number != 1){
        remainder = ten_number % 2;
        result = to_string(remainder) + result;
        ten_number = ten_number/2; 
    }
    result = "1" + result;
    while (result.length() != 64){
        result = "0" + result;
    }
    return result;
}

string str_twoscomplement(string num){              // input the  number, output twosform
    num[0] = '1';
    int carry = 1;
    for (int i = num.length()-1; i >= 0; i--){
        if (carry == 1 && num[i] == '1'){
            num[i] = '0';
        }
        else if (carry == 1 && num[i] == '0'){
            num[i] = '1';
            carry = 0;
        }
    }
    for (int i = 0; i < num.length(); i++){
        if (num[i] == '0'){
            num[i] = '1';
        }
        else{
            num[i] = '0';
        }
    }
    return num;
}

unsigned int calculate_u(int signed_n){
    string binary = tentotwo_thirtytwo(signed_n);
    int n = 1;
    unsigned int result = 0;
    for (int i = 31; i >= 0; i--){
        result += (binary[i] - '0') *n;
        n *= 2;
    }
    return result;
}

void memory_init(){
    real_men = (char*)malloc(0x600000);
    find_registry["11101"] = (int*)real_men + 0x600000 - 1;
    find_registry["11110"] = (int*)real_men + 0x100000;
}

string find_type(istream & is, char first){
    char character;
    string word = "";
    word.push_back(first);
    while(is.get(character)){
        if (character == ':' || character == ' ' || character == '\n' || character == '\t'){
            break;
        }
        word += character;
    }
    return word;
}

string s_find_segment(istream & is, bool & finish, bool ascii = false){
    char character;
    string segment = "";
    bool doubleover = true;
    while(is.get(character)){
        if (character == '"'){
            if (doubleover){
                doubleover = false;
            }
            else{
                doubleover = true;
            }
        }
        if (character == ',' && !ascii){
            is.get(character);
            break;
        }
        else if (character == 'n' && segment[segment.length()-1] == char(92)){
            segment[segment.length()-1] = '\n';
        }
        else if (character == 't' && segment[segment.length()-1] == char(92)){
            segment[segment.length()-1] = '\n';
        }
        else if (character == ' ' && segment != "" && doubleover){
            break;
        }
        else if (character == '\n' || character == '\t'){
            finish = true;
            return segment;
        }
        else if (character != ' ' || !doubleover){
            segment += character;
        }
        
    }
    return segment;
}

int twototen_u(string two){
    int result = 0;
    int n = 1;
    for (int i = 0; i < two.length(); i++){
        result += (two[two.length()-1-i] - '0') * n;
        n *= 2;
    }
    return result;
}

int twototen_s(string two, int bit = 16){
    int result = 0;
    if (two[0] == '1'){
        int carry = -1;
        for (int i = bit-1; i>=0; i--){
            if (two[i] == '0' && carry == -1){
                two[i] = '1';
            }
            else if (two[i] == '1' && carry == -1){
                two[i] = '0';
                carry = 0;
            }
        }
        for (int i = 0; i<two.length(); i++){
            if (two[i] == '0'){
                two[i] = '1';
            }
            else{
                two[i] = '0';
            }
        }
        result = 0 - twototen_u(two);
    }
    else{
        result = twototen_u(two);
    }
    return result;
}

void put_data(){
    ifstream data_file;
    char character;
    string type;
    string structure;
    data_file.open("data_put.txt");

    while (data_file.get(character)){
        type = "";
        structure = "";
        if (character == ' ' || character == '\n' || character == '\t'){
            int i = 0;
        }
        else{
            type = find_type(data_file, character);
        }
        bool dontuseit;
        structure = s_find_segment(data_file, dontuseit);
        if (structure == ".asciiz" || structure == ".ascii"){
            bool finish = false;
            while (true){
                if (finish){
                    break;
                }
                string str = s_find_segment(data_file, finish, true);
                for (int i = 1; i < str.length()-1; i++){
                    *(real_men + *find_registry["11110"] - 0x400000) = str[i];
                    *find_registry["11110"] += 1;
                }
                if (structure == ".asciiz"){
                    *(real_men + *find_registry["11110"] - 0x400000) = '\0';
                    *find_registry["11110"] = (*find_registry["11110"]/4 + 1)*4;
                }
                else if (structure == ".ascii"){
                    if (*find_registry["11110"]%4 != 0){
                        *find_registry["11110"] = (*find_registry["11110"]/4 + 1)*4;
                    }
                }
            }
        }
        else if (structure == ".word"){
            if (*find_registry["11110"]%4 != 0){
                *find_registry["11110"] = (*find_registry["11110"]/4 + 1)*4;
            }
            bool finish = false;
            while (true){
                if (finish){
                    break;
                }
                string str_seg = s_find_segment(data_file, finish);
                int seg;
                seg = stoi(str_seg);
                int *pt;
                pt = &seg;
                memcpy((real_men + *find_registry["11110"] - 0x400000), pt, 4);
                *find_registry["11110"] += 4;
            }
        }
        else if (structure == ".byte"){
            bool finish = false;
            while (true){
                if (finish){
                    break;
                }
                string str = s_find_segment(data_file, finish);
                *(real_men + *find_registry["11110"] - 0x400000) = char(stoi(str));
                *find_registry["11110"] += 1;
            }
            *find_registry["11110"] -= 1;
            *find_registry["11110"] = (*find_registry["11110"]/4 + 1)*4;
        }
        else if (structure == ".half"){
            bool finish = false;
            while (true){
                if (finish){
                    break;
                }
                string str_seg = s_find_segment(data_file, finish);
                short seg;
                seg = stoi(str_seg);
                short *pt;
                pt = &seg;
                memcpy((real_men + *find_registry["11110"] - 0x400000), pt, 2);
                *find_registry["11110"] += 2;
            }
            *find_registry["11110"] -= 2;
            *find_registry["11110"] = (*find_registry["11110"]/4 + 1)*4;
        }
    }
    data_file.close();
}

void put_code(){
    ifstream code_file;
    string code;
    code_file.open("machine_code.txt");
    int n = 0;
    while (getline(code_file, code)){
        int code_int = twototen_u(code);
        int *pt;
        pt = &code_int;
        memcpy((real_men + text_pc - 0x400000), pt, 4);
        text_pc += 4;
    }
}
    
string getop(string code){
    string result = "";
    for(int i = 0; i < 6; i++){
        result += code[i];
    }
    return result;
}

string getsegment(string code, int start, int length){
    string result = "";
    for (int i = (start-1); i < (length+start-1); i++){
        result += code[i];
    }
    return result;
}

/*
simulated functions
*/
void add(int* rs, int* rt, int* rd){
    try{
        *rd = *rs + *rt;
    }
    catch(exception){
        throw("Integer Overflow");
    }
    
}
void addu(int* rs, int* rt, int* rd){
    *rd = calculate_u(*rs) + calculate_u(*rt);
}

void addi(int*rs, int*rt, int imm){
    try{
       *rt = *rs + imm; 
    }
    catch(exception){
        throw("Integer Overflow");
    }
    
}

void addiu(int*rs, int*rt, int imm){
    *rt = calculate_u(*rs) + imm;
}

void AND(int* rs, int* rt, int* rd){
    int result;
    string str_result = "";
    string str_rs = tentotwo_thirtytwo(*rs);
    string str_rt = tentotwo_thirtytwo(*rt);
    for (int i = 0; i < 32; i++){
        if (str_rs[i] == '1' && str_rt[i] == '1'){
            str_result = str_result + "1";
        }
        else{
            str_result = str_result + "0";
        }
    }
    result = twototen_u(str_result);
    *rd = result;
}

void andi(int* rs, int* rt, int imm){
    int result;
    string str_result = "";
    string str_rs = tentotwo_thirtytwo(*rs);
    string imm_str = tentotwo_thirtytwo(imm);
    for (int i = 0; i < 32; i++){
        if (str_rs[i] == '1' && imm_str[i] == '1'){
            str_result = str_result + "1";
        }
        else{
            str_result = str_result + "0";
        }
    }
    result = twototen_u(str_result);
    *rt = result;
}

void clo(int* rs, int* rd){
    string str_rs = tentotwo_thirtytwo(*rs);
    int n = 0;
    while(str_rs[n] == '0'){
        n += 1;
    }
    *rd = n;
}

void clz(int* rs, int* rd){
    string str_rs = tentotwo_thirtytwo(*rs);
    int n = 0;
    while(str_rs[n] == '1'){
        n += 1;
    }
    *rd = n;
}


void sub(int* rs, int* rt, int* rd){
    try{
        *rd = *rs - *rt;
    }
    catch(exception){
        throw("Integer Overflow");
    }
}

void subu(int* rs, int* rt, int* rd){
    *rd = *rs - *rt;
}

void div(int* rs, int* rt){
    *LO = (uint)((*rs)/(*rt));
    *HI = ((*rs)%(*rt));
}

void divu(int*rs, int* rt){
    unsigned int x = calculate_u(*rs);
    unsigned int y = calculate_u(*rt);
    *LO = (uint)(calculate_u(*rs)/calculate_u(*rt));
    *HI = (calculate_u(*rs)%calculate_u(*rt));
}

void mult(int *rs, int *rt){
    int64_t long_rs = (int64_t)(*rs);
    int64_t long_rt = (int64_t)(*rt);
    int64_t result = (long_rs) * (long_rt);
    string str_result;
    if (result < 0){
        str_result = twoscomplement(to_string(result), 64);
    }
    else{
        str_result = tentotwo_sixtyfour(result);
    }
    string answer_lo = "";
    string answer_hi = "";
    for (int i = 0; i < 32; i++){
        answer_hi.push_back(str_result[i]);
        answer_lo.push_back(str_result[i+32]);
    }
    *LO = twototen_u(answer_lo);
    *HI = twototen_s(answer_hi);
}

void multu(int *rs, int *rt){
    string str_rs = tentotwo_thirtytwo(*rs);
    string str_rt = tentotwo_thirtytwo(*rt);
    string result = binary_mul(str_rs, str_rt);
    string answer_lo = "";
    string answer_hi = "";
    for (int i = 0; i < 32; i++){
        answer_hi.push_back(result[i]);
        answer_lo.push_back(result[i+32]);
    }
    *LO = twototen_u(answer_lo);
    *HI = twototen_s(answer_hi);
}

void mul(int *rs, int *rt, int* rd){
    int64_t long_rs = (int64_t)(*rs);
    int64_t long_rt = (int64_t)(*rt);
    int64_t result = (long_rs) * (long_rt);
    string str_result;
    if (result < 0){
        str_result = twoscomplement(to_string(result), 64);
    }
    else{
        str_result = tentotwo_sixtyfour(result);
    }
    string answer_lo = "";
    for (int i = 0; i < 32; i++){
        answer_lo.push_back(str_result[i+32]);
    }
    *rd = twototen_s(answer_lo, 32);
}

void madd(int *rs, int *rt){
    int64_t long_rs = (int64_t)(*rs);
    int64_t long_rt = (int64_t)(*rt);
    int64_t result = (long_rs) * (long_rt);
    string str_result;
    if (result < 0){
        str_result = twoscomplement(to_string(result), 64);
    }
    else{
        str_result = tentotwo_sixtyfour(result);
    }
    string HI_str = tentotwo_thirtytwo(*HI);
    string LO_str = tentotwo_thirtytwo(calculate_u(*LO));
    string answer = binary_adder(str_result, (HI_str + LO_str), 64);
    string answer_lo = "";
    string answer_hi = "";
    for (int i = 0; i < 32; i++){
        answer_hi.push_back(answer[i]);
        answer_lo.push_back(answer[i+32]);
    }
    *LO = twototen_u(answer_lo);
    *HI = twototen_s(answer_hi);
}

void msub(int *rs, int *rt){
    int64_t long_rs = (int64_t)(*rs);
    int64_t long_rt = (int64_t)(*rt);
    int64_t result = (long_rs) * (long_rt);
    string HI_str = tentotwo_thirtytwo(*HI);
    string LO_str = tentotwo_thirtytwo(calculate_u(*LO));
    int64_t hiio = twototen_s((HI_str + LO_str), 64);
    string answer = tentotwo_sixtyfour(hiio - result);
    string answer_lo = "";
    string answer_hi = "";
    for (int i = 0; i < 32; i++){
        answer_hi.push_back(answer[i]);
        answer_lo.push_back(answer[i+32]);
    }
    *LO = twototen_u(answer_lo);
    *HI = twototen_s(answer_hi);
}

void maddu(int *rs, int *rt){
    string str_rs = tentotwo_thirtytwo(*rs);
    string str_rt = tentotwo_thirtytwo(*rt);
    string str_result = binary_mul(str_rs, str_rt);
    string HI_str = tentotwo_thirtytwo(*HI);
    string LO_str = tentotwo_thirtytwo(calculate_u(*LO));
    string answer = binary_adder(str_result, (HI_str + LO_str), 64);
    string answer_lo = "";
    string answer_hi = "";
    for (int i = 0; i < 32; i++){
        answer_hi.push_back(answer[i]);
        answer_lo.push_back(answer[i+32]);
    }
    *LO = twototen_u(answer_lo);
    *HI = twototen_s(answer_hi);
}

void msubu(int *rs, int *rt){
    uint64_t long_rs = (uint64_t)(*rs);
    uint64_t long_rt = (uint64_t)(*rt);
    uint64_t result = (long_rs) * (long_rt);
    string HI_str = tentotwo_thirtytwo(*HI);
    string LO_str = tentotwo_thirtytwo(calculate_u(*LO));
    int64_t hiio = twototen_s((HI_str + LO_str), 64);
    string answer = tentotwo_sixtyfour(hiio - result);
    string answer_lo = "";
    string answer_hi = "";
    for (int i = 0; i < 32; i++){
        answer_hi.push_back(answer[i]);
        answer_lo.push_back(answer[i+32]);
    }
    *LO = twototen_u(answer_lo);
    *HI = twototen_s(answer_hi);
}

void nor(int* rs, int* rt, int* rd){
    int result;
    string str_result = "";
    string str_rs = tentotwo_thirtytwo(*rs);
    string str_rt = tentotwo_thirtytwo(*rt);
    for (int i = 0; i < 32; i++){
        if (str_rs[i] == '0' && str_rt[i] == '0'){
            str_result = str_result + "1";
        }
        else{
            str_result = str_result + "0";
        }
    }
    result = twototen_s(str_result, 32);
    *rd = result;
}

void OR(int* rs, int* rt, int* rd){
    int result;
    string str_result = "";
    string str_rs = tentotwo_thirtytwo(*rs);
    string str_rt = tentotwo_thirtytwo(*rt);
    for (int i = 0; i < 32; i++){
        if (str_rs[i] == '1' || str_rt[i] == '1'){
            str_result = str_result + "1";
        }
        else{
            str_result = str_result + "0";
        }
    }
    result = twototen_s(str_result);
    *rd = result;
}

void ori(int* rs, int* rt, int imm){
    int result;
    string str_result = "";
    string str_rs = tentotwo_thirtytwo(*rs);
    string imm_str = tentotwo_thirtytwo(imm);
    for (int i = 0; i < 32; i++){
        if (str_rs[i] == '1' || imm_str[i] == '1'){
            str_result = str_result + "1";
        }
        else{
            str_result = str_result + "0";
        }
    }
    result = twototen_s(str_result, 32);
    *rt = result;
}

void sll(int* rd, int*rt, int shamt){
    int result;
    string str_result = "";
    string str_rt = tentotwo_thirtytwo(*rt);
    for (int i = shamt; i < 32; i++){
        str_result.push_back(str_rt[i]);
    }
    for (int i = 0; i < shamt; i++){
        str_result.push_back('0');
    }
    result = twototen_s(str_result, 32);
    *rd = result;
}

void sllv(int* rs, int* rt, int* rd){
    int result;
    string str_result = "";
    string str_rt = tentotwo_thirtytwo(*rt);
    string str_rs = tentotwo_thirtytwo(*rs);
    int shamt = 0;
    int n = 1;
    for (int i = 31; i >26; i--){
        if (str_rs[i] == '1'){
            shamt += n;
        }
        n *= 2;
    }
    for (int i = shamt; i < 32; i++){
        str_result.push_back(str_rt[i]);
    }
    for (int i = 0; i < shamt; i++){
        str_result.push_back('0');
    }
    result = twototen_s(str_result, 32);
    *rd = result;
}

void sra(int* rs, int* rt, int* rd, int shamt){
    int result;
    string str_result = "";
    string str_rt = tentotwo_thirtytwo(*rt);
    string str_ra = tentotwo_thirtytwo(*(int*)(find_registry["11111"]));
    for (int i = 0; i < 32 - shamt; i++){
        str_result.push_back(str_rt[i]);
    }
    for (int i = 0; i < shamt; i++){
        str_result = "0" + str_result;
    }
    for (int i = 0; i < shamt; i++){
        str_result[i] = str_ra[32+i-shamt];
    }
    if (str_rt[0] == '1' && (*rt) < 0){
        str_result[0] = '1';
    }
    result = twototen_s(str_result, 32);
    *rd = result;
}

void srav(int* rs, int* rt, int* rd){
    int result;
    string str_result = "";
    string str_rt = tentotwo_thirtytwo(*rt);
    string str_ra = tentotwo_thirtytwo(*(int*)(find_registry["11111"]));
    string str_rs = tentotwo_thirtytwo(*rs);
    int shamt = 0;
    int n = 1;
    for (int i = 31; i >26; i--){
        if (str_rs[i] == '1'){
            shamt += n;
        }
        n *= 2;
    }
    for (int i = 0; i < 32 - shamt; i++){
        str_result.push_back(str_rt[i]);
    }
    for (int i = 0; i < shamt; i++){
        str_result = "0" + str_result;
    }
    for (int i = 0; i < shamt; i++){
        str_result[i] = str_ra[32+i-shamt];
    }
    if (str_rt[0] == '1' && (*rt) < 0){
        str_result[0] = '1';
    }
    result = twototen_s(str_result, 32);
    *rd = result;
}

void srl(int* rd, int*rt, int shamt){
    int result;
    string str_result = "";
    string str_rt = tentotwo_thirtytwo(*rt);
    for (int i = 0; i < 32 - shamt; i++){
        str_result.push_back(str_rt[i]);
    }
    for (int i = 0; i < shamt; i++){
        str_result = "0" + str_result;
    }
    result = twototen_s(str_result, 32);
    *rd = result;
}

void srlv(int* rs, int* rt, int* rd){
    int result;
    string str_result = "";
    string str_rt = tentotwo_thirtytwo(*rt);
    string str_rs = tentotwo_thirtytwo(*rs);
    int shamt = 0;
    int n = 1;
    for (int i = 31; i >26; i--){
        if (str_rs[i] == '1'){
            shamt += n;
        }
        n *= 2;
    }
    for (int i = 0; i < 32 - shamt; i++){
        str_result.push_back(str_rt[i]);
    }
    for (int i = 0; i < shamt; i++){
        str_result = "0" + str_result;
    }
    result = twototen_s(str_result);
    *rd = result;
}

void XOR(int* rs, int* rt, int* rd){
    int result;
    string str_result = "";
    string str_rs = tentotwo_thirtytwo(*rs);
    string str_rt = tentotwo_thirtytwo(*rt);
    for (int i = 0; i < 32; i++){
        if (str_rs[i] != str_rt[i]){
            str_result = str_result + "1";
        }
        else{
            str_result = str_result + "0";
        }
    }
    result = twototen_s(str_result, 32);
    *rd = result;
}

void xori(int* rs, int* rt, int imm){
    int result;
    string str_result = "";
    string str_rs = tentotwo_thirtytwo(*rs);
    string imm_str = tentotwo_thirtytwo(imm);
    for (int i = 0; i < 32; i++){
        if (str_rs[i] != imm_str[i]){
            str_result = str_result + "1";
        }
        else{
            str_result = str_result + "0";
        }
    }
    result = twototen_s(str_result, 32);
    *rt = result;
}

void lui(int* rs, int* rt, string imm){
    int result;
    string str_result = imm;
    for (int i = 0; i< 16; i++){
        str_result += "0";
    }
    result = twototen_s(str_result);
    *rt = result;
}

void slt(int* rs, int* rt, int* rd){
    if (*rs < *rt){
        *rd = 1;
    }
    else{
        *rd = 0;
    }
}

void sltu(int* rs, int* rt, int* rd){
    if ((uint)(*rs) < (uint)(*rt)){
        *rd = 1;
    }
    else{
        *rd = 0;
    }
}

void slti(int* rs, int* rt, int imm){
    if (*rs < imm){
        *rt= 1;
    }
    else{
        *rt = 0;
    }
}

void sltiu(int* rs, int* rt, int imm){
    if ((uint)(*rs) < imm){
        *rt = 1;
    }
    else{
        *rt = 0;
    }
}

void beq(int *rs, int* rt, string offset){
    if (*rs == *rt){
        string reduct = "";
        if (offset[0] == '1'){
            reduct = "11111111111111" + offset + "00";
        }
        else{
            reduct = "00000000000000" + offset + "00";
        }
        int red = twototen_s(reduct, 32);
        pc = red + (pc);
    }
}

void bgez(int *rs, int* rt, string offset){
    if (*rs >= 0){
        string reduct = "";
        if (offset[0] == '1'){
            reduct = "11111111111111" + offset + "00";
        }
        else{
            reduct = "00000000000000" + offset + "00";
        } 
        int red = twototen_s(reduct, 32);
        pc = red + (pc);
    }
}

void bgezal(int *rs, int* rt, string offset){
    if (*rs >= 0){
        string reduct = "";
        if (offset[0] == '1'){
            reduct = "11111111111111" + offset + "00";
        }
        else{
            reduct = "00000000000000" + offset + "00";
        }
        int red = twototen_s(reduct, 32);
        pc = red + (pc + 4);
        *(find_registry["11111"]) = pc + 4;
    }
}

void bgtz(int *rs, int* rt, string offset){
    if (*rs > 0){
        string reduct = "";
        if (offset[0] == '1'){
            reduct = "11111111111111" + offset + "00";
        }
        else{
            reduct = "00000000000000" + offset + "00";
        }
        int red = twototen_s(reduct, 32);
        pc = red + (pc);
    }
}

void blez(int *rs, int* rt, string offset){
    if (*rs <= 0){
        string reduct = "";
        if (offset[0] == '1'){
            reduct = "11111111111111" + offset + "00";
        }
        else{
            reduct = "00000000000000" + offset + "00";
        }
        int red = twototen_s(reduct, 32);
        pc = red + (pc);
    }
}

void bltzal(int *rs, int* rt, string offset){
    if (*rs < 0){
        string reduct = "";
        if (offset[0] == '1'){
            reduct = "11111111111111" + offset + "00";
        }
        else{
            reduct = "00000000000000" + offset + "00";
        } 
        int red = twototen_s(reduct, 32);
        pc = red + (pc);
        *find_registry["11111"] = pc+4;
    }
}

void bltz(int *rs, int* rt, string offset){
    if (*rs < 0){
        string reduct = "";
        if (offset[0] == '1'){
            reduct = "11111111111111" + offset + "00";
        }
        else{
            reduct = "00000000000000" + offset + "00";
        }
        int red = twototen_s(reduct, 32);
        pc = red + (pc);
    }
}

void bne(int *rs, int* rt, string offset){
    if (*rs != *rt){
        string reduct = "";
        if (offset[0] == '1'){
            reduct = "11111111111111" + offset + "00";
        }
        else{
            reduct = "00000000000000" + offset + "00";
        } 
        int red = twototen_s(reduct, 32);
        pc = red + (pc);
        int i = 0;
    }
}

void j(string index){
    string str_pc = "";
    for (int i = 0; i < 4; i++){
        str_pc.push_back(tentotwo_thirtytwo(pc)[i]);
    }
    str_pc += (index+"00");
    pc = twototen_s(str_pc) - 4;
}

void jal(string index){
    string str_pc = "";
    for (int i = 0; i < 4; i++){
        str_pc.push_back(tentotwo_thirtytwo(pc)[i]);
    }
    str_pc += (index+"00");
    *find_registry["11111"] = pc + 4;
    pc = twototen_s(str_pc) - 4;
    
}

void jalr(int* rs, int* rt, int* rd = (int*)(find_registry["11111"])){
    pc = *rs - 4;
    *rd = pc + 12;
}

void jr(int* rs, int* rt, int* rd){
    pc = *rs - 4;
}

void teq(int* rs, int* rt, int* rd){
    if (*rs = *rt){
        throw "Trap";
    }
}

void teqi(int* rs, int* rt, int imm){
    if (*rs == imm){
        throw "Trap";
    }
}

void tne(int* rs, int* rt){
    if (*rs != *rt){
        throw "Trap";
    }
}

void tnei(int* rs, int* rt, int imm){
    if (*rs != imm){
        throw "Trap";
    }
}

void tge(int* rs, int* rt, int* rd){
    if (*rs >= *rt){
        throw "Trap";
    }
}

void tgeu(int* rs, int* rt, int* rd){
    if (calculate_u(*rs) < calculate_u(*rt)){
        throw "Trap";
    }
}

void tgei(int* rs, int* rt, int imm){
    if (*rs >= imm){
        throw "Trap";
    }
}

void tgeiu(int* rs, int* rt, int imm){
    if (calculate_u(*rs) >= imm){
        throw "Trap";
    }
}

void tlt(int* rs, int* rt, int* rd){
    if (*rs < *rt){
        throw "Trap";
    }
}

void tltu(int* rs, int* rt, int* rd){
    if (calculate_u(*rs) < calculate_u(*rt)){
        throw "Trap";
    }
}

void tlti(int* rs, int* rt, int imm){
    if (*rs < imm){
        throw "Trap";
    }
}

void tltiu(int* rs, int* rt, int imm){
    if (calculate_u(*rs) < imm){
        throw "Trap";
    }
}

void lb(int *rs,char *rt, int offset){
    char* ch = (char*)(offset - 0x400000 + real_men + *rs);
    *rt = *ch; 
}

void lbu(int *rs,int8_t *rt, int offset){
    char* ch = (char*)(offset - 0x400000 + real_men + *rs);
    int8_t value = *ch;
    *rt = value; 
}

void lh(int *rs, int16_t *rt, int offset){
    int16_t* half = (int16_t*)(offset - 0x400000 + real_men + *rs);
    *rt = *half; 
}

void lhu(int *rs,int16_t*rt, int offset){
    int16_t* half = (int16_t*)(offset - 0x400000 + real_men + *rs);
    *rt = *half; 
}

void lw(int *rs,int *rt, int offset){
    int* value = (int*)(offset - 0x400000 + real_men + *rs);
    *rt = *value; 
}

void lwl(int *rs, int *rt, int offset){
    int address = (offset + *rs);
    int remainder = address % 4;
    int load = *(int*)(address - 0x400000 + real_men - remainder + *rs);
    string load_str = tentotwo_thirtytwo(load);
    string rt_str = tentotwo_thirtytwo(*rt);
    string result = "";
    for (int i = 0; i < 8*remainder; i++){
        result.push_back(rt_str[i]);
    }
    for (int i = 8 * remainder; i < 32; i++){
        result.push_back(load_str[i]);
    }
    *rt = twototen_s(result, 32);
}

void lwr(int *rs, int *rt, int offset){
    int address = (offset + *rs);
    int remainder = address % 4;
    int load = *(int*)(address - 0x400000 + real_men - remainder + *rs);
    string load_str = tentotwo_thirtytwo(load);
    string rt_str = tentotwo_thirtytwo(*rt);
    string result = "";
    for (int i = 0; i < 8*(remainder+1); i++){
        result.push_back(rt_str[i]);
    }
    for (int i = 0; i < 32-(remainder+1)*8; i++){
        result.push_back(load_str[i]);
    }
    *rt = twototen_s(result, 32);
}

void ll(int *rs, int* rt, int offset){
    lw(rs, rt, offset);
}

void sw(int *rs, int* rt, int offset){
    int address_i = offset + *rs;
    int remainder = address_i % 4;
    int* address = (int*)(offset - 0x400000 + real_men + *rs + -remainder);
    *address = *rt; 
}

void sc(int *rs, int* rt, int offset){
    int address_i = offset + *rs;
    int remainder = address_i % 4;
    int* address = (int*)(offset - 0x400000 + real_men + *rs + (4-remainder));
    *address = *rt; 
}

void sh(int *rs, int16_t* rt, int offset){
    int address_i = offset + *rs;
    int remainder = address_i % 2;
    int16_t* address = (int16_t*)(offset - 0x400000 + real_men + *rs + remainder);
    *address = *rt;
}

void sb(int *rs, int8_t* rt, int offset){
    char* address = (char*)(offset - 0x400000 + real_men + *rs);
    char ch;
    ch = *rt;
    *address = ch;
}

void swl(int *rs, int* rt, int offset){
    int* address = (int*)(offset - 0x400000 + real_men - 3 + *rs);
    *address = *rt; 
}

void swr(int *rs, int* rt, int offset){
    int* address = (int*)(offset - 0x400000 + real_men + *rs);
    *address = *rt; 
}

void mfhi(int* rs, int* rt, int* rd){
    *rd = *HI;
}

void mflo(int* rs, int* rt, int* rd){
    *rd = *LO;
}

void mthi(int* rs, int* rt, int* rd){
    *HI = *rs;
}

void mtlo(int* rs, int* rt, int* rd){
    *LO = calculate_u(*rs);
}

void syscall(istream& is, ostream& os){
    int switches = *(int*)(find_registry["00010"]);
    if (switches == 1){
        int hi = *(int*)(find_registry["00100"]);
        os << hi;
    }
    else if (switches == 4){
        int si_address = *(int*)(find_registry["00100"]);
        char ch = *(char*)(si_address + real_men - 0x400000);
        string out = "";
        while (ch != '\0'){
            out.push_back(ch);
            si_address += 1;
            ch = *(char*)(si_address + real_men - 0x400000);
        }
        os << out;
        
    }
    else if (switches == 5){
        int ins;
        string input;
        getline(is, input);
        ins = stoi(input, nullptr, 0);
        *(int*)(find_registry["00010"]) = ins;
    }
    else if (switches == 8){
        string read_str;
        int length = *(int*)(find_registry["00101"]);
        getline(is, read_str);
        char *ch = (char*)(*find_registry["00100"] + real_men - 0x400000);
        for (int i = 0; i < length; i++){
            *ch = read_str[i];
            ch += 1;
        }
        *ch = '\0';
    }
    else if (switches == 9){
        *find_registry["00010"] = *find_registry["11110"]; 
        *find_registry["11110"] = *find_registry["11110"] + *find_registry["00100"];
    }
    else if (switches == 10){
        pc = 0xa00000 - 4;
    }
    else if (switches == 11){
        os << *(char*)(find_registry["00100"]);
    }
    else if (switches == 12){
        string input;
        getline(is, input);
        char ch = input[0];
        *(char*)(find_registry["00010"]) = ch;
    }
    else if (switches == 13){
        string mode = "";
        string filename = "";
        int n = 0;
        char* a0 = (char*)(*find_registry["00100"] + real_men - 0x400000);
        char* a2 = (char*)(*find_registry["00110"] + real_men - 0x400000);
        while (*a0 != 0){
            filename += *(a0 + n);
            n += 1;
        }
        n = 0;
        while (*a2 != 0){
            mode += *(a2 + n);
            n += 1;
        }

        int f = open(filename.c_str(),*(char*)(find_registry["00101"]), mode.c_str());
        *(int*)(find_registry["00010"]) = f;
    }
    else if (switches == 14){
        int fd = *(int*)find_registry["00100"];
        char* buffer = real_men + *(int*)(find_registry["00101"]) - 0x400000;
        int length = *(int*)find_registry["00110"];
        int num = int(read(fd, buffer, length));
        *(int*)find_registry["00010"] = num;
    }
    else if (switches == 15){
        int fd = *(int*)find_registry["00100"];
        char* buffer = real_men + *(int*)(find_registry["00101"]) - 0x400000;
        int length = *(int*)find_registry["00110"];
        int num = int(write(fd, buffer, length));
        *(int*)find_registry["00010"] = num;
    }
    else if (switches == 16){
        close(*(int*)(find_registry["00100"]));
    }
    else if (switches == 17){
        exit(*(int*)(find_registry["00100"]));
    }
}


void read_excute(istream& is, ostream &os){
    string code;
    int code_int = *(int*)(real_men + pc - 0x400000);
    while (code_int != 0){
        code = tentotwo_thirtytwo(code_int);
        string op = getop(code);
        if (op == "000000"){
            int *rs = (int*)(find_registry[getsegment(code, 7, 5)]);
            int *rt = (int*)(find_registry[getsegment(code, 12, 5)]);
            int *rd = (int*)(find_registry[getsegment(code, 17, 5)]);     // destinate register;
            string shamt = getsegment(code, 22, 5);
            string funct = getsegment(code, 27, 6);
            if (funct == "100000"){                 //add
                add(rs, rt, rd);
            }
            else if (funct == "100001"){            //addu
                addu(rs, rt, rd);
            }
            else if (funct == "100010"){            //sub
                sub(rs, rt, rd);
            }
            else if (funct == "100100"){            //and
                AND(rs, rt, rd);
            }
            else if (funct == "011010"){            //div
                div(rs, rt);
            }
            else if (funct == "011011"){            //divu
                divu(rs, rt);
            }
            else if (funct == "011000"){            //mult
                mult(rs, rt);
            }
            else if (funct == "011001"){            //multu
                multu(rs, rt);
            }
            else if (funct == "100111"){            //nor
                nor(rs, rt, rd);
            }
            else if (funct == "100101"){            //or
                OR(rs, rt, rd);
            }
            else if (funct == "000000"){            //sll
                sll(rd, rt, twototen_s(shamt));
            }
            else if (funct == "000100"){            //sllv
                sllv(rs, rt, rd);
            }
            else if (funct == "000011"){            //sra
                sra(rs, rt, rd, twototen_s(shamt));
            }
            else if (funct == "000111"){            //srav
                srav(rs, rt, rd);
            }
            else if (funct == "000010"){            //srl
                srl(rd, rt, twototen_s(shamt));
            }
            else if (funct == "000110"){            //srlv
                srlv(rs, rt, rd);
            }
            else if (funct == "100011"){            //subu
                subu(rs, rt, rd);
            }
            else if (funct == "101010"){            //slt
                slt(rs, rt, rd);
            }
            else if (funct == "101011"){            //sltu
                sltu(rs, rt, rd);
            }
            else if (funct == "001001"){            //jalr
                jalr(rs, rt, rd);
            }
            else if (funct == "001000"){            //jr
                jr(rs, rt, rd);
            }
            else if (funct == "110100"){            //teq
                teq(rs, rt, rd);
            }
            else if (funct == "110000"){            //tge
                tge(rs, rt, rd);
            }
            else if (funct == "110010"){            //tlt
                tlt(rs, rt, rd);
            }
            else if (funct == "110011"){            //tltu
                tltu(rs, rt, rd);
            }
            else if (funct == "010000"){            //mfhi
                mfhi(rs, rt, rd);
            }
            else if (funct == "010010"){            //mflo
                mflo(rs, rt, rd);
            }
            else if (funct == "010001"){            //mthi
                mthi(rs, rt, rd);
            }
            else if (funct == "010011"){            //mtlo
                mtlo(rs, rt, rd);
            }
            else if (funct == "100110"){            //XOR
                XOR(rs, rt, rd);
            }
            else if (funct == "001100"){            //syscall
                syscall(is, os);
            }
        }
        else{
            if (op == "001000"){               // addi
                int *rs = (int*)(find_registry[getsegment(code, 7, 5)]);
                int *rt = (int*)(find_registry[getsegment(code, 12, 5)]);
                string str_imm = getsegment(code, 17, 16);
                int imm = twototen_s(str_imm);
                addi(rs, rt, imm);
            }
            else if (op == "001001"){              // addiu
                int *rs = (int*)(find_registry[getsegment(code, 7, 5)]);
                int *rt = (int*)(find_registry[getsegment(code, 12, 5)]);
                int imm = twototen_s(getsegment(code, 17, 16));
                addiu(rs, rt, imm);
            }
            else if (op == "001100"){              //andi
                int *rs = (int*)(find_registry[getsegment(code, 7, 5)]);
                int *rt = (int*)(find_registry[getsegment(code, 12, 5)]);
                int imm = twototen_s(getsegment(code, 17, 16));
                andi(rs, rt, imm);
            }
            else if (op == "011100"){              //clo clz
                int *rs = (int*)(find_registry[getsegment(code, 7, 5)]);
                int *rt = (int*)(find_registry[getsegment(code, 12, 5)]);
                int imm = twototen_s(getsegment(code, 17, 16));
                string af = getsegment(code, 27, 6);
                if (af == "100001"){                //clo
                    int *rd = (int*)(find_registry[getsegment(code, 17, 5)]);
                    clo(rs, rd);
                }
                else if (af == "100000"){           //clz
                    int *rd = (int*)(find_registry[getsegment(code, 17, 5)]);
                    clz(rs, rd);
                }
                else if (af == "000010"){           //mul
                    int *rd = (int*)(find_registry[getsegment(code, 17, 5)]);
                    mul(rs, rt, rd);
                }
                else if (af == "000000"){           //madd
                    madd(rs, rt);
                }
                else if (af == "000001"){           //maddu
                    maddu(rs, rt);
                }
                else if (af == "000100"){           //msub
                    msub(rs, rt);
                }
                else if (af == "000101"){           //msubu
                    msubu(rs, rt);
                }
            }
            else if (op == "001101"){              //ori
                int *rs = (int*)(find_registry[getsegment(code, 7, 5)]);
                int *rt = (int*)(find_registry[getsegment(code, 12, 5)]);
                int imm = twototen_s(getsegment(code, 17, 16));
                ori(rs, rt, imm);
            }
            else if (op == "001110"){              //xori
                int *rs = (int*)(find_registry[getsegment(code, 7, 5)]);
                int *rt = (int*)(find_registry[getsegment(code, 12, 5)]);
                int imm = twototen_s(getsegment(code, 17, 16));
                xori(rs, rt, imm);
            }
            else if (op == "001111"){              //lui
                int *rs = (int*)(find_registry[getsegment(code, 7, 5)]);
                int *rt = (int*)(find_registry[getsegment(code, 12, 5)]);
                string imm = getsegment(code, 17, 16);
                lui(rs, rt, imm);
            }
            else if (op == "001010"){              //slti
                int *rs = (int*)(find_registry[getsegment(code, 7, 5)]);
                int *rt = (int*)(find_registry[getsegment(code, 12, 5)]);
                int imm = twototen_s(getsegment(code, 17, 16));
                slti(rs, rt, imm);
            }
            else if (op == "001011"){              //sltiu
                int *rs = (int*)(find_registry[getsegment(code, 7, 5)]);
                int *rt = (int*)(find_registry[getsegment(code, 12, 5)]);
                int imm = twototen_s(getsegment(code, 17, 16));
                sltiu(rs, rt, imm);
            }
            else if (op == "000100"){              //beq
                int *rs = (int*)(find_registry[getsegment(code, 7, 5)]);
                int *rt = (int*)(find_registry[getsegment(code, 12, 5)]);
                string offset = getsegment(code, 17, 16);
                beq(rs, rt, offset);
            }
            else if (op == "000001"){              
                int *rs = (int*)(find_registry[getsegment(code, 7, 5)]);
                int *rt = (int*)(find_registry[getsegment(code, 12, 5)]);
                string offset = getsegment(code, 17, 16);
                if (*rt == 1){                      //bgez
                    bgez(rs, rt, offset);
                }
                else if (*rt == 0x11){              //bgezal
                    bgezal(rs, rt, offset);
                }
                else if (*rt == 0x10){              //bltzal
                    bltzal(rs, rt, offset);
                }
                else if (*rt == 0){              //bltz
                    bltz(rs, rt, offset);
                }
                else if (*rt == 8){                 //tgei
                    int imm = twototen_s(getsegment(code, 17, 16));
                    tgei(rs, rt, imm);
                }
                else if (*rt == 9){                 //tgeiu
                    int imm = twototen_s(getsegment(code, 17, 16));
                    tgeiu(rs, rt, imm);
                }
                else if (*rt == 10){                 //tlti
                    int imm = twototen_s(getsegment(code, 17, 16));
                    tlti(rs, rt, imm);
                }
                else if (*rt == 11){                 //tltiu
                    int imm = twototen_s(getsegment(code, 17, 16));
                    tltiu(rs, rt, imm);
                }
                else if (*rt == 14){             //tne
                    tne(rs, rt);
                }
                else if (*rt == 12){         //teqi
                    int imm = twototen_s(getsegment(code, 17, 16));
                    teqi(rs, rt, imm);
                }
            }
            else if (op == "000111"){              //bgtz
                int *rs = (int*)(find_registry[getsegment(code, 7, 5)]);
                int *rt = (int*)(find_registry[getsegment(code, 12, 5)]);
                string offset = getsegment(code, 17, 16);
                bgtz(rs, rt, offset);
            }
            else if (op == "000110"){              //blez
                int *rs = (int*)(find_registry[getsegment(code, 7, 5)]);
                int *rt = (int*)(find_registry[getsegment(code, 12, 5)]);
                string offset = getsegment(code, 17, 16);
                blez(rs, rt, offset);
            }
            else if (op == "000101"){              //bne
                int *rs = (int*)(find_registry[getsegment(code, 7, 5)]);
                int *rt = (int*)(find_registry[getsegment(code, 12, 5)]);
                string offset = getsegment(code, 17, 16);
                bne(rs, rt, offset);
            }
            else if (op == "000010"){              //j
                string target = getsegment(code, 7, 26);
                j(target);
            }
            else if (op == "000011"){              //jal
                string target = getsegment(code, 7, 26);
                jal(target);
            }
            else if (op == "100000"){              //lb
                int *rs = (int*)(find_registry[getsegment(code, 7, 5)]);
                char *rt = (char*)(find_registry[getsegment(code, 12, 5)]);
                int imm = twototen_s(getsegment(code, 17, 16));
                lb(rs, rt, imm);
            }
            else if (op == "100100"){              //lbu
                int *rs = (int*)(find_registry[getsegment(code, 7, 5)]);
                int8_t *rt = (int8_t*)(find_registry[getsegment(code, 12, 5)]);
                int imm = twototen_s(getsegment(code, 17, 16));
                lbu(rs, rt, imm);
            }
            else if (op == "100001"){              //lh
                int *rs = (int*)(find_registry[getsegment(code, 7, 5)]);
                int16_t *rt = (int16_t*)(find_registry[getsegment(code, 12, 5)]);
                int imm = twototen_s(getsegment(code, 17, 16));
                lh(rs, rt, imm);
            }
            else if (op == "100101"){              //lhu
                int *rs = (int*)(find_registry[getsegment(code, 7, 5)]);
                int16_t *rt = (int16_t*)(find_registry[getsegment(code, 12, 5)]);
                int imm = twototen_s(getsegment(code, 17, 16));
                lhu(rs, rt, imm);
            }
            else if (op == "100011"){              //lw
                int *rs = (int*)(find_registry[getsegment(code, 7, 5)]);
                int *rt = (int*)(find_registry[getsegment(code, 12, 5)]);
                int imm = twototen_s(getsegment(code, 17, 16));
                lw(rs, rt, imm);
            }
            else if (op == "101011"){              //sw
                int *rs = (int*)(find_registry[getsegment(code, 7, 5)]);
                int *rt = (int*)(find_registry[getsegment(code, 12, 5)]);
                int imm = twototen_s(getsegment(code, 17, 16));
                sw(rs, rt, imm);
            }
            else if (op == "101001"){              //sh
                int *rs = (int*)(find_registry[getsegment(code, 7, 5)]);
                int16_t *rt = (int16_t*)(find_registry[getsegment(code, 12, 5)]);
                int imm = twototen_s(getsegment(code, 17, 16));
                sh(rs, rt, imm);
            }
            else if (op == "101000"){              //sb
                int *rs = (int*)(find_registry[getsegment(code, 7, 5)]);
                int8_t *rt = (int8_t*)(find_registry[getsegment(code, 12, 5)]);
                int imm = twototen_s(getsegment(code, 17, 16));
                sb(rs, rt, imm);
            }
            else if (op == "100010") {              //lwl
                int *rs = (int*)(find_registry[getsegment(code, 7, 5)]);
                int *rt = (int*)(find_registry[getsegment(code, 12, 5)]);
                int imm = twototen_s(getsegment(code, 17, 16));
                lwl(rs, rt, imm);
            }
            else if (op == "100110") {              //lwr
                int *rs = (int*)(find_registry[getsegment(code, 7, 5)]);
                int *rt = (int*)(find_registry[getsegment(code, 12, 5)]);
                int imm = twototen_s(getsegment(code, 17, 16));
                lwr(rs, rt, imm);
            }
            else if (op == "110000"){               //ll
                int *rs = (int*)(find_registry[getsegment(code, 7, 5)]);
                int *rt = (int*)(find_registry[getsegment(code, 12, 5)]);
                int imm = twototen_s(getsegment(code, 17, 16));
                ll(rs, rt, imm);
            }
            else if (op == "101010"){               //swl
                int *rs = (int*)(find_registry[getsegment(code, 7, 5)]);
                int *rt = (int*)(find_registry[getsegment(code, 12, 5)]);
                int imm = twototen_s(getsegment(code, 17, 16));
                swl(rs, rt, imm);
            }
            else if (op == "101110"){               //swr
                int *rs = (int*)(find_registry[getsegment(code, 7, 5)]);
                int *rt = (int*)(find_registry[getsegment(code, 12, 5)]);
                int imm = twototen_s(getsegment(code, 17, 16));
                swr(rs, rt, imm);
            }
            else if (op == "111000"){              //sc
                int *rs = (int*)(find_registry[getsegment(code, 7, 5)]);
                int *rt = (int*)(find_registry[getsegment(code, 12, 5)]);
                int imm = twototen_s(getsegment(code, 17, 16));
                sc(rs, rt, imm);
            }
        }
        pc += 4;
        code_int = *(int*)(real_men + pc - 0x400000);
    }
}

void memory_simulation(istream& is, ostream& os){
    memory_init();                   // initialize a 6MB memory  
    so_registry(); 
    put_data();
    put_code();
    read_excute(is, os);
}

void deletion(){
    delete real_men;
    delete HI;
    delete LO;
    delete find_registry["00000"];
    delete find_registry["00001"];
    delete find_registry["00010"];
    delete find_registry["00011"];
    delete find_registry["00100"];
    delete find_registry["00101"];
    delete find_registry["00110"];
    delete find_registry["00111"];
    delete find_registry["01000"];
    delete find_registry["01001"];
    delete find_registry["01010"];
    delete find_registry["01011"];
    delete find_registry["01100"];
    delete find_registry["01101"];
    delete find_registry["01110"];
    delete find_registry["01111"];
    delete find_registry["10000"];
    delete find_registry["10001"];
    delete find_registry["10010"];
    delete find_registry["10011"];
    delete find_registry["10100"];
    delete find_registry["10101"];
    delete find_registry["10110"];
    delete find_registry["10111"];
    delete find_registry["11000"];
    delete find_registry["11001"];
    delete find_registry["11010"];
    delete find_registry["11011"];
    delete find_registry["11100"];
    delete find_registry["11101"];
    delete find_registry["11110"];
    delete find_registry["11111"];
}

int main(int argc, char** argv){
    store_registers();
    ifstream infile1;
    ifstream infile2;
    ifstream infile3;
    ofstream outfile1;
    ofstream outfile2;
    ofstream data_file;
    infile1.open(argv[1]);
    infile3.open(argv[2]);
    outfile2.open(argv[3]);
    outfile1.open("a_put.txt",ios::out);
    data_file.open("data_put.txt");
    removeComments(infile1, outfile1, data_file);
    outfile1.close();
    data_file.close();
    infile1.close();
    transfer();  
    memory_simulation(infile3, outfile2);
    infile3.close();
    outfile2.close();
    deletion();
    return 0;
} 