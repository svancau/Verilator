// DESCRIPTION: Verilator: Verilog Test module
//
// This file ONLY is placed into the Public Domain, for any use,
// without warranty, 2013 by Wilson Snyder.

interface ifc;
   integer ok;
   modport out_modport (output ok);
endinterface

module t (/*AUTOARG*/);

   ifc itop();

   counter_ansi c1 (.isub(itop),
                    .i_value(4'h4));

endmodule

module counter_ansi
  (
   ifc.oop_modport isub,  // Bad
   input logic [3:0] i_value
   );
endmodule
