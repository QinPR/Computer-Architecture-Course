// input and input from:
//     (data) Instruction_RAM : instruction 
//     (data) PC: PCplus4F
// output and output destination；
//     (data) instructionD -> ID
//     (data) PCplus4D -> ID_EX
// function and basic logic:
//     - trigger when positive edge of CLOCK
//     - directly output value

module IF_ID(IF_FLUSH, CLOCK, DATA, PCplus4F, instructionD, PCplus4D, dont_branch, IF_IDWrite, JR_IF_FLUSH);
    input [31:0] IF_FLUSH;
    input[31:0] DATA;
    input[31:0] PCplus4F;
    input CLOCK;
    input dont_branch;
    input IF_IDWrite;
    input [31:0] JR_IF_FLUSH;

    output reg[31:0] instructionD = 32'b0;
    output reg[31:0] PCplus4D;

    reg[31:0] temp_instructionD = 32'b0;
    reg[31:0] temp_PCplus4D;


    always@(posedge CLOCK)begin
      
      if (DATA != 10'b0) begin
        temp_instructionD = DATA;
      end
      if (JR_IF_FLUSH > 0 || IF_FLUSH > 0 || dont_branch == 1'b1)begin
        temp_instructionD = 32'b0;
      end
      temp_PCplus4D = PCplus4F;

      if (IF_IDWrite == 1'b0)begin
        // $display("lw hazard");
      end
      else begin
        instructionD = temp_instructionD;
        PCplus4D = temp_PCplus4D;
      end

      // if (1)begin
      //   instructionD = temp_instructionD;
      //   PCplus4D = temp_instructionD;
      // end
    end
    
endmodule