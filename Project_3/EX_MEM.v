// in this pipeline registers, input and output are the same
//     output:
//     - FECTH_ADDRESS
//     - WriteDataM
//     - wb_addrM
//     - RegWriteM
//     - MemtoRegM

module EX_MEM(
    input [31:0] ALUOutE,
    input [31:0] rtE,
    input [4:0] wb_addrE,
    input RegWriteE,
    input MemtoRegE,
    input CLOCK,
    input MemWriteE,
    input ENABLEE,
    input [4:0] rs_addrE, 
    input [4:0] rt_addrE,

    //output reg MemWriteM,
    output reg[64:0] EDIT_SERIAL,
    output reg[31:0] ALUOutM,
    //output reg[31:0] rt_valueM,
    output reg[4:0] wb_addrM,
    output reg RegWriteM,
    output reg MemtoRegM,
    output reg ENABLE_MEM,
    output reg [4:0] rs_addrM,
    output reg [4:0] rt_addrM
);
    always@(posedge CLOCK)begin
        
        EDIT_SERIAL = {MemWriteE, ALUOutE, rtE};
        //MemWriteM <= MemWriteE;
        ALUOutM <= ALUOutE;
        //rt_valueM <= rtE;
        wb_addrM <= wb_addrE;
        RegWriteM <= RegWriteE;
        MemtoRegM <= MemtoRegE;
        ENABLE_MEM <= ENABLEE;
        rs_addrM <= rs_addrE;
        rt_addrM <= rt_addrE;

    end

endmodule

