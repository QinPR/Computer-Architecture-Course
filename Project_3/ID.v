// input and input from:
//     - (data) IF_ID : instructionD
//     - (signal) RegWriteW
//     - (data) ResultW
// output and output destination:
//     (data) OP -> Control_Unit
//     (data) Funct -> Control_Unit
//     (data) rs_valueD -> ID_EX
//     (data) rt_valueD -> ID_EX
//     (data) wb_addrD -> ID_EX     used to identify the adress to be written
//     (data) branch_addr -> PC
//     (data) branch_signal -> Hazard_Detection_Unit      used to tell the Hazard_Detection_Unit to stall for a cycle
//     (data) immD -> ID_EX
// function and basci logic:
//     - trigger when positive edge of CLOCK
//     - first, decode the instuctionD to many segment 
//     - send the address to different units, send rs_addr, rt_addr to the register file
//     - fetch rs_value and rt_value from the register file
//     - use the OP and funct to judge whether rt or rd is the wb_addr
//     - calculate the branch address and use a XOR Gate to judge the branch_signal

// in here judge whether should branch or jump and output the branch address

// input and input from:
//     (data) OP : ID
    //    (data) funct : ID
    //    (data) hazard_flush : Hazard_Detection_Unit
// output and output destination:
//     (signal) RegWriteD -> ID_EX      Enable the ID to write data into the regfile
//     (signal) MemtoRegD -> ID_EX      judge the WB value is from memory or just from ALU      }these three need to make sure that ID flush is not zero
//     (signal) MemWriteD -> ID_EX      Enable write data to memory
//     (signal) ALUControlD  -> ID_EX   a 6 bit value determine ALU operation  eg. 00 + 00000
//     (signal) ALUSrcD  -> ID_EX       
// function and basic logic:
//     according to the OP and funct, judge the corresponding instruction 
//     and send the control signal to the EX_MEM

module ID(CLOCK, PCplus4D, instructionD, RegWriteW, wb_addrW, ResultW, hazard_flush, saD, 
RegWriteD,ENABLED, MemtoRegD, MemWriteD, ALUControlD, ALUSrcD, branch_addr, rs_valueD, rt_valueD, 
wb_addrD, branch_signal, immD, rs_addrD, rt_addrD, IF_FLUSH, wb_addrE, ALUOutE);
    input CLOCK;
    input [31:0] PCplus4D;
    input [31:0] instructionD;
    input RegWriteW;
    input [4:0] wb_addrW;
    input [31:0] ResultW;
    input hazard_flush;
    input [4:0] wb_addrE;
    input [31:0] ALUOutE;

    output reg RegWriteD;
    output reg MemtoRegD;
    output reg MemWriteD;
    output reg[11:0] ALUControlD;
    output reg ALUSrcD;
    output reg[31:0] branch_addr;
    output reg[31:0] rs_valueD;
    output reg[31:0] rt_valueD;
    output reg[4:0] wb_addrD;
    output reg branch_signal;
    output reg[31:0] immD;
    output reg[4:0] saD;
    output reg ENABLED;
    output reg[4:0] rt_addrD;
    output reg[4:0] rs_addrD;
    output reg[31:0] IF_FLUSH = 0;

    reg[31:0] regfile[31:0];
    reg[4:0] rd_addr;
    reg[5:0] OP;
    reg[5:0] Funct;
    reg[31:0] shift_imm;
    reg[5:0] i;

    parameter Rtype = 6'b000000, Add = 6'b100000, Addi = 6'b001000, Addu = 6'b100001, Addiu = 6'b001001, 
    Sub = 6'b100010, Subu = 6'b100011, And = 6'b100100, Andi = 6'b001100, Nor = 6'b100111, Or = 6'b100101,
    Ori = 6'b001101, Xor = 6'b100110, Xori = 6'b001110, Beq = 6'b000100, Bne = 6'b000101, Slt = 6'b101010,
    Slti = 6'b001010, Sltiu = 6'b001011, Sltu = 6'b101011, Lw = 6'b100011, Sw = 6'b101011, Sll = 6'b000000,
    Sllv = 6'b000100, Srl = 6'b000010, J = 6'b000010, Srlv = 6'b000110, Sra = 6'b000011, Jr = 6'b001000, Jal = 6'b000011 ,Srav = 6'b000111;    

    initial begin
        for (i = 0; i < 32; i = i+1) begin
            regfile[i] = 32'b0;
        end
    end

    always@(negedge CLOCK)begin

        immD = $signed(instructionD[15:0]);
        OP = instructionD[31:26];
        rs_addrD = instructionD[25:21];
        rt_addrD = instructionD[20:16];
        rd_addr = instructionD[15:11];
        Funct = instructionD[5:0];
        saD = instructionD[10:6];

        rs_valueD = regfile[rs_addrD];
        rt_valueD = regfile[rt_addrD];

        ALUControlD = {OP, Funct};
        ALUSrcD = 0;           // select from imm or rt
        RegWriteD = 0;        // allow write to reg
        MemtoRegD = 0;         // select from mem or alu result
        MemWriteD = 0;          // allow to write to mem
        ENABLED = 0;
        branch_signal = 0;
        if (IF_FLUSH != 0)begin
            IF_FLUSH = IF_FLUSH - 1;
        end
        case (OP)
            Rtype: begin
                if (instructionD != 32'b0)begin
                    RegWriteD = 1'b1;
                    wb_addrD = rd_addr; 
                end
                if (Funct == Jr)begin
                    RegWriteD = 1'b0;
                end
            end
            Lw: begin
                MemtoRegD = 1'b1;
                wb_addrD = rt_addrD;
                rt_addrD = 5'b00000;
                ENABLED = 1'b1;
                RegWriteD = 1'b1;
            end    
            Sw: begin 
                MemWriteD = 1'b1;
                ENABLED = 1'b1;
            end
            Beq: begin
                if (rs_valueD == rt_valueD) begin
                    // branch_addr = PCplus4D + immD;
                    // branch_signal = 1'b1;
                    // IF_FLUSH = IF_FLUSH + 1;
                end
            end
            Addi: begin
                RegWriteD = 1'b1;
                ALUSrcD = 1'b1;
                wb_addrD = rt_addrD;
                rt_addrD = 5'b00000;
                // $display(PCplus4D); 
            end    
            Addiu: begin
                RegWriteD = 1'b1;
                ALUSrcD = 1'b1;
                wb_addrD = rt_addrD;
                rt_addrD = 5'b00000; 
            end
            Andi: begin
                RegWriteD = 1'b1;
                ALUSrcD = 1'b1;
                wb_addrD = rt_addrD;
                rt_addrD = 5'b00000; 
            end
            Ori: begin
                RegWriteD = 1'b1;
                ALUSrcD = 1'b1;
                wb_addrD = rt_addrD;
                rt_addrD = 5'b00000; 
            end
            Xori: begin
                RegWriteD = 1'b1;
                ALUSrcD = 1'b1;
                wb_addrD = rt_addrD;
                rt_addrD = 5'b00000;
            end
            Bne: begin
                if (rs_valueD != rt_valueD) begin
                    // branch_addr = PCplus4D + immD;
                    // branch_signal = 1'b1;
                    // IF_FLUSH = IF_FLUSH + 1;
                end
            end
            J: begin
                shift_imm = immD;
                branch_addr = {PCplus4D[31:28] , shift_imm[27:0]};
                branch_signal = 1'b1;
                IF_FLUSH = IF_FLUSH + 1;
            end
            Jal: begin
                shift_imm = immD;
                branch_addr = {PCplus4D[31:28] , shift_imm[27:0]};
                RegWriteD = 1'b1;
                wb_addrD = 5'b11111;
                branch_signal = 1'b1;
                rs_valueD = PCplus4D;
                IF_FLUSH = IF_FLUSH + 1;
                rt_valueD = 0;
            end
        endcase
    end
    always@(ResultW, RegWriteW, wb_addrW)begin
        if (RegWriteW == 1'b1) begin      
            regfile[wb_addrW] = ResultW;
            // $display("%b", wb_addrW);
            // $display("%b", ResultW);
        end
    end
endmodule