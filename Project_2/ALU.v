`timescale 1ns/1ps
module ALU(instruction, regA, regB, result, flags);
    input [31:0] instruction;
    input signed [31:0]  regA, regB;
    output reg signed [31:0] result;
    output [2:0] flags;

    wire signed [31:0] regA, regB;
    reg [5:0] opcode;
    reg signed [31:0] imm;
    reg signed [4:0] sa;
    reg [4:0] rs_code;
    reg [4:0] rt_code;
    reg [5:0] funct;

    reg signed [31:0] rs;
    reg signed [31:0] rt;
    reg signed [31:0] usigned_rs;
    reg signed [31:0] usigned_rt;
    reg signed [31:0] usigned_imm; 
    reg signed [31:0] compare;
    reg signed [31:0] temp;
    reg [2:0] flags;
    reg [2:0] flag_temp;


    parameter Rtype = 6'b000000, Add = 6'b100000, Addi = 6'b001000, Addu = 6'b100001, Addiu = 6'b001001, 
    Sub = 6'b100010, Subu = 6'b100011, And = 6'b100100, Andi = 6'b001100, Nor = 6'b100111, Or = 6'b100101,
    Ori = 6'b001101, Xor = 6'b100110, Xori = 6'b001110, Beq = 6'b000100, Bne = 6'b000101, Slt = 6'b101010,
    Slti = 6'b001010, Sltiu = 6'b001011, Sltu = 6'b101011, Lw = 6'b100011, Sw = 6'b101011, Sll = 6'b000000,
    Sllv = 6'b000100, Srl = 6'b000010, Srlv = 6'b000110, Sra = 6'b000011, Srav = 6'b000111;


    always@(instruction, regA, regB)begin
        imm = $signed(instruction[15:0]);
        sa = instruction[10:6];
        opcode = instruction[31:26];
        rs_code = instruction[25:21];
        rt_code = instruction[20:16];
        funct = instruction[5:0];
        flag_temp = 3'b000;

        if (rs_code == 5'b00000)begin
            rs = regA[31:0];
        end
        else if (rs_code == 5'b00001)begin
            rs = regB[31:0];
        end

        if (rt_code === 5'b00000)begin
            rt = regA[31:0];
        end
        else if (rt_code == 5'b00001)begin
            rt = regB[31:0];
        end

        case(opcode)
        // Add
            Rtype: begin
                case(funct)
                    Add: begin
                        temp = rs + rt;
                        if (rs[31] == 1'b1 && rt[31] == 1'b1 && temp[31] == 1'b0)begin
                            flag_temp = 3'b100;
                        end
                        if (rs[31] == 1'b0 && rt[31] == 1'b0 && temp[31] == 1'b1)begin
                            flag_temp = 3'b100;
                        end
                    end

                    Addu: begin
                        temp = rs + rt;
                    end
                    Sub: begin
                        temp = rs - rt;
                        if (rs[31] == 1'b1 && rt[31] == 1'b0 && temp[31] == 1'b0)begin
                            flag_temp = 3'b100;
                        end
                        if (rs[31] == 1'b0 && rt[31] == 1'b1 && temp[31] == 1'b1)begin
                            flag_temp = 3'b100;
                        end
                    end
                    Subu: begin
                        temp = rs - rt;
                    end
                    And: begin
                        temp = rs & rt;
                    end
                    Nor: begin
                        temp = ~(rs | rt);
                    end
                    Or: begin
                        temp = rs | rt;
                    end
                    Xor: begin
                        temp = rs ^ rt;
                    end
                    Slt: begin
                        if (rs < rt)begin
                            flag_temp = 3'b010;
                            temp = 32'b1;
                        end
                        else begin
                            temp = 32'b0;
                        end
                    end
                    Sltu: begin
                        if (rt[31] == rs[31])begin
                            usigned_rt = {1'b0, rt[30:0]};
                            usigned_rs = {1'b0, rs[30:0]};
                            if (usigned_rs < usigned_rt)begin
                                flag_temp = 3'b010;
                                temp = 32'b1;
                            end
                            else begin
                                temp = 32'b0;
                            end
                        end
                        else if (rt[31] == 1'b1 && rs[31] == 1'b0)begin
                            flag_temp = 3'b010;
                            temp = 32'b1;
                        end
                        else if (rt[31] == 1'b0 && rs[31] == 1'b1)begin
                        temp = 32'b0;
                        end
                    end
                    Sll: begin
                        temp = rt << sa;
                    end
                    Sllv: begin
                        temp = rt << rs[4:0];
                    end
                    Srl: begin
                        temp = rt >> sa;
                    end
                    Srlv:begin
                        temp = rt >> rs[4:0];
                    end
                    Sra:begin
                        temp = rt >>> sa;
                    end
                    Srav:begin
                        temp = rt >>> rs[4:0];
                    end
                endcase
            end
            Addi: begin
                temp = rs + imm;
                if (rs[31] == 1'b1 && imm[31] == 1'b1 && temp[31] == 1'b0)begin
                    flag_temp = 3'b100;
                end
                if (rs[31] == 1'b0 && imm[31] == 1'b0 && temp[31] == 1'b1)begin
                    flag_temp = 3'b100;
                end
            end
            Addiu: begin
                temp = rs + imm;
            end
            Andi: begin
                imm = {16'b0, imm[15:0]};
                temp = rs & imm;
            end
            Ori: begin
                imm = {16'b0, imm[15:0]};
                temp = rs | imm;
            end
            Xori: begin
                imm = {16'b0, imm[15:0]};
                temp = rs ^ imm;
            end
            Slti: begin
                if (rs < imm)begin
                    flag_temp = 3'b010;
                    temp = 32'b1;
                end
                else begin
                    temp = 32'b0;
                end
            end
            Sltiu:begin
                if (imm[31] == rs[31])begin
                    usigned_imm = {1'b0, imm[30:0]};
                    usigned_rs = {1'b0, rs[30:0]};
                    if (usigned_rs < usigned_imm)begin
                        flag_temp = 3'b010;
                        temp = 32'b1;
                    end
                    else begin
                        temp = 32'b0;
                    end
                end
                else if (imm[31] == 1'b1 && rs[31] == 1'b0)begin
                    flag_temp = 3'b010;
                    temp = 32'b1;
                end
                else if (imm[31] == 1'b0 && rs[31] == 1'b1)begin
                    temp = 32'b0;
                end
            end
            Lw: begin
                temp = imm + rs;
            end
            Sw: begin
                temp = imm + rs;
            end
            Beq: begin
                if (rs === rt)begin
                    flag_temp = 3'b001;
                    temp = 32'b0000_0000_0000_0000_0000_0000_0000_0001;
                end
                else begin
                    temp = 32'b0000_0000_0000_0000_0000_0000_0000_0000;
                end
            end
            Bne: begin
                if (rs == rt)begin
                    flag_temp = 3'b001;
                    temp = 32'b0000_0000_0000_0000_0000_0000_0000_0000;
                end
                else begin
                    temp = 32'b0000_0000_0000_0000_0000_0000_0000_0001;
                end
            end
        endcase
    assign result = temp;
    assign flags = flag_temp;
    end
    
    
endmodule