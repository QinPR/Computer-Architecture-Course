// `include "PC.v"
// `include "InstructionRAM.v"
// `include "IF_ID.v"
// `include "ID.v"
// `include "ID_EX.v"
// `include "EX.v"
// `include "EX_MEM.v"
// `include "MainMemory.v"
// `include "MEM_WB.v"
// `include "Forwarding_Unit.v"
// `include "Hazard_Detection_Unit.v"

module cpu(CLOCK);
    input CLOCK;
    // PC
    wire PCWrite, PCsrc;
    wire [31:0] branch_addr, PCplus4F, FETCH_ADDRESS;
    // INSTRUCTIONRAM
    wire  RESET;          // instructionRAM reset
    wire  ENABLE;         // instructionRAM reset
    wire [31:0] DATA;
    // IF_ID
    wire [31:0] instructionD;
    wire [31:0] PCplus4D;
    // ID
    wire RegWriteW, hazard_flush, RegWriteD, MemtoRegD, ALUSrcD, branch_signal, ENABLED;
    wire [4:0] wb_addrW, saD, rs_addrD, rt_addrD, wb_addrD;
    wire [11:0] ALUControlD;
    wire [31:0] ResultW, rs_valueD, rt_valueD, immD, IF_FLUSH;
    // ID_EX
    wire RegWriteE, MemtoRegE, MemWriteE, ALUSrcE, ENABLEE, branch_signalE;
    wire [4:0] saE, rs_addrE, rt_addrE, wb_addrE;
    wire [11:0] ALUControlE;
    wire [31:0] immE, rs_valueE, rt_valueE;
    // EX
    wire dont_branch, JR_branch_signal, JR_EX_NOP;
    wire [31:0] ALUOutE, rtE, JR_IF_FLUSH, JR_branch_addr, PCplus4E;
    // EX_MEM
    wire RegWriteM, MemtoRegM;
    wire [4:0] rs_addrM, rt_addrM, wb_addrM;
    wire [31:0] ALUOutM;
    // MEM
    wire RESET_MEM, ENABLE_MEM;
    wire [31:0] DATA_MEM;
    wire [64:0] EDIT_SERIAL;
    // MEM_WB

    // FOWARD_UNIT
    wire [1:0] ForwardA, ForwardB;
    // HAZARD_DETECTION_UNIT
    wire IF_IDWrite, EX_NOP;


    PC pc_calculate(CLOCK, branch_addr, PCWrite, branch_signal, PCplus4F, FETCH_ADDRESS, ENABLE, RESET, dont_branch, JR_branch_addr, JR_branch_signal);
    InstructionRAM instruction_fecth(CLOCK, RESET, ENABLE, FETCH_ADDRESS, DATA);
    IF_ID if_id(IF_FLUSH, CLOCK, DATA, PCplus4F, instructionD, PCplus4D, dont_branch, IF_IDWrite, JR_IF_FLUSH);
    ID id(CLOCK, PCplus4D, instructionD, RegWriteW, wb_addrW, ResultW, hazard_flush, saD, 
    RegWriteD,ENABLED, MemtoRegD, MemWriteD, ALUControlD, ALUSrcD, branch_addr, rs_valueD, rt_valueD, 
    wb_addrD, branch_signal, immD, rs_addrD, rt_addrD, IF_FLUSH, wb_addrE, ALUOutE);
    ID_EX id_ex(RegWriteD, MemtoRegD, MemWriteD, ALUControlD, ALUSrcD, rs_valueD, rt_valueD, immD, wb_addrD, 
    CLOCK, saD,ENABLED, rs_addrD, rt_addrD, branch_signal, EX_NOP, JR_EX_NOP, PCplus4D,RegWriteE, MemtoRegE,
    MemWriteE, ALUControlE, ALUSrcE, immE, wb_addrE, rs_valueE, rt_valueE, saE, ENABLEE, rs_addrE, rt_addrE, 
    branch_signalE, PCplus4E);
    EX ex(CLOCK, immE, ALUSrcE, rs_valueE, rt_valueE, ALUControlE, ALUOutE, saE, ForwardA, 
    ForwardB, ResultW, rtE, branch_signalE, dont_branch, DATA_MEM, JR_IF_FLUSH, JR_branch_signal, 
    JR_branch_addr, JR_EX_NOP, PCplus4E);
    EX_MEM ex_mem(ALUOutE,rtE,wb_addrE,RegWriteE,MemtoRegE,CLOCK, MemWriteE,ENABLEE,
    rs_addrE, rt_addrE ,EDIT_SERIAL, ALUOutM, wb_addrM, RegWriteM, MemtoRegM, ENABLE_MEM, rs_addrM, rt_addrM);
    MainMemory mainmemory(CLOCK, RESET_MEM, ENABLE_MEM, ALUOutM, EDIT_SERIAL, DATA_MEM);
    MEM_WB mem_wb(ALUOutM, DATA_MEM, CLOCK, RegWriteM, MemtoRegM, wb_addrM, RegWriteW, ResultW, wb_addrW);
    Forwarding_Unit forward(CLOCK, RegWriteM, wb_addrE, wb_addrM, rs_addrD, rt_addrD, rs_addrE, rt_addrE, RegWriteE, ForwardA, ForwardB);
    Hazard_Detection_Unit no_hazard(CLOCK, MemWriteE, ENABLEE, wb_addrD, instructionD, PCWrite, IF_IDWrite, EX_NOP);
    
endmodule