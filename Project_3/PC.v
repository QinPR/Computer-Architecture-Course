// input and input from: 
//     (data) ID : branch_addr
//     (control signal) Hazard_Detection_Unit: PCWrite
//     (control signal) ID : PCsrc

// output and output destination:
//     (data) PCaddr -> InstrcutionRAM
//     (data) PCPlus4 -> IF_ID

// function and basic logic:
//     - if PCWrite = 0: PC keep unchanged
//     else: 
//     - if PCsrc is 1: output branch_addr, and change current PC to branch_addr
//     - if PCsrc is 0: output current PC
//     - current PC += 4, and output it as PCPlus4


module PC(CLOCK, branch_addr, PCWrite, branch_signal, PCplus4F, FETCH_ADDRESS, ENABLE, RESET, dont_branch, JR_branch_addr, JR_branch_signal);
    input [31:0] branch_addr;
    input PCWrite;
    input branch_signal;
    input CLOCK;
    input dont_branch;
    input [31:0] JR_branch_addr;
    input JR_branch_signal;

    output reg[31:0] PCplus4F = 32'b0; 
    output reg[31:0] FETCH_ADDRESS;
    reg[31:0] PC = 32'b0;
    output reg ENABLE = 1'b1;
    output reg RESET = 1'b0;

    reg [31:0] previous_pc;

always@(posedge CLOCK) begin

    if (PCWrite == 1'b0) begin
        PC <= PC;
        // $display("PC rewrite");
    end
    else begin
        if (JR_branch_signal == 1'b1)begin
            FETCH_ADDRESS <= JR_branch_addr;
            previous_pc <= (PC - 1);
            PC <= JR_branch_addr;
        end
        else if (branch_signal == 1'b1)begin
            FETCH_ADDRESS <= branch_addr;
            previous_pc <= (PC - 1);
            PC <= branch_addr;
            // $display(branch_addr);
        end

        if (dont_branch == 1'b1)begin
            PC = previous_pc;
            FETCH_ADDRESS = PC;
        end
        else begin
            FETCH_ADDRESS = PC;
        end
        // $display(PC);
        PC = PC + 1;
        // PCplus4F = PC;   
    
        // $display(PCplus4F);
    end
end

always@(negedge CLOCK)begin
    // $display(PCplus4F);
    PCplus4F = PC;
end
endmodule