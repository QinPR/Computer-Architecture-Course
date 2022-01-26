// input and input from:
//     (data) immE : ID_EX
//     (signal) ALUSrcE : ID_EX
//     (data) rs_valueE : ID_EX
//     (data) rt_valueE
//     (signal) ALUControlE

// output and output destination:
//     (data) ALUOutE    -> EX_MEM
// function and basic logic:
//     according to the alu control signal, do the corresponding operation

module EX(CLOCK, immE, ALUSrcE, rs_valueE, rt_valueE, ALUControlE, 
ALUOutE, saE, ForwardA, ForwardB, ResultW, rtE, branch_signalE, dont_branch, 
DATA_MEM, JR_IF_FLUSH, JR_branch_signal, JR_branch_addr, JR_EX_NOP, PCplus4E);
    input CLOCK;
    input [31:0] immE;
    input ALUSrcE;
    input signed [31:0] rs_valueE;
    input signed [31:0] rt_valueE;
    input [11:0] ALUControlE;
    input [4:0] saE;
    input [1:0] ForwardA, ForwardB;
    input [31:0] ResultW;
    input branch_signalE;
    input [31:0] DATA_MEM;
    input [31:0] PCplus4E;

    output reg signed [31:0] ALUOutE;
    output reg[31:0] rtE;
    output reg dont_branch;
    output reg[31:0] JR_IF_FLUSH;
    output reg JR_branch_signal;
    output reg[31:0] JR_branch_addr;
    output reg JR_EX_NOP;

    reg[31:0] opcode;
    reg[31:0] funct;
    reg [31:0] usigned_rs;
    reg [31:0] usigned_rt;
    reg [31:0] usigned_imm;
    reg [31:0] temp_imm;
    reg signed [31:0] rs;
    reg signed [31:0] rt;
    reg signed [31:0] last_result;
    reg signed [31:0] last_last_result;
    reg signed [31:0] last_mem_result;
    reg branchE;

    parameter Rtype = 6'b000000, Add = 6'b100000, Addi = 6'b001000, Addu = 6'b100001, Addiu = 6'b001001, 
    Sub = 6'b100010, Subu = 6'b100011, And = 6'b100100, Andi = 6'b001100, Nor = 6'b100111, Or = 6'b100101,
    Ori = 6'b001101, Xor = 6'b100110, Xori = 6'b001110, Beq = 6'b000100, Bne = 6'b000101, Slt = 6'b101010,
    Slti = 6'b001010, Sltiu = 6'b001011, Sltu = 6'b101011, Lw = 6'b100011, Sw = 6'b101011, Sll = 6'b000000,
    Sllv = 6'b000100, Srl = 6'b000010, J = 6'b000010, Srlv = 6'b000110, Sra = 6'b000011, Jr = 6'b001000, Jal = 6'b000011 ,Srav = 6'b000111;

    always@(posedge CLOCK)begin
        last_result <= ALUOutE;
        
    end

    always@(ResultW)begin
        last_last_result <= ResultW;
    end

    // always@(ForwardA, ForwardB, immE, ALUSrcE, rs_valueE, rt_valueE, ALUControlE)begin
    always@(negedge CLOCK)begin

    opcode = ALUControlE[11:6];
    funct = ALUControlE[5:0];
    JR_IF_FLUSH = 0;
    JR_EX_NOP = 0;
    JR_branch_signal = 0;

    rs = rs_valueE;
    rt = rt_valueE;
    dont_branch = 1'b0;
    if (ForwardA == 2'b01)begin
        rs = last_last_result;
    end
    if (ForwardB == 2'b01)begin
        rt = last_last_result;
    end
    if (ForwardA == 2'b10)begin
        rs = last_result;
        // $display(rs);
    end
    if (ForwardB == 2'b10)begin
        rt = last_result;
    end
    rtE = rt;
        case(opcode)
            Rtype: begin
                case(funct)
                    Add: begin
                        ALUOutE = rs + rt;
                    end
                    Addu: begin
                        ALUOutE = rs + rt;
                    end
                    Sub: begin
                        ALUOutE = rs - rt;
                        // $display(rs);
                        // $display(rt);
                    end
                    Subu: begin
                        ALUOutE = rs - rt;
                    end
                    And: begin
                        ALUOutE = rs & rt;
                    end
                    Nor: begin
                        ALUOutE = ~(rs | rt);
                    end
                    Or: begin
                        ALUOutE = rs | rt;
                    end
                    Xor: begin
                        ALUOutE = rs ^ rt;
                    end
                    Slt: begin
                        if (rs < rt)begin
                            ALUOutE = 32'b1;
                        end
                        else begin
                            ALUOutE = 32'b0;
                        end

                    end
                    Sltu: begin
                        if (rt[31] == rs[31])begin
                            usigned_rt = {1'b0, rt[30:0]};
                            usigned_rs = {1'b0, rs[30:0]};
                            if (usigned_rs < usigned_rt)begin
                                ALUOutE = 32'b1;
                            end
                            else begin
                                ALUOutE = 32'b0;
                            end
                        end
                        else if (rt[31] == 1'b1 && rs[31] == 1'b0)begin
                            ALUOutE = 32'b1;
                        end
                        else if (rt[31] == 1'b0 && rs[31] == 1'b1)begin
                            ALUOutE = 32'b0;
                        end
                    end
                    Sll: begin
                        ALUOutE = rt << saE;
                    end
                    Sllv: begin
                        ALUOutE = rt << rs[4:0];
                    end
                    Srl: begin
                        ALUOutE = rt >> saE;
                    end
                    Srlv:begin
                        ALUOutE = rt >> rs[4:0];
                    end
                    Sra:begin
                        ALUOutE = rt >>> saE;
                    end
                    Srav:begin
                        ALUOutE = rt >>> rs[4:0];
                    end
                    Jr: begin
                        JR_branch_addr = rs/4;
                        JR_branch_signal = 1'b1;
                        JR_IF_FLUSH = JR_IF_FLUSH + 1;
                        JR_EX_NOP = 1;
                    end
                endcase
            end
            Addi: begin
                ALUOutE = rs + immE;
            end
            Addiu: begin
                ALUOutE = rs + immE;
            end
            Andi: begin
                temp_imm = {16'b0, immE[15:0]};
                ALUOutE = rs & temp_imm;
            end
            Ori: begin
                temp_imm = {16'b0, immE[15:0]};
                ALUOutE = rs | temp_imm;
            end
            Xori: begin
                temp_imm = {16'b0, immE[15:0]};
                ALUOutE = rs ^ temp_imm;
            end
            Slti: begin
                if (rs < immE)begin
                    ALUOutE = 32'b1;
                end
                else begin
                    ALUOutE = 32'b0;
                end
            end
            Sltiu:begin
                if (immE[31] == rs[31])begin
                    usigned_imm = {1'b0, immE[30:0]};
                    usigned_rs = {1'b0, rs[30:0]};
                    if (usigned_rs < usigned_imm)begin
                        ALUOutE = 32'b1;
                    end
                    else begin
                        ALUOutE = 32'b0;
                    end
                end
                else if (immE[31] == 1'b1 && rs[31] == 1'b0)begin
                    ALUOutE = 32'b1;
                end
                else if (immE[31] == 1'b0 && rs[31] == 1'b1)begin
                    ALUOutE = 32'b0;
                end
            end
            Lw: begin
                ALUOutE = immE + rs;
            end
            Sw: begin
                ALUOutE = immE + rs;
            end
            Beq: begin
                if (rs == rt)begin
                    ALUOutE = 32'b0000_0000_0000_0000_0000_0000_0000_0001;
                    JR_branch_addr = PCplus4E + immE;
                    JR_branch_signal = 1'b1;
                    JR_IF_FLUSH = JR_IF_FLUSH + 1;
                    JR_EX_NOP = 1;
                end
                else begin
                    ALUOutE = 32'b0000_0000_0000_0000_0000_0000_0000_0000;
                end
            end
            Bne: begin
                if (rs == rt)begin
                    ALUOutE = 32'b0000_0000_0000_0000_0000_0000_0000_0000;
                    branchE = 0;
                end
                else begin
                    ALUOutE = 32'b0000_0000_0000_0000_0000_0000_0000_0001;
                    branchE = 1;
                    JR_branch_addr = PCplus4E + immE;
                    JR_branch_signal = 1'b1;
                    JR_IF_FLUSH = JR_IF_FLUSH + 1;
                    JR_EX_NOP = 1;
                end
                // if (branch_signalE != branchE)begin
                //     dont_branch = 1;
                // end
            end
            Jal: begin
                ALUOutE = (rs + rt) * 4;
            end
        endcase    
        // $display("%h", ALUOutE);
    end
endmodule