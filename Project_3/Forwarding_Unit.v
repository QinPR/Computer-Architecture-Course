
module Forwarding_Unit(
    input CLOCK,
    input RegWriteM,
    input [4:0] wb_addrE,
    input [4:0] wb_addrM,
    input [4:0] rs_addrD,
    input [4:0] rt_addrD,
    input [4:0] rs_addrE,
    input [4:0] rt_addrE,
    input RegWriteE,

    output reg[1:0] ForwardA,
    output reg[1:0] ForwardB
);

initial begin
  ForwardA <= 2'b00;
  ForwardB <= 2'b00;
end

always@(posedge CLOCK)begin
    ForwardA = 2'b00;
    ForwardB = 2'b00;

    // MEM HAZARD
    if (RegWriteM && (wb_addrM == rs_addrD) && (wb_addrM != 5'b00000))begin
      ForwardA = 2'b01;
    end
    if (RegWriteM && (wb_addrM == rt_addrD) && (wb_addrM != 5'b00000))begin
      ForwardB = 2'b01;
    end

    // EX HAZARD
    if ((RegWriteE == 1'b1) && (wb_addrE == rs_addrD) && (wb_addrE != 5'b00000))begin
      ForwardA = 2'b10;
    end
    if ((RegWriteE == 1'b1) && (wb_addrE == rt_addrD) && (wb_addrE != 5'b00000))begin
      ForwardB = 2'b10;
    end
    
end


endmodule