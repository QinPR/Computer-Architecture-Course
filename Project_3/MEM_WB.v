// output:
//     - (signal RegWriteW)
//     - (data) ResultW

module MEM_WB(
    input [31:0] ALUOutM,
    input [31:0] DATA_MEM,
    input CLOCK,
    input RegWriteM,
    input MemtoRegM,
    input [4:0] wb_addrM,

    output reg RegWriteW,
    output reg[31:0] ResultW,
    output reg[4:0] wb_addrW
);

always@(posedge CLOCK)begin
  
    RegWriteW <= RegWriteM;
    if (MemtoRegM == 1'b0)begin
        ResultW <= ALUOutM;
    end
    else begin
        ResultW <= DATA_MEM;
    end
    wb_addrW <= wb_addrM;

end

endmodule