// `include "CPU.v"
`timescale 1ns/1ps


module test_cpu;
    reg [10:0] i;
    reg CLOCK = 1'b0;
    reg [31:0] Cycles = 0;
    reg [31:0] instruction = 0;
    cpu testcpu(CLOCK);

    always begin
        if (testcpu.if_id.instructionD != 32'hffffffff)begin
            #100
            CLOCK <= ~CLOCK;
            #100
            CLOCK <= ~CLOCK;
            Cycles = Cycles + 1;
        end
        else begin
            #100
            CLOCK = ~CLOCK;
            #100
            CLOCK = ~CLOCK;
            Cycles = Cycles + 1;
            #100
            CLOCK = ~CLOCK;
            #100
            CLOCK = ~CLOCK;
            Cycles = Cycles + 1;

            $display("Cycles Used: %d", Cycles);
            for (i=0; i<512; i = i+1) begin
                $display("%b", testcpu.mainmemory.DATA_RAM[i]);
            end
            $finish;
        end
    end
endmodule