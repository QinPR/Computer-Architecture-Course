
module Hazard_Detection_Unit(
    input CLOCK,
    input MemWriteE,
    input ENABLEE,
    input[4:0] wb_addrD,
    input[31:0] instructionD,

    output PCWrite = 1,
    output IF_IDWrite = 1,
    output EX_NOP = 0  
);

reg[4:0] IF_ID_rsaddr;
reg[4:0] IF_ID_rtaddr;

always@(negedge CLOCK)begin
  // $display("%b", instructionD);
  PCWrite = 1'b1;
  IF_IDWrite = 1'b1;
  EX_NOP = 1'b0;
  IF_ID_rsaddr = instructionD[25:21];
  IF_ID_rtaddr = instructionD[20:16];



  if (MemWriteE == 1'b0 && ENABLEE == 1'b1)begin
    if (IF_ID_rsaddr == wb_addrD || IF_ID_rtaddr == wb_addrD)begin
      PCWrite = 0;
      IF_IDWrite = 0;
      EX_NOP = 1;
      // $display("%b",instructionD);
    end
  end   
end


endmodule