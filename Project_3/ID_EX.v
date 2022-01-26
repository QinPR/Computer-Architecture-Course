//     input:
//     - ...
//     output and output destination:
//     - (signal) RegWriteE -> EX_MEM     Enable the ID to write data into the regfile
//     - (signal) MemtoRegE -> EX_MEM      judge the WB value is from memory or just from ALU
//     - (signal) MemWriteE -> EX_MEM      Enable write data to memory
//     - (signal) ALUControlE  -> EX   a 4 bit value determine ALU operation
//     - (signal) ALUSrcE  -> EX
//     - (data) immE -> EX
//     - (data) wb_addrE - > EX_MEM
//     - (data) WriteDataE  -> EX_MEM

module ID_EX(
    input RegWriteD,
    input MemtoRegD,
    input MemWriteD,
    input [11:0] ALUControlD,
    input ALUSrcD,
    input [31:0] rs_valueD,
    input [31:0] rt_valueD,
    input [31:0] immD,
    input [4:0] wb_addrD,
    input CLOCK,
    input [4:0] saD,
    input ENABLED,
    input [4:0] rs_addrD, 
    input [4:0] rt_addrD,
    input branch_signal,
    input EX_NOP,
    input JR_EX_NOP,
    input [31:0] PCplus4D,

    output reg RegWriteE,
    output reg MemtoRegE,
    output reg MemWriteE,
    output reg [11:0] ALUControlE,
    output reg ALUSrcE,
    output reg [31:0] immE,
    output reg [4:0] wb_addrE,
    output reg [31:0] rs_valueE,
    output reg [31:0] rt_valueE,
    output reg [4:0] saE,
    output reg ENABLEE,
    output reg [4:0] rs_addrE, 
    output reg [4:0] rt_addrE,
    output reg branch_signalE,
    output reg[31:0] PCplus4E

);

    always@(posedge CLOCK)begin
        if (EX_NOP == 1'b1 || JR_EX_NOP == 1'b1)begin     
            RegWriteE <= 0;
            MemtoRegE <= 0;
            MemWriteE <= 0;
            ALUControlE <= 0;
            ALUSrcE <= 0;
            immE <= 0;
            wb_addrE <= 0;
            rs_valueE <= 0;
            rt_valueE <= 0;
            saE <= 0;
            wb_addrE <= 0;
            ENABLEE <= 0;
            rs_addrE <= 0;
            rt_addrE <= 0;
            branch_signalE <= 0;
            PCplus4E <= 0;
        end
        else begin
            RegWriteE <= RegWriteD;
            MemtoRegE <= MemtoRegD;
            MemWriteE <= MemWriteD;
            ALUControlE <= ALUControlD;
            ALUSrcE <= ALUSrcD;
            immE <= immD;
            wb_addrE <= wb_addrD;
            rs_valueE <= rs_valueD;
            rt_valueE <= rt_valueD;
            saE <= saD;
            wb_addrE <= wb_addrD;
            ENABLEE <= ENABLED;
            rs_addrE <= rs_addrD;
            rt_addrE <= rt_addrD;
            branch_signalE <= branch_signal;
            PCplus4E <= PCplus4D;
        end
        
    end


endmodule
