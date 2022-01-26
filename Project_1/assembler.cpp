#include <stdio.h>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <sstream>
#include "assembler.h"
using namespace std;

map<string, int> label_map;
map<string, string> registers;
map<string, int> var_map;
int address = 0x400000 - 4;


void store_registers(){
    registers["$zero"] = "00000";
    registers["$at"] = "00001";
    registers["$v0"] = "00010";
    registers["$v1"] = "00011";
    registers["$a0"] = "00100";
    registers["$a1"] = "00101";
    registers["$a2"] = "00110";
    registers["$a3"] = "00111";
    registers["$t0"] = "01000";
    registers["$t1"] = "01001";
    registers["$t2"] = "01010";
    registers["$t3"] = "01011";
    registers["$t4"] = "01100";
    registers["$t5"] = "01101";
    registers["$t6"] = "01110";
    registers["$t7"] = "01111";
    registers["$s0"] = "10000";
    registers["$s1"] = "10001";
    registers["$s2"] = "10010";
    registers["$s3"] = "10011";
    registers["$s4"] = "10100";
    registers["$s5"] = "10101";
    registers["$s6"] = "10110";
    registers["$s7"] = "10111";
    registers["$t8"] = "11000";
    registers["$t9"] = "11001";
    registers["$k0"] = "11010";
    registers["$k1"] = "11011";
    registers["$gp"] = "11100";
    registers["$sp"] = "11101";
    registers["$fp"] = "11110";
    registers["$ra"] = "11111";
}

string tentotwo_thirtyone(string s){
    int ten_number;
    stringstream stream;
    stream << s;
    stream >> ten_number;
    int remainder;
    string result = "";
    if (ten_number == 0){
        return "0000000000000000000000000000000";
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
    while (result.length() != 31){
        result = "0" + result;
    }
    return result;
}

string a_twoscomplement(string neg_value){
    string onesform = "";
    string posi_neg_value = "";
    string twosform = "";
    if (neg_value == "-1"){
        return "11111111111111111111111111111111";
    }
    for (int n = 1; n < neg_value.length(); n++){
        posi_neg_value.push_back(neg_value[n]);
    }
    string first = tentotwo_thirtyone(posi_neg_value);
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
    for (int i = 31; i >= 0; i--){
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

string tentotwo(string s){
    string result = "";
    if (s[0] != '-'){
        int ten_number;
        stringstream stream;
        stream << s;
        stream >> ten_number;
        int remainder;
        if (ten_number == 0){
            return "0000000000000000";
        }
        while (ten_number != 1){
            remainder = ten_number % 2;
            result = to_string(remainder) + result;
            ten_number = ten_number/2; 
        }
        result = "1" + result;
        while (result.length() != 16){
            result = "0" + result;
        }

    }
    else{
        string re = a_twoscomplement(s);
        for (int i = 14; i >= 0; i--){
            result.push_back(re[31-i]);
        }
        result = "1" + result;
    }
    return result;
}


string tentotwo_eightteen(string s){
    int ten_number;
    stringstream stream;
    stream << s;
    stream >> ten_number;
    int remainder;
    string result = "";
    if (ten_number == 0){
        return "000000000000000000";
    }
    while (ten_number != 1){
        remainder = ten_number % 2;
        result = to_string(remainder) + result;
        ten_number = ten_number/2; 
    }
    result = "1" + result;
    while (result.length() != 18){
        result = "0" + result;
    }
    return result;
}

string a_tentotwo_thirtytwo(string s){
    int ten_number;
    stringstream stream;
    stream << s;
    stream >> ten_number;
    int remainder;
    string result = "";
    if (ten_number == 0){
        return "00000000000000000000000000000000";
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

string tentotwo_shamt(string s){
    int ten_number;
    stringstream stream;
    stream << s;
    stream >> ten_number;
    int remainder;
    string result = "";
    if (ten_number == 0){
        return "00000";
    }
    else if (ten_number == 1){
        return "00001";
    }
    else if (ten_number == -1){
        return "11111";
    }
    else if (ten_number == -2){
        return "11110";
    }
    else if (ten_number == -3){
        return "11101";
    }
    else if (ten_number == -4){
        return "11100";
    }
    else if (ten_number == -5){
        return "11011";
    }
    else if (ten_number == -6){
        return "11010";
    }
    else if (ten_number == -7){
        return "11001";
    }
    else if (ten_number == -8){
        return "11000";
    }
    else if (ten_number == -9){
        return "10111";
    }
    else if (ten_number == -10){
        return "10110";
    }
    else if (ten_number == -11){
        return "10101";
    }
    else if (ten_number == -12){
        return "10100";
    }
    else if (ten_number == -13){
        return "10011";
    }
    else if (ten_number == -14){
        return "10010";
    }
    else if (ten_number == -15){
        return "10001";
    }
    while (ten_number != 1){
        remainder = ten_number % 2;
        result = to_string(remainder) + result;
        ten_number = ten_number/2; 
    }
    result = "1" + result;
    while (result.length() != 5){
        result = "0" + result;
    }
    return result;
}

string remove_space(string line){
    int n = 0;
    string new_line = "";
    while (line[n] == ' '){
        n += 1;
    }
    for (int i = n; i < line.length(); i++){
        new_line += line[i];
    }
    return new_line;
}

bool line_empty(string line){
    if (line[0] == '\n'){
        return true;
    }
    for (int i = 0; i<line.length(); i++){
        if (line[i] != ' '){
            return false;
        }
    }
    return true;
}

void removeComments(istream & is, ostream & os, ostream & dataos){
    char character;
    bool skipline = false;
    bool remove_whole_line = true;
    bool data_end = false;
    string line = ""; 
    int label_address = 0x400000;
    while (is.get(character)){
        if (character == '#'){
            skipline = true;
        }
        else if (character == '\n' || character == EOF || character == '\t'){
            if (line == ".text" || line == ".data"){
                remove_whole_line = true;
                if (line == ".text"){
                    data_end = true;
                }
            }
            if (!remove_whole_line){
                line.push_back(character);
                line = remove_space(line);
                if (data_end && !line_empty(line)){
                    os << line;
                    label_address += 4;
                }
                else{
                    dataos << line;
                }
            }
            remove_whole_line = true;
            skipline = false;
            line = "";
        }
        else if (!skipline){
            if (character == ':'){
                if (data_end){
                    label_map[line] = label_address;
                    line = "";
                }
                else{
                    var_map[line] = label_address;
                    line.push_back(':');
                }
                
            }
            else{
                if (line != "" || character != ' '){
                    line.push_back(character);
                }
            }
            if (character != ' '){
                remove_whole_line = false;
            }
        }
    }
    if (character != '\n'){
        line = remove_space(line);
        if (data_end){
            os << line;
        }
        else{
            dataos << line;
        }
    }
    
}

string find_instruction(istream & is, char first){
    char character;
    string instruction = "";
    instruction.push_back(first);
    while(is.get(character)){
        if (character == ' ' || character == '\n'){
            break;
        }
        instruction += character;
    }
    return instruction;
}

string find_segment(istream & is){
    char character;
    string segment = "";
    while (is.get(character)){
        if (character != ' '){
            segment.push_back(character);
            break;
        }
    }
    while(is.get(character)){
        if (character == ','){
            break;
        }
        else if (character == ' ' && segment != ""){
            break;
        }
        else if (character == '\n'){
            return segment;
        }
        else if (character != ' '){
            segment += character;
        }
        
    }
    return segment;
}

string offset(string lable){
    int offset_int = label_map[lable] - (address + 4);
    string offset_string = "";
    if (offset_int > 0){
        string pre = tentotwo_eightteen(to_string(offset_int));
        for (int i = 0; i < 16; i++){
            offset_string.push_back(pre[i]);
        }
        return offset_string;
    }
    else if (offset_int == 0){
        return "0000000000000000";
    }
    else{
        string twosform = a_twoscomplement(to_string(offset_int));
        string result = "";
        for (int i = 14; i < 30; i++){
            result += twosform[i];
        }
        return result;
    }
}

string calculate_target(string lable){
    string target_thirtytwo = a_tentotwo_thirtytwo(to_string(label_map[lable]));
    string result = "";
    for (int i = 4; i < 30; i++){
        result.push_back(target_thirtytwo[i]);
    }
    return result;
}

void translate(istream & is, ostream & os){
    char character;
    string instruction;
    while (is.get(character)){
        
        instruction = "";
        string code = "";
        if (character == ' ' || character == '\n'){
            int i = 0;
        }
        else{
            instruction = find_instruction(is, character);
            address += 4;
        }
        if (instruction == "add"){
            code += "000000";
            string rd = find_segment(is);
            string rs = find_segment(is);
            string rt = find_segment(is);
            code = code + registers[rs] + registers[rt] + registers[rd] + "00000" + "100000";
            os << code << endl;
        }
        else if (instruction == "addu"){
            code += "000000";
            string rd = find_segment(is);
            string rs = find_segment(is);
            string rt = find_segment(is);
            code = code + registers[rs] + registers[rt] + registers[rd] + "00000" + "100001";
            os << code << endl;
        }
        else if (instruction == "sub"){
            code += "000000";
            string rd = find_segment(is);
            string rs = find_segment(is);
            string rt = find_segment(is);
            code = code + registers[rs] + registers[rt] + registers[rd] + "00000" + "100010";
            os << code << endl;
        }
        else if (instruction == "subu"){
            code += "000000";
            string rd = find_segment(is);
            string rs = find_segment(is);
            string rt = find_segment(is);
            code = code + registers[rs] + registers[rt] + registers[rd] + "00000" + "100011";
            os << code << endl;
        }
        else if (instruction == "addi"){
            code += "001000";
            string rt = find_segment(is);
            string rs = find_segment(is);
            string imm_str = find_segment(is);
            string imm = tentotwo(imm_str);
            code = code + registers[rs] + registers[rt] + imm;
            os << code << endl;
        }
        else if (instruction == "addiu"){
            code += "001001";
            string rt = find_segment(is);
            string rs = find_segment(is);
            string imm_str = find_segment(is);
            string imm = tentotwo(imm_str);
            code = code + registers[rs] + registers[rt] + imm;
            os << code << endl;
        }
        else if (instruction == "and"){
            code += "000000";
            string rd = find_segment(is);
            string rs = find_segment(is);
            string rt = find_segment(is);
            code = code + registers[rs] + registers[rt] + registers[rd] + "00000" + "100100";
            os << code << endl;
        }
        else if (instruction == "andi"){
            code += "001100";
            string rt = find_segment(is);
            string rs = find_segment(is);
            string imm_str = find_segment(is);
            string imm = tentotwo(imm_str);
            code = code + registers[rs] + registers[rt] + imm;
            os << code << endl;
        }
        else if (instruction == "clo"){
            code += "011100";
            string rd = find_segment(is);
            string rs = find_segment(is);
            code = code + registers[rs] + "00000" + registers[rd] + "00000" + "100001";
            os << code << endl;
        }
        else if (instruction == "clz"){
            code += "011100";
            string rd = find_segment(is);
            string rs = find_segment(is);
            code = code + registers[rs] + "00000" + registers[rd] + "00000" + "100000";
            os << code << endl;
        }
        else if (instruction == "div"){
            code += "000000";
            string rs = find_segment(is);
            string rt = find_segment(is);
            code = code + registers[rs] + registers[rt] + "0000000000" + "011010";
            os << code << endl;
        }
        else if (instruction == "divu"){
            code += "000000";
            string rs = find_segment(is);
            string rt = find_segment(is);
            code = code + registers[rs] + registers[rt] + "0000000000" + "011011";
            os << code << endl;
        }
        else if (instruction == "mult"){
            code += "000000";
            string rs = find_segment(is);
            string rt = find_segment(is);
            code = code + registers[rs] + registers[rt] + "0000000000" + "011000";
            os << code << endl;
        }
        else if (instruction == "multu"){
            code += "000000";
            string rs = find_segment(is);
            string rt = find_segment(is);
            code = code + registers[rs] + registers[rt] + "0000000000" + "011001";
            os << code << endl;
        }
        else if (instruction == "mul"){
            code += "011100";
            string rd = find_segment(is);
            string rs = find_segment(is);
            string rt = find_segment(is);
            code = code + registers[rs] + registers[rt] + registers[rd] + "00000" + "000010";
            os << code << endl;
        }
        else if (instruction == "madd"){
            code += "011100";
            string rs = find_segment(is);
            string rt = find_segment(is);
            code = code + registers[rs] + registers[rt] + "0000000000" + "000000";
            os << code << endl;
        }
        else if (instruction == "maddu"){
            code += "011100";
            string rs = find_segment(is);
            string rt = find_segment(is);
            code = code + registers[rs] + registers[rt] + "0000000000" + "000001";
            os << code << endl;
        }
        else if (instruction == "msub"){
            code += "011100";
            string rs = find_segment(is);
            string rt = find_segment(is);
            code = code + registers[rs] + registers[rt] + "0000000000" + "000100";
            os << code << endl;
        }
        else if (instruction == "msubu"){
            code += "011100";
            string rs = find_segment(is);
            string rt = find_segment(is);
            code = code + registers[rs] + registers[rt] + "0000000000" + "000101";
            os << code << endl;
        }
        else if (instruction == "nor"){
            code += "000000";
            string rd = find_segment(is);
            string rs = find_segment(is);
            string rt = find_segment(is);
            code = code + registers[rs] + registers[rt] + registers[rd] + "00000" + "100111";
            os << code << endl;
        }
        else if (instruction == "or"){
            code += "000000";
            string rd = find_segment(is);
            string rs = find_segment(is);
            string rt = find_segment(is);
            code = code + registers[rs] + registers[rt] + registers[rd] + "00000" + "100101";
            os << code << endl;
        }
        else if (instruction == "ori"){
            code += "001101";
            string rt = find_segment(is);
            string rs = find_segment(is);
            string imm_str = find_segment(is);
            string imm = tentotwo(imm_str);
            code = code + registers[rs] + registers[rt] + imm;
            os << code << endl;
        }
        else if (instruction == "sll"){
            code += "000000";
            string rd = find_segment(is);
            string rt = find_segment(is);
            string shamt_str = find_segment(is);
            string shamt = tentotwo_shamt(shamt_str);
            code = code + "00000" + registers[rt] + registers[rd] + shamt + "000000";
            os << code << endl;
        }
        else if (instruction == "sllv"){
            code += "000000";
            string rd = find_segment(is);
            string rt = find_segment(is);
            string rs = find_segment(is);
            code = code + registers[rs] + registers[rt] + registers[rd] + "00000" + "000100";
            os << code << endl;
        }
        else if (instruction == "sra"){
            code += "000000";
            string rd = find_segment(is);
            string rt = find_segment(is);
            string shamt_str = find_segment(is);
            string shamt = tentotwo_shamt(shamt_str);
            code = code + "00000" + registers[rt] + registers[rd] + shamt + "000011";
            os << code << endl;
        }
        else if (instruction == "srav"){
            code += "000000";
            string rd = find_segment(is);
            string rt = find_segment(is);
            string rs = find_segment(is);
            code = code + registers[rs] + registers[rt] + registers[rd] + "00000" + "000111";
            os << code << endl;
        }
        else if (instruction == "srl"){
            code += "000000";
            string rd = find_segment(is);
            string rt = find_segment(is);
            string shamt_str = find_segment(is);
            string shamt = tentotwo_shamt(shamt_str);
            code = code + "00000" + registers[rt] + registers[rd] + shamt + "000010";
            os << code << endl;
        }
        else if (instruction == "srlv"){
            code += "000000";
            string rd = find_segment(is);
            string rt = find_segment(is);
            string rs = find_segment(is);
            code = code + registers[rs] + registers[rt] + registers[rd] + "00000" + "000110";
            os << code << endl;
        }
        else if (instruction == "xor"){
            code += "000000";
            string rd = find_segment(is);
            string rs = find_segment(is);
            string rt = find_segment(is);
            code = code + registers[rs] + registers[rt] + registers[rd] + "00000" + "100110";
            os << code << endl;
        }
        else if (instruction == "xori"){
            code += "001110";
            string rt = find_segment(is);
            string rs = find_segment(is);
            string imm_str = find_segment(is);
            string imm = tentotwo(imm_str);
            code = code + registers[rs] + registers[rt] + imm;
            os << code << endl;
        }
        else if (instruction == "lui"){
            code += "001111";
            string rt = find_segment(is);
            string imm_str = find_segment(is);
            string imm = tentotwo(imm_str);
            code = code + "00000" + registers[rt] + imm;
            os << code << endl;
        }
        else if (instruction == "slt"){
            code += "000000";
            string rd = find_segment(is);
            string rs = find_segment(is);
            string rt = find_segment(is);
            code = code + registers[rs] + registers[rt] + registers[rd] + "00000" + "101010";
            os << code << endl;
        }
        else if (instruction == "sltu"){
            code += "000000";
            string rd = find_segment(is);
            string rs = find_segment(is);
            string rt = find_segment(is);
            code = code + registers[rs] + registers[rt] + registers[rd] + "00000" + "101011";
            os << code << endl;
        }
        else if (instruction == "slti"){
            code += "001010";
            string rt = find_segment(is);
            string rs = find_segment(is);
            string imm_str = find_segment(is);
            string imm = tentotwo(imm_str);
            code = code + registers[rs] + registers[rt] + imm;
            os << code << endl;
        }
        else if (instruction == "sltiu"){
            code += "001011";
            string rt = find_segment(is);
            string rs = find_segment(is);
            string imm_str = find_segment(is);
            string imm = tentotwo(imm_str);
            code = code + registers[rs] + registers[rt] + imm;
            os << code << endl;
        }
        else if (instruction == "beq"){
            code += "000100";
            string rs = find_segment(is);
            string rt = find_segment(is);
            string lable = find_segment(is);
            code = code + registers[rs] + registers[rt] + offset(lable);
            os << code << endl;
        }
        else if (instruction == "bgez"){
            code += "000001";
            string rs = find_segment(is);
            string lable = find_segment(is);
            code = code + registers[rs] + "00001" + offset(lable);
            os << code << endl;
        }
        else if (instruction == "bgezal"){
            code += "000001";
            string rs = find_segment(is);
            string lable = find_segment(is);
            code = code + registers[rs] + "10001" + offset(lable);
            os << code << endl;
        }
        else if (instruction == "blez"){
            code += "000110";
            string rs = find_segment(is);
            string lable = find_segment(is);
            code = code + registers[rs] + "00000" + offset(lable);
            os << code << endl;
        }
        else if (instruction == "bltzal"){
            code += "000001";
            string rs = find_segment(is);
            string lable = find_segment(is);
            code = code + registers[rs] + "10000" + offset(lable);
            os << code << endl;
        }
        else if (instruction == "bgtz"){
            code += "000111";
            string rs = find_segment(is);
            string lable = find_segment(is);
            code = code + registers[rs] + "00000" + offset(lable);
            os << code << endl;
        }
        else if (instruction == "bltz"){
            code += "000001";
            string rs = find_segment(is);
            string lable = find_segment(is);
            code = code + registers[rs] + "00000" + offset(lable);
            os << code << endl;
        }
        else if (instruction == "bne"){
            code += "000101";
            string rs = find_segment(is);
            string rt = find_segment(is);
            string lable = find_segment(is);
            code = code + registers[rs] + registers[rt] + offset(lable);
            os << code << endl;
        }
        else if (instruction == "j"){
            code += "000010";
            string target = find_segment(is);
            code = code + calculate_target(target);
            os << code << endl;
        }
        else if (instruction == "jal"){
            code += "000011";
            string target = find_segment(is);
            code = code + calculate_target(target);
            os << code << endl;
        }
        else if (instruction == "jalr"){
            code += "000000";
            string rs = find_segment(is);
            string rd = find_segment(is);
            code = code + registers[rs] + "00000" +registers[rd] + "00000" + "001001";
            os << code << endl;
        }
        else if (instruction == "jr"){
            code += "000000";
            string rs = find_segment(is);
            code = code + registers[rs] + "000000000000000" + "001000";
            os << code << endl;
        }
        else if (instruction == "teq"){
            code += "000000";
            string rs = find_segment(is);
            string rt = find_segment(is);
            code = code + registers[rs] + registers[rt] + "0000000000" + "110100";
            os << code << endl;
        }
        else if (instruction == "teqi"){
            code += "000001";
            string rs = find_segment(is);
            string imm_str = find_segment(is);
            string imm = tentotwo(imm_str);
            code = code + registers[rs] + "01110" + imm;
            os << code << endl;
        }
        else if (instruction == "tne"){
            code += "000000";
            string rs = find_segment(is);
            string rt = find_segment(is);
            code = code + registers[rs] + registers[rt] + "0000000000" + "110110";
            os << code << endl;
        }
        else if (instruction == "tnei"){
            code += "000001";
            string rs = find_segment(is);
            string imm_str = find_segment(is);
            string imm = tentotwo(imm_str);
            code = code + registers[rs] + "01110" + imm;
            os << code << endl;
        }
        else if (instruction == "tge"){
            code += "000000";
            string rs = find_segment(is);
            string rt = find_segment(is);
            code = code + registers[rs] + registers[rt] + "0000000000" + "110000";
            os << code << endl;
        }
        else if (instruction == "tgeu"){
            code += "000000";
            string rs = find_segment(is);
            string rt = find_segment(is);
            code = code + registers[rs] + registers[rt] + "0000000000" + "110001";
            os << code << endl;
        }
        else if (instruction == "tgei"){
            code += "000001";
            string rs = find_segment(is);
            string imm_str = find_segment(is);
            string imm = tentotwo(imm_str);
            code = code + registers[rs] + "01000" + imm;
            os << code << endl;
        }
        else if (instruction == "tgeiu"){
            code += "000001";
            string rs = find_segment(is);
            string imm_str = find_segment(is);
            string imm = tentotwo(imm_str);
            code = code + registers[rs] + "01001" + imm;
            os << code << endl;
        }
        else if (instruction == "tlt"){
            code += "000000";
            string rs = find_segment(is);
            string rt = find_segment(is);
            code = code + registers[rs] + registers[rt] + "0000000000" + "110010";
            os << code << endl;
        }
        else if (instruction == "tltu"){
            code += "000000";
            string rs = find_segment(is);
            string rt = find_segment(is);
            code = code + registers[rs] + registers[rt] + "0000000000" + "110011";
            os << code << endl;
        }
        else if (instruction == "tlti"){
            code += "000001";
            string rs = find_segment(is);
            string imm_str = find_segment(is);
            string imm = tentotwo(imm_str);
            code = code + registers[rs] + "01010" + imm;
            os << code << endl;
        }
        else if (instruction == "tltiu"){
            code += "000001";
            string rs = find_segment(is);
            string imm_str = find_segment(is);
            string imm = tentotwo(imm_str);
            code = code + registers[rs] + "01011" + imm;
            os << code << endl;
        }
        else if (instruction == "lb"){
            code += "100000";
            string rs = find_segment(is);
            string rt_imm = find_segment(is);
            string imm_str = "";
            string rt = "";
            bool start_rt = false;
            for (int i = 0; i < rt_imm.length(); i++){
                if(!start_rt && rt_imm[i] != '(' && rt_imm[i] != ')'){
                    imm_str.push_back(rt_imm[i]);
                }
                else if(start_rt && rt_imm[i] != '(' && rt_imm[i] != ')'){
                    rt.push_back(rt_imm[i]);
                }
                else if (rt_imm[i] == '('){
                    start_rt = true;
                }
            }
            string imm = tentotwo(imm_str);
            code = code + registers[rt] + registers[rs] + imm;
            os << code << endl;
        }
        else if (instruction == "lbu"){
            code += "100100";
            string rs = find_segment(is);
            string rt_imm = find_segment(is);
            string imm_str = "";
            string rt = "";
            bool start_rt = false;
            for (int i = 0; i < rt_imm.length(); i++){
                if(!start_rt && rt_imm[i] != '(' && rt_imm[i] != ')'){
                    imm_str.push_back(rt_imm[i]);
                }
                else if(start_rt && rt_imm[i] != '(' && rt_imm[i] != ')'){
                    rt.push_back(rt_imm[i]);
                }
                else if (rt_imm[i] == '('){
                    start_rt = true;
                }
            }
            string imm = tentotwo(imm_str);
            code = code + registers[rt] + registers[rs] + imm;
            os << code << endl;
        }
        else if (instruction == "lh"){
            code += "100001";
            string rs = find_segment(is);
            string rt_imm = find_segment(is);
            string imm_str = "";
            string rt = "";
            bool start_rt = false;
            for (int i = 0; i < rt_imm.length(); i++){
                if(!start_rt && rt_imm[i] != '(' && rt_imm[i] != ')'){
                    imm_str.push_back(rt_imm[i]);
                }
                else if(start_rt && rt_imm[i] != '(' && rt_imm[i] != ')'){
                    rt.push_back(rt_imm[i]);
                }
                else if (rt_imm[i] == '('){
                    start_rt = true;
                }
            }
            string imm = tentotwo(imm_str);
            code = code + registers[rt] + registers[rs] + imm;
            os << code << endl;
        }
        else if (instruction == "lhu"){
            code += "100101";
            string rs = find_segment(is);
            string rt_imm = find_segment(is);
            string imm_str = "";
            string rt = "";
            bool start_rt = false;
            for (int i = 0; i < rt_imm.length(); i++){
                if(!start_rt && rt_imm[i] != '(' && rt_imm[i] != ')'){
                    imm_str.push_back(rt_imm[i]);
                }
                else if(start_rt && rt_imm[i] != '(' && rt_imm[i] != ')'){
                    rt.push_back(rt_imm[i]);
                }
                else if (rt_imm[i] == '('){
                    start_rt = true;
                }
            }
            string imm = tentotwo(imm_str);
            code = code + registers[rt] + registers[rs] + imm;
            os << code << endl;
        }
        else if (instruction == "lw"){
            code += "100011";
            string rs = find_segment(is);
           string rt_imm = find_segment(is);
            string imm_str = "";
            string rt = "";
            bool start_rt = false;
            for (int i = 0; i < rt_imm.length(); i++){
                if(!start_rt && rt_imm[i] != '(' && rt_imm[i] != ')'){
                    imm_str.push_back(rt_imm[i]);
                }
                else if(start_rt && rt_imm[i] != '(' && rt_imm[i] != ')'){
                    rt.push_back(rt_imm[i]);
                }
                else if (rt_imm[i] == '('){
                    start_rt = true;
                }
            }
            string imm = tentotwo(imm_str);
            code = code + registers[rt] + registers[rs] + imm;
            os << code << endl;
        }
        else if (instruction == "lwcl"){
            code += "011111";
            string rs = find_segment(is);
            string rt_imm = find_segment(is);
            string imm_str = "";
            string rt = "";
            bool start_rt = false;
            for (int i = 0; i < rt_imm.length(); i++){
                if(!start_rt && rt_imm[i] != '(' && rt_imm[i] != ')'){
                    imm_str.push_back(rt_imm[i]);
                }
                else if(start_rt && rt_imm[i] != '(' && rt_imm[i] != ')'){
                    rt.push_back(rt_imm[i]);
                }
                else if (rt_imm[i] == '('){
                    start_rt = true;
                }
            }
            string imm = tentotwo(imm_str);
            code = code + registers[rt] + registers[rs] + imm;
            os << code << endl;
        }
        else if (instruction == "lwl"){
            code += "100010";
            string rs = find_segment(is);
            string rt_imm = find_segment(is);
            string imm_str = "";
            string rt = "";
            bool start_rt = false;
            for (int i = 0; i < rt_imm.length(); i++){
                if(!start_rt && rt_imm[i] != '(' && rt_imm[i] != ')'){
                    imm_str.push_back(rt_imm[i]);
                }
                else if(start_rt && rt_imm[i] != '(' && rt_imm[i] != ')'){
                    rt.push_back(rt_imm[i]);
                }
                else if (rt_imm[i] == '('){
                    start_rt = true;
                }
            }
            string imm = tentotwo(imm_str);
            code = code + registers[rt] + registers[rs] + imm;
            os << code << endl;
        }
        else if (instruction == "lwr"){
            code += "100110";
            string rs = find_segment(is);
            string rt_imm = find_segment(is);
            string imm_str = "";
            string rt = "";
            bool start_rt = false;
            for (int i = 0; i < rt_imm.length(); i++){
                if(!start_rt && rt_imm[i] != '(' && rt_imm[i] != ')'){
                    imm_str.push_back(rt_imm[i]);
                }
                else if(start_rt && rt_imm[i] != '(' && rt_imm[i] != ')'){
                    rt.push_back(rt_imm[i]);
                }
                else if (rt_imm[i] == '('){
                    start_rt = true;
                }
            }
            string imm = tentotwo(imm_str);
            code = code + registers[rt] + registers[rs] + imm;
            os << code << endl;
        }
        else if (instruction == "ll"){
            code += "110000";
            string rs = find_segment(is);
            string rt_imm = find_segment(is);
            string imm_str = "";
            string rt = "";
            bool start_rt = false;
            for (int i = 0; i < rt_imm.length(); i++){
                if(!start_rt && rt_imm[i] != '(' && rt_imm[i] != ')'){
                    imm_str.push_back(rt_imm[i]);
                }
                else if(start_rt && rt_imm[i] != '(' && rt_imm[i] != ')'){
                    rt.push_back(rt_imm[i]);
                }
                else if (rt_imm[i] == '('){
                    start_rt = true;
                }
            }
            string imm = tentotwo(imm_str);
            code = code + registers[rt] + registers[rs] + imm;
            os << code << endl;
        }
        else if (instruction == "sb"){
            code += "101000";
            string rs = find_segment(is);
            string rt_imm = find_segment(is);
            string imm_str = "";
            string rt = "";
            bool start_rt = false;
            for (int i = 0; i < rt_imm.length(); i++){
                if(!start_rt && rt_imm[i] != '(' && rt_imm[i] != ')'){
                    imm_str.push_back(rt_imm[i]);
                }
                else if(start_rt && rt_imm[i] != '(' && rt_imm[i] != ')'){
                    rt.push_back(rt_imm[i]);
                }
                else if (rt_imm[i] == '('){
                    start_rt = true;
                }
            }
            string imm = tentotwo(imm_str);
            code = code + registers[rt] + registers[rs] + imm;
            os << code << endl;
        }
        else if (instruction == "sh"){
            code += "101001";
            string rs = find_segment(is);
            string rt_imm = find_segment(is);
            string imm_str = "";
            string rt = "";
            bool start_rt = false;
            for (int i = 0; i < rt_imm.length(); i++){
                if(!start_rt && rt_imm[i] != '(' && rt_imm[i] != ')'){
                    imm_str.push_back(rt_imm[i]);
                }
                else if(start_rt && rt_imm[i] != '(' && rt_imm[i] != ')'){
                    rt.push_back(rt_imm[i]);
                }
                else if (rt_imm[i] == '('){
                    start_rt = true;
                }
            }
            string imm = tentotwo(imm_str);
            code = code + registers[rt] + registers[rs] + imm;
            os << code << endl;
        }
        else if (instruction == "sw"){
            code += "101011";
            string rs = find_segment(is);
            string rt_imm = find_segment(is);
            string imm_str = "";
            string rt = "";
            bool start_rt = false;
            for (int i = 0; i < rt_imm.length(); i++){
                if(!start_rt && rt_imm[i] != '(' && rt_imm[i] != ')'){
                    imm_str.push_back(rt_imm[i]);
                }
                else if(start_rt && rt_imm[i] != '(' && rt_imm[i] != ')'){
                    rt.push_back(rt_imm[i]);
                }
                else if (rt_imm[i] == '('){
                    start_rt = true;
                }
                
            }
            string imm = tentotwo(imm_str);
            code = code + registers[rt] + registers[rs] + imm;
            os << code << endl;
        }
        else if (instruction == "swl"){
            code += "101010";
            string rs = find_segment(is);
            string rt_imm = find_segment(is);
            string imm_str = "";
            string rt = "";
            bool start_rt = false;
            for (int i = 0; i < rt_imm.length(); i++){
                if(!start_rt && rt_imm[i] != '(' && rt_imm[i] != ')'){
                    imm_str.push_back(rt_imm[i]);
                }
                else if(start_rt && rt_imm[i] != '(' && rt_imm[i] != ')'){
                    rt.push_back(rt_imm[i]);
                }
                else if (rt_imm[i] == '('){
                    start_rt = true;
                }
            }
            string imm = tentotwo(imm_str);
            code = code + registers[rt] + registers[rs] + imm;
            os << code << endl;
        }
        else if (instruction == "swr"){
            code += "101110";
            string rs = find_segment(is);
            string rt_imm = find_segment(is);
            string imm_str = "";
            string rt = "";
            bool start_rt = false;
            for (int i = 0; i < rt_imm.length(); i++){
                if(!start_rt && rt_imm[i] != '(' && rt_imm[i] != ')'){
                    imm_str.push_back(rt_imm[i]);
                }
                else if(start_rt && rt_imm[i] != '(' && rt_imm[i] != ')'){
                    rt.push_back(rt_imm[i]);
                }
                else if (rt_imm[i] == '('){
                    start_rt = true;
                }
            }
            string imm = tentotwo(imm_str);
            code = code + registers[rt] + registers[rs] + imm;
            os << code << endl;
        }
        else if (instruction == "sc"){
            code += "111000";
            string rs = find_segment(is);
            string rt_imm = find_segment(is);
            string imm_str = "";
            string rt = "";
            bool start_rt = false;
            for (int i = 0; i < rt_imm.length(); i++){
                if(!start_rt && rt_imm[i] != '(' && rt_imm[i] != ')'){
                    imm_str.push_back(rt_imm[i]);
                }
                else if(start_rt && rt_imm[i] != '(' && rt_imm[i] != ')'){
                    rt.push_back(rt_imm[i]);
                }
                else if (rt_imm[i] == '('){
                    start_rt = true;
                }
            }
            string imm = tentotwo(imm_str);
            code = code + registers[rt] + registers[rs] + imm;
            os << code << endl;
        }
        else if (instruction == "mfhi"){
            code += "0000000000000000";
            string rd = find_segment(is);
            code = code + registers[rd] + "00000" + "010000";
            os << code << endl;
        }
        else if (instruction == "mflo"){
            code += "0000000000000000";
            string rd = find_segment(is);
            code = code + registers[rd] + "00000" + "010010";
            os << code << endl;
        }
        else if (instruction == "mthi"){
            code += "000000";
            string rs = find_segment(is);
            code = code + registers[rs] + "000000000000000" + "010001";
            os << code << endl;
        }
        else if (instruction == "mtlo"){
            code += "000000";
            string rs = find_segment(is);
            code = code + registers[rs] + "000000000000000" + "010011";
            os << code << endl;
        }
        else if (instruction == "syscall"){
            code = code + "000000" + "00000000000000000000" + "001100";
            os << code << endl;
        }
    }
}


void transfer(){
    ifstream a_put;
    ofstream machine_code;
    a_put.open("a_put.txt");
    machine_code.open("machine_code.txt");
    translate(a_put, machine_code);
    a_put.close();
    machine_code.close();
}

/*
int main(){
    store_registers();
    ifstream infile1;
    ofstream outfile1;
    ofstream data_file;
    infile1.open("MIP_user.txt");
    outfile1.open("a_put.txt",ios::out);
    data_file.open("data_put.txt",ios::out);
    removeComments(infile1, outfile1, data_file);
    infile1.close();
    outfile1.close();
    transfer();
}
*/
    