#!/usr/bin/perl
if (!$::Driver) { use FindBin; exec("$FindBin::Bin/bootstrap.pl", @ARGV, $0); die; }
# DESCRIPTION: Verilator: Verilog Test driver/expect definition
#
# Copyright 2003-2009 by Wilson Snyder. This program is free software; you can
# redistribute it and/or modify it under the terms of either the GNU
# Lesser General Public License Version 3 or the Perl Artistic License
# Version 2.0.

scenarios(vlt_all => 1);

compile(
    make_top_shell => 0,
    make_main => 0,
    verilator_flags2 => ["-Wno-MULTITOP --exe $Self->{t_dir}/$Self->{name}.cpp"],
    );

execute(
    check_finished => 1,
    );

file_grep($Self->{run_log_filename}, qr!In 'a'!);
file_grep($Self->{run_log_filename}, qr!In 'a.sub'!);
file_grep($Self->{run_log_filename}, qr!In 'b'!);
file_grep($Self->{run_log_filename}, qr!In 'b.sub'!);
file_grep($Self->{run_log_filename}, qr!In 'c'!);
file_grep($Self->{run_log_filename}, qr!In 'c.sub'!);

ok(1);
1;
